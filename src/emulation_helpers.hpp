#pragma once

#ifdef EMULATE

#define json_is_obj(o) o.is_object()
#define json_is_str(o) o.is_string()
#define json_is_null(o) o.is_null()
#define json_is_array(o) o.is_array()

#define json_to_str(o) o.get<String>()
#define json_to(type, obj) obj.get<type>()

#define json_array_length(o) o.size()
#define json_contains_key(o, key) o.contains(key)

#else

#define json_is_obj(o) o.is<JsonObject>()
#define json_is_str(o) o.is<JsonString>()
#define json_is_null(o) o.isNull()
#define json_is_array(o) o.is<JsonArray>()

#define json_to_str(o) o.as<String>()
#define json_to(type, obj) obj.as<type>()

#define json_array_length(o) o.length()
#define json_contains_key(o, key) o.containsKey(key)

#endif

#define json_to_int(o) (json_is_null(o) ? 0L : json_to(long, o))
