/*
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "common.h"
#include "wear_leveling.h"

//--------------------------------------------------------------------+
// Keyboard Persistent Configuration
//--------------------------------------------------------------------+

// Persistent configuration version. The size of the configuration must be
// non-decreasing, so that the migration can assume that the new version is at
// least as large as the previous version.
#define EECONFIG_VERSION 0x0101
// Magic number to identify the start of the configuration
#define EECONFIG_MAGIC_START 0x0A42494C
// Magic number to identify the end of the configuration
#define EECONFIG_MAGIC_END 0x0A4B4D48

// Keyboard calibration configuration
typedef struct __attribute__((packed)) {
  // Initial rest value of the key matrix. If the value is smaller than the
  // actual rest value, the key will have a dead zone at the beginning of the
  // keystroke. If the value is larger than the actual rest value, a longer
  // calibration process may be required.
  uint16_t initial_rest_value;
  // Minimum change in ADC values for the key to be considered bottom-out. If
  // the value is larger than the actual bottom-out threshold, the key will have
  // a dead zone at the end of the keystroke. If the value is smaller than the
  // actual bottom-out threshold, the distance calculation may be inaccurate
  // until the first bottom-out event.
  uint16_t initial_bottom_out_threshold;
} eeconfig_calibration_t;

// Keyboard options configuration
typedef union {
  struct __attribute__((packed)) {
    // Whether the XInput interface is enabled
    bool xinput_enabled : 1;
    // Reserved bits for future use
    uint16_t reserved : 15;
  };
  uint16_t raw;
} eeconfig_options_t;

// Keyboard profile configuration
typedef struct __attribute__((packed)) {
  uint8_t keymap[NUM_LAYERS][NUM_KEYS];
  actuation_t actuation_map[NUM_KEYS];
  advanced_key_t advanced_keys[NUM_ADVANCED_KEYS];
  uint8_t gamepad_buttons[NUM_KEYS];
  gamepad_options_t gamepad_options;
  uint8_t tick_rate;
} eeconfig_profile_t;

// Keyboard configuration
typedef struct __attribute__((packed)) {
  // Global configurations
  // Magic number to identify the start of the configuration
  uint32_t magic_start;
  // Version of the configuration
  uint16_t version;

  // Calibration configuration
  eeconfig_calibration_t calibration;
  // Options configuration
  eeconfig_options_t options;

  // Current profile index
  uint8_t current_profile;
  // Last non-default profile index, used for profile swapping
  uint8_t last_non_default_profile;
  // End of global configurations

  // Profiles
  eeconfig_profile_t profiles[NUM_PROFILES];

  // Magic number to identify the end of the configuration
  uint32_t magic_end;
} eeconfig_t;

_Static_assert(
    sizeof(eeconfig_t) <= WL_VIRTUAL_SIZE,
    "Keyboard configuration size must be at most the virtual storage size.");

extern const eeconfig_t *eeconfig;

#define CURRENT_PROFILE (eeconfig->profiles[eeconfig->current_profile])

//--------------------------------------------------------------------+
// Default Keyboard Configuration
//--------------------------------------------------------------------+

#if !defined(DEFAULT_CALIBRATION)
#error "DEFAULT_CALIBRATION is not defined"
#endif

#if !defined(DEFAULT_OPTIONS)
#define DEFAULT_OPTIONS                                                        \
  {                                                                            \
  }
#endif

#if !defined(DEFAULT_KEYMAP)
// Default keymap
#define DEFAULT_KEYMAP                                                         \
  {                                                                            \
    [0 ... NUM_LAYERS - 1] = { [0 ... NUM_KEYS - 1] = KC_NO }                  \
  }
#endif

#if !defined(DEFAULT_ACTUATION_MAP)
// Default actuation map
#define DEFAULT_ACTUATION_MAP                                                  \
  {                                                                            \
      [0 ... NUM_KEYS - 1] =                                                   \
          {                                                                    \
              .actuation_point = 128,                                          \
              .rt_down = 0,                                                    \
              .rt_up = 0,                                                      \
              .continuous = false,                                             \
          },                                                                   \
  }
#endif

#if !defined(DEFAULT_ADVANCED_KEYS)
// Default advanced keys
#define DEFAULT_ADVANCED_KEYS                                                  \
  {                                                                            \
  }
#endif

#if !defined(DEFAULT_GAMEPAD_BUTTONS)
// Default gamepad buttons
#define DEFAULT_GAMEPAD_BUTTONS {[0 ... NUM_KEYS - 1] = GP_BUTTON_NONE}
#endif

#if !defined(DEFAULT_GAMEPAD_OPTIONS)
// Default gamepad options
#define DEFAULT_GAMEPAD_OPTIONS                                                \
  {                                                                            \
      .analog_curve = {{4, 20}, {85, 95}, {165, 170}, {255, 255}},             \
      .keyboard_enabled = true,                                                \
      .snappy_joystick = true,                                                 \
  }
#endif

#if !defined(DEFAULT_TICK_RATE)
// Default tick rate
#define DEFAULT_TICK_RATE 30
#endif

#if !defined(DEFAULT_PROFILE)
// Default profile
#define DEFAULT_PROFILE                                                        \
  {                                                                            \
      .keymap = DEFAULT_KEYMAP,                                                \
      .actuation_map = DEFAULT_ACTUATION_MAP,                                  \
      .advanced_keys = DEFAULT_ADVANCED_KEYS,                                  \
      .gamepad_buttons = DEFAULT_GAMEPAD_BUTTONS,                              \
      .gamepad_options = DEFAULT_GAMEPAD_OPTIONS,                              \
      .tick_rate = DEFAULT_TICK_RATE,                                          \
  }
#endif

//--------------------------------------------------------------------+
// Persistent Configuration API
//--------------------------------------------------------------------+

/**
 * @brief Initialize the persistent configuration module
 *
 * @return None
 */
void eeconfig_init(void);

/**
 * @brief Reset the persistent configuration to default values
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_reset(void);

/**
 * @brief Set the calibration configuration
 *
 * @param calibration New calibration configuration
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_set_calibration(const void *calibration);

/**
 * @brief Set the options configuration
 *
 * @param options New options configuration
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_set_options(const void *options);

/**
 * @brief Set the current profile
 *
 * @param profile Profile index
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_set_current_profile(uint8_t profile);

/**
 * @brief Set the keymap for a profile
 *
 * @param profile Profile index
 * @param layer Layer index
 * @param start Starting key index
 * @param len Length of the partial keymap
 * @param keymap New partial keymap
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_set_keymap(uint8_t profile, uint8_t layer, uint8_t start,
                         uint8_t len, const void *keymap);

/**
 * @brief Set the actuation map for a profile
 *
 * @param profile Profile index
 * @param start Starting key index
 * @param len Length of the partial actuation map
 * @param actuation_map New partial actuation map
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_set_actuation_map(uint8_t profile, uint8_t start, uint8_t len,
                                const void *actuation_map);

/**
 * @brief Set the advanced keys for a profile
 *
 * @param profile Profile index
 * @param start Starting advanced key index
 * @param len Length of the partial advanced keys
 * @param advanced_keys New partial advanced keys
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_set_advanced_keys(uint8_t profile, uint8_t start, uint8_t len,
                                const void *advanced_keys);

/**
 * @brief Set the gamepad buttons for a profile
 *
 * @param profile Profile index
 * @param start Starting button index
 * @param len Length of the partial buttons
 * @param buttons New partial gamepad buttons
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_set_gamepad_buttons(uint8_t profile, uint8_t start, uint8_t len,
                                  const void *buttons);

/**
 * @brief Set the gamepad options for a profile
 *
 * @param profile Profile index
 * @param options New gamepad options
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_set_gamepad_options(uint8_t profile, const void *options);

/**
 * @brief Set the tick rate
 *
 * @param profile Profile index
 * @param tick_rate New tick rate value
 *
 * @return true if successful, false otherwise
 */
bool eeconfig_set_tick_rate(uint8_t profile, uint8_t tick_rate);
