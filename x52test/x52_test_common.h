/*
 * Saitek X52 Pro MFD & LED driver
 *
 * Copyright (C) 2012-2015 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#ifndef X52_TEST_COMMON_H
#define X52_TEST_COMMON_H

#include "config.h"
#include <stdio.h>
#include <stdbool.h>
#include "libx52.h"
#include "gettext.h"

extern libx52_device *dev;
extern int test_exit;
extern bool nodelay;

#define TEST(tst, ...) do { \
    int rc; \
    rc = ( libx52_set_ ## tst (dev, __VA_ARGS__) ); \
    if (rc) { \
        fprintf(stderr, _("\n%s(%s) failed with %d(%s)\n"), #tst, #__VA_ARGS__, rc, libx52_strerror(rc)); \
        return rc; \
    } \
    if (test_exit) return test_exit; \
    rc  = libx52_update(dev); \
    if (rc) { \
        fprintf(stderr, _("\nupdate failed with %d(%s)\n"), rc, libx52_strerror(rc)); \
        return rc; \
    } \
    if (test_exit) return test_exit; \
} while (0)

/* For i18n */
#define _(x) gettext(x)

void test_cleanup(void);

void print_banner(const char *message);

int test_brightness(void);
int test_mfd_display(void);
int test_mfd_text(void);
int test_leds(void);
int test_blink_n_shift(void);
int test_clock(void);

#endif /* X52_TEST_COMMON_H */
