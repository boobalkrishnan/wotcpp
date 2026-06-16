# WoTCpp

WoTCpp contains the core Web of Things runtime abstractions used by the gateway, including the `WoT` facade, servient support, and thing model elements (properties, actions, events, and Thing Description helpers).

## Contents

- `Core/include/`: public headers for WoT core entities
- `Core/src/`: implementation of WoT runtime classes
- `Schema/`: schema-related resources and validation assets

## Build behavior

`wotcpp` is integrated as part of the root CMake project and builds the `WoTCore` library via:

- `wotcpp/CMakeLists.txt` -> `add_subdirectory(Core)`
- `wotcpp/Core/CMakeLists.txt` -> `add_library(WoTCore ...)`

When building from the repository root, `WoTCore` is linked into the final `wot-gateway` executable through dependent modules.

## Main classes

- `WoT`: high-level entry point for producing/consuming things and attaching a servient
- `WoTServient`: server lifecycle and processing integration point
- `WoTThing`, `WoTProperty`, `WoTAction`, `WoTEvent`: core thing model entities
- `WoTTd`: Thing Description handling support

## Integration points

`WoTCore` links against:
- `cpp-logger` for logging
- `wot-binding-rpi` for protocol binding integration

This keeps protocol-specific behavior separate from the WoT model while still allowing runtime coordination through the servient.

## Future improvements

- Add focused unit tests for `WoTCore` classes
- Add usage examples for creating and exposing things
- Document error-handling and threading expectations in the processing loop

