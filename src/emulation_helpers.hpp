#pragma once

#ifdef EMULATE

#define json_is_obj(o) o.is_object()
#define json_is_str(o) o.is_string()
#define json_is_null(o) o.is_null()

#define json_to_str(o) o.get<String>()
#define json_to(type, obj) obj.get<type>()

#else

#define json_is_obj(o) o.is<JsonObject>()
#define json_is_str(o) o.is<JsonString>()
#define json_is_null(o) o.isNull()

#define json_to_str(o) o.as<String>()
#define json_to(type, obj) obj.as<type>()

#endif
