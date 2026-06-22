/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>

#include <zmk/keys.h>
#include <zmk/hid.h>
#include <zmk/endpoints_types.h>

#if IS_ENABLED(CONFIG_ZMK_HID_IO_JOYSTICK)
#include <zmk/hid-io/joystick.h>
#include <zmk/hid-io/hid_joystick.h>
#define ZMK_HID_JOYSTICK_NUM_BUTTONS 0x08
#define ZMK_HID_REPORT_ID__IO_JOYSTICK 0x02
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_JOYSTICK)

#if IS_ENABLED(CONFIG_ZMK_HID_IO_MOUSE)
#include <zmk/hid-io/mouse.h>
#include <zmk/hid-io/hid_mouse.h>
#define ZMK_HID_MOUSE_NUM_BUTTONS 0x05
#define ZMK_HID_REPORT_ID__IO_MOUSE 0x03
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_MOUSE)

#if IS_ENABLED(CONFIG_ZMK_HID_IO_OUTPUT)
#include <zmk/hid-io/hid_output.h>
#define ZMK_HID_REPORT_ID__IO_OUTPUT 0x04
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_OUTPUT)

#if IS_ENABLED(CONFIG_ZMK_HID_IO_VOLUME_KNOB)
#include <zmk/hid-io/hid_volume_knob.h>
#define ZMK_HID_REPORT_ID__IO_VOLUME_KNOB 0x05
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_VOLUME_KNOB)

#include <dt-bindings/zmk/hid_usage.h>
#include <dt-bindings/zmk/hid_usage_pages.h>

// See https://www.usb.org/sites/default/files/hid1_11.pdf section 6.2.2.4 Main Items

#ifndef HID_USAGE_PAGE16
#define HID_USAGE_PAGE16(page, page2)                                                              \
    HID_ITEM(HID_ITEM_TAG_USAGE_PAGE, HID_ITEM_TYPE_GLOBAL, 2), page, page2
#endif

#define HID_USAGE16(a, b) HID_ITEM(HID_ITEM_TAG_USAGE, HID_ITEM_TYPE_LOCAL, 2), a, b

#define HID_USAGE16_SINGLE(a) HID_USAGE16((a & 0xFF), ((a >> 8) & 0xFF))

static const uint8_t zmk_hid_report_desc_alt[] = {
    
    // HID_USAGE_PAGE16(0x0C, 0xFF),
    // HID_USAGE(0x01),
    // HID_COLLECTION(HID_COLLECTION_APPLICATION),

#if IS_ENABLED(CONFIG_ZMK_HID_IO_JOYSTICK)
    /* Vendor-defined top-level collection (page 0xFF00, usage 0x01) instead of
     * Generic-Desktop / Joystick. DirectInput, XInput, and the "Set up USB game
     * controllers" panel classify a device by its top-level application-collection
     * usage, so a vendor page keeps the faders out of all of them -- a disconnect
     * can no longer steal a real gamepad's player slot. The report layout below
     * (report id 2, six 16-bit axes + 8 button bits) is unchanged; the PC app
     * finds the device by VID/PID + this vendor usage. */
    HID_USAGE_PAGE16(0x00, 0xFF),
    HID_USAGE(0x01),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(ZMK_HID_REPORT_ID__IO_JOYSTICK),
    HID_COLLECTION(HID_COLLECTION_LOGICAL),
    HID_USAGE_PAGE(HID_USAGE_GEN_DESKTOP),
    HID_USAGE(HID_USAGE_GD_X),
    HID_USAGE(HID_USAGE_GD_Y),
    HID_USAGE(HID_USAGE_GD_Z),
    HID_USAGE(HID_USAGE_GD_RX),
    HID_USAGE(HID_USAGE_GD_RY),
    HID_USAGE(HID_USAGE_GD_RZ),
    // 16-bit axes (was 8) so faders keep their full ADC resolution on the wire.
    HID_LOGICAL_MIN16(0xFF, -0x7F),
    HID_LOGICAL_MAX16(0xFF, 0x7F),
    HID_REPORT_SIZE(0x10),
    HID_REPORT_COUNT(0x06),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_REL),
    HID_USAGE_PAGE(HID_USAGE_BUTTON),
    HID_USAGE_MIN8(0x1),
    HID_USAGE_MAX8(ZMK_HID_JOYSTICK_NUM_BUTTONS),
    HID_LOGICAL_MIN8(0x00),
    HID_LOGICAL_MAX8(0x01),
    HID_REPORT_SIZE(0x01),
    HID_REPORT_COUNT(0x8),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),
    HID_END_COLLECTION,
    HID_END_COLLECTION,
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_JOYSTICK)

#if IS_ENABLED(CONFIG_ZMK_HID_IO_MOUSE)
    HID_USAGE_PAGE(HID_USAGE_GD),
    HID_USAGE(HID_USAGE_GD_MOUSE),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(ZMK_HID_REPORT_ID__IO_MOUSE),
    HID_USAGE(HID_USAGE_GD_POINTER),
    HID_COLLECTION(HID_COLLECTION_PHYSICAL),
    HID_USAGE_PAGE(HID_USAGE_BUTTON),
    HID_USAGE_MIN8(0x1),
    HID_USAGE_MAX8(ZMK_HID_MOUSE_NUM_BUTTONS),
    HID_LOGICAL_MIN8(0x00),
    HID_LOGICAL_MAX8(0x01),
    HID_REPORT_SIZE(0x01),
    HID_REPORT_COUNT(0x5),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),
    // Constant padding for the last 3 bits.
    HID_REPORT_SIZE(0x03),
    HID_REPORT_COUNT(0x01),
    HID_INPUT(ZMK_HID_MAIN_VAL_CONST | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),
    // Some OSes ignore pointer devices without X/Y data.
    HID_USAGE_PAGE(HID_USAGE_GEN_DESKTOP),
    HID_USAGE(HID_USAGE_GD_X),
    HID_USAGE(HID_USAGE_GD_Y),
    HID_USAGE(HID_USAGE_GD_WHEEL),
    HID_LOGICAL_MIN16(0xFF, -0x7F),
    HID_LOGICAL_MAX16(0xFF, 0x7F),
    HID_REPORT_SIZE(0x10),
    HID_REPORT_COUNT(0x03),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_REL),
    HID_USAGE_PAGE(HID_USAGE_CONSUMER),
    HID_USAGE16_SINGLE(HID_USAGE_CONSUMER_AC_PAN),
    HID_LOGICAL_MIN16(0xFF, -0x7F),
    HID_LOGICAL_MAX16(0xFF, 0x7F),
    HID_REPORT_SIZE(0x08),
    HID_REPORT_COUNT(0x01),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_REL),
    HID_END_COLLECTION,
    HID_END_COLLECTION,
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_MOUSE)

#if IS_ENABLED(CONFIG_ZMK_HID_IO_OUTPUT)
    HID_USAGE_PAGE(HID_USAGE_HAPTICS),
    HID_USAGE_MIN8(0x0),
    HID_USAGE_MAX8(0xFF),
    HID_REPORT_COUNT(0x2),
    HID_REPORT_SIZE(0x08),
    HID_OUTPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_OUTPUT)

#if IS_ENABLED(CONFIG_ZMK_HID_IO_VOLUME_KNOB)
    HID_USAGE_PAGE(HID_USAGE_CONSUMER),
    HID_USAGE(HID_USAGE_CONSUMER_VOLUME),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(ZMK_HID_REPORT_ID__IO_VOLUME_KNOB),
    HID_LOGICAL_MIN8(0),
    HID_LOGICAL_MAX8(100),
    HID_REPORT_COUNT(0x1),
    HID_REPORT_SIZE(0x07),
    HID_OUTPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_REL
             | ZMK_HID_MAIN_VAL_NO_WRAP | ZMK_HID_MAIN_VAL_LIN 
             | ZMK_HID_MAIN_VAL_NO_PREFERRED),
    HID_END_COLLECTION,
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_OUTPUT)

    // HID_END_COLLECTION,
};
