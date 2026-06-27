#pragma once

#ifdef EMULATE

#define json_is_obj(o) o.is_object()
#define json_is_str(o) o.is_string()
#define json_is_null(o) o.is_null()
#define json_is_array(o) o.is_array()

#define json_to_str(o) o.get<String>()
#define json_to(type, obj) obj.get<type>()
#define json_to_array(obj) obj.get<JsonArray>()

#define json_array_length(o) o.size()
#define json_contains_key(o, key) o.contains(key)

#else

#define json_is_obj(o) o.is<JsonObject>()
#define json_is_str(o) o.is<JsonString>()
#define json_is_null(o) o.isNull()
#define json_is_array(o) o.is<JsonArray>()

#define json_to_str(o) o.as<String>()
#define json_to(type, obj) obj.as<type>()
#define json_to_array(obj) obj.as<JsonArrayConst>()

#define json_array_length(o) o.length()
#define json_contains_key(o, key) o.containsKey(key)

#endif

#define json_to_int(o) (json_is_null(o) ? 0L : json_to(long, o))
#define json_optional_to(type, obj) (json_is_null(obj) ? optional<type>{} : optional<type>{json_to(type, obj)})
#define json_to_or(type, obj, alt) (json_is_null(obj) ? alt : json_to(type, obj))
#define json_choose_key(obj, key1, key2) (json_is_null(obj[key1]) ? obj[key2] : obj[key1])
#define json_optional_key_to(type, obj, key) (json_contains_key(obj, key) ? optional<type>{json_to(type, obj[key])} : optional<type>{})
