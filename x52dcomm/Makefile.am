# Automake for libx52dcomm
#
# Copyright (C) 2021 Nirenjan Krishnan (nirenjan@nirenjan.org)
#
# SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0

noinst_LTLIBRARIES += libx52d_comm_common.la # x52d_comm_server.la x52d_comm_client.la

#lib_LTLIBRARIES = libx52dcomm.la

# X52d communications library - common routines
# This library provides common routines for both the client and daemon to
# communicate with each other

libx52d_comm_common_la_SOURCES = x52dcomm/x52d_comm_setup.c

libx52d_comm_common_la_CFLAGS = \
	-I $(top_srcdir) \
	-I $(top_srcdir)/lib/pinelog \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-DLOCALEDIR=\"$(localedir)\" \
	-DLOGDIR=\"$(localstatedir)/log\" \
	-DRUNDIR=\"$(runstatedir)\" \
	$(WARN_CFLAGS)

libx52d_comm_common_la_LDFLAGS = $(WARN_LDFLAGS)

EXTRA_DIST += \
	x52dcomm/x52d_comm.h \
	x52dcomm/x52d_comm_const.h \
	x52dcomm/x52d_comm_setup.h \
	x52dcomm/README.md
