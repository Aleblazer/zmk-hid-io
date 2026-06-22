/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/init.h>
#include <zephyr/kernel.h>

#include <zmk/endpoints.h>
#include <zmk/hid.h>
#include <zmk/usb_hid.h>
#include <zmk/hog.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(hid_io, CONFIG_ZMK_HID_IO_LOG_LEVEL);

#include <zmk/hid-io/endpoints.h>
#include <zmk/hid-io/hid.h>
#include <zmk/hid-io/usb_hid.h>
#include <zmk/hid-io/hog.h>

#if IS_ENABLED(CONFIG_ZMK_HID_IO_JOYSTICK)
int zmk_endpoints_send_joystick_report_alt() {
    struct zmk_endpoint_instance current_instance = zmk_endpoint_get_selected();

    switch (current_instance.transport) {
#if IS_ENABLED(CONFIG_ZMK_USB)
    case ZMK_TRANSPORT_USB: {
        int err = zmk_usb_hid_send_joystick_report_alt();
        if (err) {
            LOG_ERR("FAILED TO SEND OVER USB: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_USB: break;
#endif /* IS_ENABLED(CONFIG_ZMK_USB) */

#if IS_ENABLED(CONFIG_ZMK_BLE)
    case ZMK_TRANSPORT_BLE: {
        struct zmk_hid_joystick_report_alt *joystick_report = zmk_hid_get_joystick_report_alt();
        int err = zmk_hog_send_joystick_report_alt(&joystick_report->body);
        if (err) {
            LOG_ERR("FAILED TO SEND OVER HOG: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_BLE: break;
#endif /* IS_ENABLED(CONFIG_ZMK_BLE) */

    case ZMK_TRANSPORT_NONE:  {
        LOG_ERR("Current endpoint transport: NONE");
        return 0;
    }

    }

    LOG_ERR("Unsupported endpoint transport %d", current_instance.transport);
    return -ENOTSUP;
}

#if CONFIG_ZMK_HID_IO_JOYSTICK_HEARTBEAT_MS > 0
// Periodically re-send the latched joystick axes over USB so a host app that
// connects/opens after the last movement still sees the current position
// (event-only reporting otherwise leaves the device silent at rest). USB-gated,
// so the battery-powered halves are never involved.
static void hid_io_joystick_heartbeat(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(hid_io_joystick_heartbeat_work, hid_io_joystick_heartbeat);

static void hid_io_joystick_heartbeat(struct k_work *work) {
    if (zmk_endpoint_get_selected().transport == ZMK_TRANSPORT_USB) {
        zmk_endpoints_send_joystick_report_alt();
    }
    k_work_reschedule(&hid_io_joystick_heartbeat_work,
                      K_MSEC(CONFIG_ZMK_HID_IO_JOYSTICK_HEARTBEAT_MS));
}

static int hid_io_joystick_heartbeat_init(void) {
    k_work_reschedule(&hid_io_joystick_heartbeat_work,
                      K_MSEC(CONFIG_ZMK_HID_IO_JOYSTICK_HEARTBEAT_MS));
    return 0;
}
SYS_INIT(hid_io_joystick_heartbeat_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
#endif
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_JOYSTICK)


#if IS_ENABLED(CONFIG_ZMK_HID_IO_MOUSE)
int zmk_endpoints_send_mouse_report_alt() {
    struct zmk_endpoint_instance current_instance = zmk_endpoint_get_selected();

    switch (current_instance.transport) {
#if IS_ENABLED(CONFIG_ZMK_USB)
    case ZMK_TRANSPORT_USB: {
        int err = zmk_usb_hid_send_mouse_report_alt();
        if (err) {
            LOG_ERR("FAILED TO SEND OVER USB: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_USB: break;
#endif /* IS_ENABLED(CONFIG_ZMK_USB) */

#if IS_ENABLED(CONFIG_ZMK_BLE)
    case ZMK_TRANSPORT_BLE: {
        struct zmk_hid_mouse_report_alt *mouse_report = zmk_hid_get_mouse_report_alt();
        int err = zmk_hog_send_mouse_report_alt(&mouse_report->body);
        if (err) {
            LOG_ERR("FAILED TO SEND OVER HOG: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_BLE: break;
#endif /* IS_ENABLED(CONFIG_ZMK_BLE) */

    case ZMK_TRANSPORT_NONE: return 0;
    }

    LOG_ERR("Unsupported endpoint transport %d", current_instance.transport);
    return -ENOTSUP;
}
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_MOUSE)


#if IS_ENABLED(CONFIG_ZMK_HID_IO_VOLUME_KNOB)
int zmk_endpoints_send_volume_knob_report_alt() {
    struct zmk_endpoint_instance current_instance = zmk_endpoint_get_selected();

    switch (current_instance.transport) {
#if IS_ENABLED(CONFIG_ZMK_USB)
    case ZMK_TRANSPORT_USB: {
        int err = zmk_usb_hid_send_volume_knob_report_alt();
        if (err) {
            LOG_ERR("FAILED TO SEND OVER USB: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_USB: break;
#endif /* IS_ENABLED(CONFIG_ZMK_USB) */

#if IS_ENABLED(CONFIG_ZMK_BLE)
    case ZMK_TRANSPORT_BLE: {
        struct zmk_hid_volume_knob_report_alt *volume_knob_report = zmk_hid_get_volume_knob_report_alt();
        int err = zmk_hog_send_volume_knob_report_alt(&volume_knob_report->body);
        if (err) {
            LOG_ERR("FAILED TO SEND OVER HOG: %d", err);
        }
        return err;
    }
#else
    case ZMK_TRANSPORT_BLE: break;
#endif /* IS_ENABLED(CONFIG_ZMK_BLE) */

    case ZMK_TRANSPORT_NONE: return 0;
    }

    LOG_ERR("Unsupported endpoint transport %d", current_instance.transport);
    return -ENOTSUP;
}
#endif // IS_ENABLED(CONFIG_ZMK_HID_IO_VOLUME_KNOB)
