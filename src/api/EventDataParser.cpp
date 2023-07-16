#include "EventDataParser.h"
#include "ECFMP/event/EventParticipant.h"
#include "ECFMP/log/Logger.h"
#include "InternalApiElementCollection.h"
#include "date/ParseDateStrings.h"
#include "event/ConcreteEvent.h"
#include "event/ConcreteEventParticipant.h"
#include "nlohmann/json.hpp"

namespace ECFMP::Api {

    EventDataParser::EventDataParser(std::shared_ptr<Log::Logger> logger) : logger(std::move(logger))
    {
        assert(this->logger != nullptr && "Logger cannot be null");
    }

    auto EventDataParser::ParseEvents(const nlohmann::json& data, const InternalFlightInformationRegionCollection& firs)
            -> std::shared_ptr<InternalEventCollection>
    {
        auto events = std::make_shared<Api::InternalApiElementCollection<Event::Event>>();
        logger->Debug("Updating event data");
        if (!DataIsValid(data)) {
            logger->Error("Invalid event data from API");
            return nullptr;
        }

        for (const auto& event: data.at("events")) {
            if (!EventDataIsValid(event, firs)) {
                logger->Error("Invalid event in event data from API");
                logger->Debug("Failed updating event: " + event.dump());
                continue;
            }

            // Parse participants
            std::vector<std::shared_ptr<Event::EventParticipant>> participants;
            for (const auto& participant: event.at("participants")) {
                participants.push_back(std::make_shared<Event::ConcreteEventParticipant>(
                        participant.at("cid").get<int>(),
                        participant.at("origin").is_string() ? participant.at("origin").get<std::string>() : "",
                        participant.at("destination").is_string() ? participant.at("destination").get<std::string>()
                                                                  : ""
                ));
            }

            events->Add(std::make_shared<Event::ConcreteEvent>(
                    event.at("id").get<int>(), event.at("name").get<std::string>(),
                    Date::TimePointFromDateString(event.at("date_start").get<std::string>()),
                    Date::TimePointFromDateString(event.at("date_end").get<std::string>()),
                    firs.Get(event.at("flight_information_region_id").get<int>()),
                    event.at("vatcan_code").is_null() ? "" : event.at("vatcan_code").get<std::string>(), participants
            ));
        }

        logger->Debug("Finished updating events");
        return events;
    }

    auto EventDataParser::DataIsValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.contains("events") && data.at("events").is_array();
    }

    auto
    EventDataParser::EventDataIsValid(const nlohmann::json& data, const InternalFlightInformationRegionCollection& firs)
            -> bool
    {
        return data.is_object() && data.contains("id") && data.at("id").is_number_integer() && data.contains("name")
                && data.at("name").is_string() && DateValid(data, "date_start") && DateValid(data, "date_end")
                && data.contains("flight_information_region_id")
                && data.at("flight_information_region_id").is_number_integer()
                && firs.Get(data.at("flight_information_region_id").get<int>()) != nullptr
                && (data.contains("vatcan_code")
                    && (data.at("vatcan_code").is_null() || data.at("vatcan_code").is_string()))
                && data.contains("participants") && ParticipantsValid(data.at("participants"));
    }

    auto EventDataParser::ParticipantsValid(const nlohmann::json& data) -> bool
    {
        return data.is_array()
                && std::find_if_not(
                           data.begin(), data.end(),
                           [](const nlohmann::json& participant) {
                               return participant.is_object() && participant.contains("cid")
                                       && participant.at("cid").is_number_integer() && participant.contains("origin")
                                       && (participant.at("origin").is_string() || participant.at("origin").is_null())
                                       && participant.contains("destination")
                                       && (participant.at("destination").is_string()
                                           || participant.at("destination").is_null());
                           }
                   )
                == data.cend();
    }

    auto EventDataParser::DateValid(const nlohmann::json& data, const std::string& key) -> bool
    {
        return data.contains(key) && data.at(key).is_string() && Date::DateStringValid(data.at(key));
    }
}// namespace ECFMP::Api
