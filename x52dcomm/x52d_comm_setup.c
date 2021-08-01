/*
 * X52 Daemon communication library - Common setup routines
 *
 * Copyright (C) 2021 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#include "config.h"
#include <string.h>

#include "x52d_comm_setup.h"
#include "x52d_comm_const.h"

#include "comp_assert.h"

COMP_ASSERT_MSG(sizeof(X52D_COMM_SOCKET_NAME) <= sizeof(((struct sockaddr_un *)0)->sun_path),
                default_socket_path_exceeds_sun_path_length)

int x52d_comm_setup_socket(x52d_comm_ctx *ctx)
{
    if (ctx == NULL) {
        return X52D_COMM_ERROR_INVALID;
    }

    memset(&ctx->sock, 0, sizeof(ctx->sock));
    ctx->sock.sun_family = AF_UNIX;
    strncpy(ctx->sock.sun_path, X52D_COMM_SOCKET_NAME,
            sizeof(ctx->sock.sun_path) - 1);

    return X52D_COMM_SUCCESS;
}
