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

//--------------------------------------------------------------------+
// Tap-Hold State
//--------------------------------------------------------------------+

// Tap-Hold stage
typedef enum {
  TAP_HOLD_STAGE_NONE = 0,
  TAP_HOLD_STAGE_TAP,
  TAP_HOLD_STAGE_HOLD,
} ak_tap_hold_stage_t;

// Tap-Hold state
typedef struct {
  // Time when the key was pressed
  uint32_t since;
  // Tap-Hold stage
  uint8_t stage;
} ak_state_tap_hold_t;

//--------------------------------------------------------------------+
// Toggle State
//--------------------------------------------------------------------+

// Toggle stage
typedef enum {
  TOGGLE_STAGE_NONE = 0,
  TOGGLE_STAGE_TOGGLE,
  TOGGLE_STAGE_NORMAL,
} ak_toggle_stage_t;

// Toggle state
typedef struct {
  // Time when the key was pressed
  uint32_t since;
  // Toggle stage
  uint8_t stage;
  // Whether the key is toggled
  bool is_toggled;
} ak_state_toggle_t;

//--------------------------------------------------------------------+
// Advanced Key State
//--------------------------------------------------------------------+

// Advanced key state
typedef union {
  ak_state_tap_hold_t tap_hold;
  ak_state_toggle_t toggle;
} advanced_key_state_t;

//--------------------------------------------------------------------+
// Advanced Key Event
//--------------------------------------------------------------------+

// Key event type. The events are arranged in this order to allow for easy
// access to the DKS action bitmaps.
typedef enum {
  AK_EVENT_TYPE_NONE = 0,
  AK_EVENT_TYPE_HOLD,
  AK_EVENT_TYPE_PRESS,
  AK_EVENT_TYPE_BOTTOM_OUT,
  AK_EVENT_TYPE_RELEASE_FROM_BOTTOM_OUT,
  AK_EVENT_TYPE_RELEASE,
} ak_event_type_t;

// Advanced key event
typedef struct {
  // Key event type
  uint8_t type;
  // Key index
  uint8_t key;
  // Underlying keycode. Only for Null Bind advanced keys
  uint8_t keycode;
  // Advanced key index associated with the key
  uint8_t ak_index;
} advanced_key_event_t;

//--------------------------------------------------------------------+
// Advanced Key API
//--------------------------------------------------------------------+

/**
 * @brief Initialize the advanced key module
 *
 * @return None
 */
void advanced_key_init(void);

/**
 * @brief Clear advanced key states
 *
 * This function clears the advanced key states. It should be called before the
 * profile changes or the advanced keys are updated.
 *
 * @return None
 */
void advanced_key_clear(void);

/**
 * @brief Process an advanced key event
 *
 * @param event Advanced key event to process
 *
 * @return None
 */
void advanced_key_process(const advanced_key_event_t *event);

/**
 * @brief Advanced key tick
 *
 * This function is called periodically to update the time-based advanced keys
 * (e.g., Tap-Hold and Toggle keys).
 *
 * @param has_non_tap_hold_press Whether there is a non-Tap-Hold key press
 *
 * @return None
 */
void advanced_key_tick(bool has_non_tap_hold_press);
