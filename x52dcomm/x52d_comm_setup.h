/*
 * X52 Daemon communication library - Common setup routines
 *
 * Copyright (C) 2021 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#ifndef X52D_COMMS_SETUP_H
#define X52D_COMMS_SETUP_H

#include "config.h"
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "x52d_comm.h"

/* Maximum message size */
#define MAX_MSG_SZ  1024

/* Maximum connections */
#define MAX_CONN    32

/* Context structure for communications */
struct x52d_comm_ctx {
    /* Socket file descriptor for client/server */
    int sock_fd;

    /* Socket descriptor */
    struct sockaddr_un sock;

    /* Message buffer */
    uint8_t msg[MAX_MSG_SZ];

    /* Message length */
    size_t  msg_len;

/* Server specific arguments */
#ifdef  X52D_COMM_SERVER

#endif
};

int x52d_comm_setup_socket(x52d_comm_ctx *ctx);

int x52d_comm_listen(x52d_comm_ctx *ctx);

#endif // !defined X52D_COMMS_SETUP_H
