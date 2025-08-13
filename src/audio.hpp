#pragma once

#include "audio/player.hpp"

namespace audio {

/**
 * @brief Check if the audio format is supported.
 * @param ext The file extension of the audio format.
 * @return True if the format is supported, false otherwise.
 */
bool supported(const String &ext);

} // namespace audio
