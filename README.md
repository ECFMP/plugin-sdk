# plugin-sdk

A Software Development Kit (SDK) for the ECFMP Flow Control API

## Integrating With The SDK

Coming soon!

## Building

This project builds using CMake. You can build using a command similar to below

`cmake -DCMAKE_BUILD_TYPE=<Release|Debug> -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_DEPENDS_USE_COMPILER=FALSE -G Ninja -Bbuild`

## Running Tests

Tests can be run using the `ctest` command, from the build directory:

`ctest -C Release --output-on-failure --no-tests=error`

## Design Rationale

The SDK has the following design rationale.

### Event Driven

Similar to how EuroScope is event driven, so is this SDK. Integrations can register a series of event handlers with the SDK, which will pass on events, such as new Flow Measures for processing.

### Async

EuroScope is a single-threaded application when it comes to plugins, therefore, anything that may take a while (e.g. HTTP requests) will be done aysynchronously. The results of these operations will be deferred for when the EuroScope thread comes back around, as EuroScope sometimes doesn't like things interacting with it asynchronously.
