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
// Deferred Actions
//--------------------------------------------------------------------+

#if !defined(HID_MAX_DEFERRED_ACTIONS)
// Maximum number of deferred actions for each HID interface
#define HID_MAX_DEFERRED_ACTIONS 16
#endif

// Deferred action type
typedef enum {
  DEFERRED_ACTION_TYPE_PRESS = 0,
  DEFERRED_ACTION_TYPE_RELEASE,
  DEFERRED_ACTION_TYPE_TAP,
  DEFERRED_ACTION_COUNT,
} hid_deferred_action_type_t;

// Deferred action. This action will be executed after the final report of the
// current matrix scan is sent. This is useful for tapping keys, where the key
// should be immediately released after the report is sent.
typedef struct {
  // Action to perform
  uint8_t type;
  // Key to perform the action on
  uint8_t key;
  // Keycode associated with the action
  uint8_t keycode;
} hid_deferred_action_t;

//--------------------------------------------------------------------+
// HID API
//--------------------------------------------------------------------+

/**
 * @brief Initialize the HID module
 *
 * @return None
 */
void hid_init(void);

/**
 * @brief Add a keycode to the appropriate HID report
 *
 * @param keycode Keycode to add
 *
 * @return None
 */
void hid_keycode_add(uint8_t keycode);

/**
 * @brief Remove a keycode from the appropriate HID report
 *
 * @param keycode Keycode to remove
 *
 * @return None
 */
void hid_keycode_remove(uint8_t keycode);

/**
 * @brief Send all HID reports
 *
 * This function will block until the device is ready to send the reports.
 *
 * @return None
 */
void hid_send_reports(void);
