#pragma once

/**
 * @brief Generate a unique identifier.
 * @return A unique identifier.
 *
 * @warning The UID is only unique for the current session and will reset to 0 on reboot.
 * It should not be used for persistent identification across reboots.
 */
int uid();
