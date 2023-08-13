# plugin-sdk

A Software Development Kit (SDK) for the ECFMP Flow Control API

## Integrating With The SDK

### Create a HTTP Client

Rather than ship with an opinionated viewpoint as to what you should use for HTTP requests, this SDK
provides an interface `HttpClient` that the HTTP client of your choice must implement.

### Create a logger

You can optionally pass the SDK a logging class that implements the `Logger` interface. This will allow you to log
messages from the SDK.

### Create an instance of the SDK

To create an instance of the SDK, you must use the `SDKFactory` class. This class will allow you to configure the SDK,
and
then create an instance of it.

```c++
#include "ECFMP.h"

auto http = std::make_shared<MyHttpClient>();
auto logger = std::make_shared<MyLogger>();
auto ecfmp = ECFMP::Plugin::SdkFactory::Build()
        .WithLogger(logger)
        .WithHttpClient(std::move(http)).Instance();
```

### Add the SDK to EuroScopes timer event

The SDK needs to be called periodically to process events. To do this, you must add the call to the EuroScopes timer
event. This is necessary because EuroScope requires that calls to its internal classes are made from the plugin thread
(as opposed to something asynchronous.)

```c++
void MyPlugin::OnTimer(int time)
{
    ecfmp.OnEuroscopeTimerTick();
}
```

### Register event handlers

You can register event handlers with the SDK. These event handlers will be called when the SDK processes an event.
These listeners must implement the `EventListener` interface.

```c++
auto eventListener = std::make_shared<MyListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>();
ecfmp.EventBus().Subscribe<ECFMP::Plugin::FlowMeasureActivatedEvent>(eventListener);
```

## Testing Your Integration

You can test your integration by making use of the mocks provided by the SDK. These mocks will allow you to simulate
events that would be sent by the ECFMP SDK.

You can find the mocks in `include/mock`, just be sure to include the main include `ECFMP.h` as well as the mocks
header `ecfmp-sdk-mock.h`.

## Known Limitations

At the moment, the SDK has the following limitations:

- The SDK does not support Event Participation filters on Flow Measures. This is because EuroScope does not provide
  the CID of the aircraft that is being filtered on, and thus a way around this needs to be devised.

## Development

### Building

This project builds using CMake. You can build using a command similar to below

`cmake -DCMAKE_BUILD_TYPE=<Release|Debug> -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_DEPENDS_USE_COMPILER=FALSE -G Ninja -Bbuild`

### Running Tests

Tests can be run using the `ctest` command, from the build directory:

`ctest -C Release --output-on-failure --no-tests=error`

## Design Rationale

The SDK has the following design rationale.

### Event Driven

Similar to how EuroScope is event driven, so is this SDK. Integrations can register a series of event handlers with the
SDK, which will pass on events, such as new Flow Measures for processing.

The SDK itself is also event driven, and uses an internal event bus to handle events.

### Async

EuroScope is a single-threaded application when it comes to plugins, therefore, anything that may take a while (e.g.
HTTP requests) will be done asynchronously. The results of these operations will be deferred for when the EuroScope
thread comes back around, as EuroScope sometimes doesn't like things interacting with it asynchronously.
