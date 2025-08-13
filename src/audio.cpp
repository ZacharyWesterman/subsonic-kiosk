#include "audio.hpp"
#include "audio/player.hpp"

namespace audio {

static const std::vector<const char *> supportedTypes = {
	"wav",
};

bool supported(const String &ext) {
	return std::find(supportedTypes.begin(), supportedTypes.end(), ext) != supportedTypes.end();
}

} // namespace audio
