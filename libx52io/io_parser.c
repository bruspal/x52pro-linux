/*
 * Saitek X52 IO driver - report parser
 *
 * Copyright (C) 2012-2020 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#include <stdint.h>
#include "io_common.h"
#include "usb-ids.h"

static void map_hat(uint8_t hat, libx52io_report *report)
{
    /*
     * Hat reports values from 0-8, but just to account for any spurious
     * values, leave the remaining 7 entries blank.
     *
     * Pushing the hat North reports 1, and it increases to 2 for NE, 3 for
     * East, 4 for SE and so on in a clockwise fashion until it hits 8 for NW.
     *
     * According to the USB spec, Y axis increases as it is pulled from front
     * to back, i.e., further from the user to closer to the user, and X axis
     * increases left to right. Therefore NE is X=+1, Y=-1.
     */
    static const int32_t hat_to_axis[16][2] = {
        {0, 0},
        {0, -1},
        {1, -1},
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0},
        {-1, -1},
    };

    report->axis[LIBX52IO_AXIS_HATX] = hat_to_axis[hat][0];
    report->axis[LIBX52IO_AXIS_HATY] = hat_to_axis[hat][1];
}

static void map_axis(unsigned char *data, int thumb_pos, libx52io_report *report)
{
    /*
     * The bytes containing the throttle axes are the same, with only the
     * position of the thumbstick report varying between the X52 and X52Pro.
     * Therefore, we can share the code between the different parsers
     */
    report->axis[LIBX52IO_AXIS_Z] = data[4];
    report->axis[LIBX52IO_AXIS_RX] = data[5];
    report->axis[LIBX52IO_AXIS_RY] = data[6];
    report->axis[LIBX52IO_AXIS_SLIDER] = data[7];
    report->axis[LIBX52IO_AXIS_THUMBX] = data[thumb_pos] & 0xf;
    report->axis[LIBX52IO_AXIS_THUMBY] = data[thumb_pos] >> 4;

    /*
     * The hat report is in the upper 4 bits of the byte preceding the
     * thumbstick report. Use that to map to the axis values
     */
    report->hat = data[thumb_pos-1] >> 4;
    map_hat(report->hat, report);
}

static void map_buttons(unsigned char *data, const int *button_map, libx52io_report *report)
{
    /*
     * The bytes containing the buttons are the same between the X52 and X52Pro.
     * Therefore, we can share the code between the two parsers, and we just
     * need a different button map for each device.
     */
    uint64_t buttons = 0;
    int i;
    buttons |= data[12]; buttons <<= 8;
    buttons |= data[11]; buttons <<= 8;
    buttons |= data[10]; buttons <<= 8;
    buttons |= data[9]; buttons <<= 8;
    buttons |= data[8];

    for (i = 0; button_map[i] != -1; i++) {
        int btn = button_map[i];
        report->button[btn] = !!(buttons & ((uint64_t)1 << i));
    }

    if (report->button[LIBX52IO_BTN_MODE_1]) {
        report->mode = 1;
    } else if (report->button[LIBX52IO_BTN_MODE_2]) {
        report->mode = 2;
    } else if (report->button[LIBX52IO_BTN_MODE_3]) {
        report->mode = 3;
    }
    /*
     * NOTE: It is possible to hold the mode selector in a position such that
     * none of the mode buttons actually report as selected. It is also
     * possible that it could be in a transient state between two adjacent
     * modes. Either way, we don't want to modify the report, so leave it. It
     * is up to the application to handle the case where mode doesn't change.
     */
}

#define B(x) LIBX52IO_BTN_ ## x

static int parse_x52(unsigned char *data, int length, libx52io_report *report)
{
    /*
     * Report layout for X52
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |  X axis data        |  Y axis data        |  Rz axis data     |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |   Throttle    |  Rx axis data | Ry axis data  | Slider data   |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * | Buttons 7-0   | Buttons 15-8  | Buttons 23-16 | Buttons 31-24 |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |  Hat  |///|Btn| MouseY| MouseX|
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     */
    uint32_t axis;

    static const int button_map[] = {
        B(TRIGGER),
        B(FIRE),
        B(A),
        B(B),
        B(C),
        B(PINKY),
        B(D),
        B(E),
        B(T1_UP),
        B(T1_DN),
        B(T2_UP),
        B(T2_DN),
        B(T3_UP),
        B(T3_DN),
        B(TRIGGER_2),
        B(POV_1_N),
        B(POV_1_E),
        B(POV_1_S),
        B(POV_1_W),
        B(POV_2_N),
        B(POV_2_E),
        B(POV_2_S),
        B(POV_2_W),
        B(MODE_1),
        B(MODE_2),
        B(MODE_3),
        B(FUNCTION),
        B(START_STOP),
        B(RESET),
        B(CLUTCH),
        B(MOUSE_PRIMARY),
        B(MOUSE_SECONDARY),
        B(MOUSE_SCROLL_DN),
        B(MOUSE_SCROLL_UP),
        -1
    };

    if (length != 14) {
        return LIBX52IO_ERROR_IO;
    }

    axis = (data[3] << 24) |
           (data[2] << 16) |
           (data[1] <<  8) |
           data[0];

    report->axis[LIBX52IO_AXIS_X] = axis & 0x7ff;
    report->axis[LIBX52IO_AXIS_Y] = (axis >> 11) & 0x7ff;
    report->axis[LIBX52IO_AXIS_RZ] = (axis >> 22) & 0x3ff;
    map_axis(data, 13, report);

    map_buttons(data, button_map, report);

    return LIBX52IO_SUCCESS;
}

static int parse_x52pro(unsigned char *data, int length, libx52io_report *report)
{
    /*
     * Report layout for X52Pro
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |  X axis data      |  Y axis data      |///|  Rz axis data     |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |   Throttle    |  Rx axis data | Ry axis data  | Slider data   |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * | Buttons 7-0   | Buttons 15-8  | Buttons 23-16 | Buttons 31-24 |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |/| Btns 38-32  |  Hat  |///////| MouseY| MouseX|
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     */
    uint32_t axis;

    static const int button_map[] = {
        B(TRIGGER),
        B(FIRE),
        B(A),
        B(B),
        B(C),
        B(PINKY),
        B(D),
        B(E),
        B(T1_UP),
        B(T1_DN),
        B(T2_UP),
        B(T2_DN),
        B(T3_UP),
        B(T3_DN),
        B(TRIGGER_2),
        B(MOUSE_PRIMARY),
        B(MOUSE_SCROLL_DN),
        B(MOUSE_SCROLL_UP),
        B(MOUSE_SECONDARY),
        B(POV_1_N),
        B(POV_1_E),
        B(POV_1_S),
        B(POV_1_W),
        B(POV_2_N),
        B(POV_2_E),
        B(POV_2_S),
        B(POV_2_W),
        B(MODE_1),
        B(MODE_2),
        B(MODE_3),
        B(CLUTCH),
        B(FUNCTION),
        B(START_STOP),
        B(RESET),
        B(PG_UP),
        B(PG_DN),
        B(UP),
        B(DN),
        B(SELECT),
        -1
    };

    if (length != 15) {
        return LIBX52IO_ERROR_IO;
    }

    axis = (data[3] << 24) |
           (data[2] << 16) |
           (data[1] <<  8) |
           data[0];

    report->axis[LIBX52IO_AXIS_X] = axis & 0x3ff;
    report->axis[LIBX52IO_AXIS_Y] = (axis >> 10) & 0x3ff;
    report->axis[LIBX52IO_AXIS_RZ] = (axis >> 22) & 0x3ff;
    map_axis(data, 14, report);

    map_buttons(data, button_map, report);

    return LIBX52IO_SUCCESS;
}

void _x52io_set_report_parser(libx52io_context *ctx)
{
    switch (ctx->pid) {
    case X52_PROD_X52_1:
    case X52_PROD_X52_2:
        ctx->parser = parse_x52;
        break;

    case X52_PROD_X52PRO:
        ctx->parser = parse_x52pro;

    default:
        break;
    }
}

int _x52io_parse_report(libx52io_context *ctx, libx52io_report *report,
                        unsigned char *data, int length)
{
    if (ctx->parser == NULL) {
        return LIBX52IO_ERROR_NO_DEVICE;
    }

    return (ctx->parser)(data, length, report);
}

int libx52io_read(libx52io_context *ctx, libx52io_report *report)
{
    return libx52io_read_timeout(ctx, report, -1);
}

int libx52io_read_timeout(libx52io_context *ctx, libx52io_report *report, int timeout)
{
    int rc;
    unsigned char data[16];

    if (ctx == NULL || report == NULL) {
        return LIBX52IO_ERROR_INVALID;
    }

    if (ctx->handle == NULL) {
        return LIBX52IO_ERROR_NO_DEVICE;
    }

    rc = hid_read_timeout(ctx->handle, data, sizeof(data), timeout);
    if (rc == 0) {
        return LIBX52IO_ERROR_TIMEOUT;
    } else if (rc < 0) {
        return LIBX52IO_ERROR_IO;
    }

    // rc > 0
    return _x52io_parse_report(ctx, report, data, rc);
}
