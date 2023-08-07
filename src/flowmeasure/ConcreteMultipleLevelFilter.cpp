#include "ConcreteMultipleLevelFilter.h"
#include "euroscope/EuroscopeAircraft.h"

namespace ECFMP::FlowMeasure {
    ConcreteMultipleLevelFilter::ConcreteMultipleLevelFilter(std::vector<int> levels) : levels(std::move(levels))
    {
        for (const auto& level: this->levels) {
            this->altitudes.push_back(level * 100);
        }
    }

    auto ConcreteMultipleLevelFilter::Levels() const noexcept -> std::vector<int>
    {
        return this->levels;
    }

    auto ConcreteMultipleLevelFilter::Altitudes() const noexcept -> std::vector<int>
    {
        return this->altitudes;
    }

    auto ConcreteMultipleLevelFilter::ApplicableToLevel(int level) const noexcept -> bool
    {
        return std::find(this->levels.begin(), this->levels.end(), level) != this->levels.end();
    }

    auto ConcreteMultipleLevelFilter::ApplicableToAltitude(int altitude) const noexcept -> bool
    {
        return std::find(this->altitudes.begin(), this->altitudes.end(), altitude) != this->altitudes.end();
    }

    bool ConcreteMultipleLevelFilter::ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft) const noexcept
    {
        return ApplicableToAltitude(aircraft.CruiseAltitude());
    }
}// namespace ECFMP::FlowMeasure
