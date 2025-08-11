#pragma once

#include <Arduino_AdvancedAnalog.h>

#include "audio/player.hpp"

namespace audio
{
    const std::vector<const char *> supportedTypes = {
        "wav",
    };

    bool supported(const String &ext)
    {
        return std::find(supportedTypes.begin(), supportedTypes.end(), ext) != supportedTypes.end();
    }

}
