# Tips for development

It's a good idea to try to emulate any updates before verifying or uploading them to the Arduino, as those take a very long time compared to emulation. See my [emulator project](https://github.com/ZacharyWesterman/arduino-giga-emulator) for how to get that installed.

Note that debugging any low-level stuff (like how the *actual* network requests are handled) is not possible to test via emulation, but all higher-level logic is.

### Debugging JSON handling

The JSON decoding interface is different when emulating vs using real hardware.
Because of this, there are utility macros in `src/emulation_helpers.hpp` that abstract the actual logic away.
You should prefer using these macros whenever possible.

Do keep in mind there are some edge cases to be aware of!

- When indexing an object that may not contain a given key, make sure to **always** check that the key exists with `json_contains_key(object, "key")`.
- Never construct a `JsonObject`, instead prefer `JsonDocument`. This also means you should never do `auto var = json_to(JsonObject, object);`, instead prefer `JsonDocument var = object;`.
- Always use `json_to_array(object)` instead of `json_to(JsonArray, object)`. The former automatically handles constness.
