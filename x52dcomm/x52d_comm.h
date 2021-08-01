/*
 * X52 Daemon communication library - Common setup routines
 *
 * Copyright (C) 2021 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

/**
 * @file x52d_comm.h
 * @brief Functions, structures and enumerations for the X52 Daemon
 * communications library.
 *
 * This file contains the type, enum and function prototypes for the X52 daemon
 * communications library. These functions allow a client application to connect
 * to the running X52 daemon and control it using standard interfaces.
 *
 * @author Nirenjan Krishnan (nirenjan@nirenjan.org)
 */
#ifndef X52D_COMM_H
#define X52D_COMM_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup x52dcomm Communication library APIs
 *
 * These functions allow an application to communicate with and control the
 * running X52 daemon on the same host.
 *
 * @{
 */

/**
 * @brief Opaque structure used by x52d_comm
 */
struct x52d_comm_ctx;

/**
 * @brief Communications context structure used by x52d_comm
 *
 * All x52d_comm API functions require the application to pass in a pointer
 * to a valid communications context. A pointer can be obtained by calling
 * \ref x52d_comm_init
 */
typedef struct x52d_comm_ctx x52d_comm_ctx;

/**
 * @brief x52d_comm error codes
 *
 * Error codes returned by x52d_comm
 */
typedef enum {
    /** No error, indicates success */
    X52D_COMM_SUCCESS,

    /** Out of memory */
    X52D_COMM_ERROR_NO_MEMORY,

    /** Invalid parameters and/or pointers */
    X52D_COMM_ERROR_INVALID,

    /** Daemon not found */
    X52D_COMM_ERROR_NOT_FOUND,

    /** Connection not opened */
    X52D_COMM_ERROR_BAD_CONN,

    /** Invalid parameters for request */
    X52D_COMM_ERROR_BAD_REQUEST,

    /** No data in channel */
    X52D_COMM_ERROR_NO_DATA,

    /** Invalid data format in response */
    X52D_COMM_ERROR_BAD_FORMAT,

} x52d_comm_error_code;

/**
 * @brief List of supported communication request types
 */
typedef enum {
    /** Read some data from the X52 daemon */
    X52D_COMM_READ,

    /** Update some data in the X52 daemon */
    X52D_COMM_UPDATE,

    /** Create something in the X52 daemon */
    X52D_COMM_CREATE,

    /** Delete something in the X52 daemon */
    X52D_COMM_DELETE,
} x52d_comm_req_type;

/**
 * @brief Initialize the communications library
 *
 * This function intializes the x52d_comm library, sets up any internal data
 * structures to access the daemon, and returns a \ref x52d_comm_ctx pointer
 * in the output parameter. All calls to x52d_comm use the returned pointer to
 * control and communicate with the X52 daemon.
 *
 * @par Example
 * @code
 * int rc;
 * x52d_comm_ctx *ctx;
 * rc = x52d_comm_init(&ctx);
 * if (rc != X52D_COMM_SUCCESS) {
 *     // Handle error
 * }
 * // Save context for use later
 * @endcode
 *
 * @param[out]  ctx     Pointer to a \ref x52d_comm_ctx *. This function will
 * allocate a context and return the pointer to the context in this variable.
 *
 * @returns \ref x52d_comm_error_code indicating status
 */
int x52d_comm_init(x52d_comm_ctx **ctx);

/**
 * @brief Terminate the communications library
 *
 * This function releases any resources allocated by \ref x52d_comm_init and
 * terminates the library. Using the freed device now is invalid and can cause
 * errors.
 *
 * @param[in]   ctx     Pointer to the context
 *
 * @returns None
 */
void x52d_comm_exit(x52d_comm_ctx *ctx);

/**
 * @brief Connect to the X52 daemon
 *
 * This function attempts to connect to the running X52 daemon. If the daemon
 * is not running, or not listening for connections from clients, it will return
 * \ref X52D_COMM_ERROR_NOT_FOUND.
 *
 * @param[in]   ctx     Pointer to the context
 *
 * @returns \ref x52d_comm_error_code indicating status
 */
int x52d_comm_connect(x52d_comm_ctx *ctx);

/**
 * @brief Disconnect any open connections
 *
 * This function disconnects from the daemon. If already disconnected, it will
 * not do anything and return X52D_COMM_SUCCESS.
 *
 * @param[in]   ctx     Pointer to the context
 *
 * @returns \ref x52d_comm_error_code indicating status
 */
int x52d_comm_disconnect(x52d_comm_ctx *ctx);

/**
 * @brief Send a request to the daemon
 *
 * This function sends a request message to the X52 daemon. It will take care
 * of marshaling the request and sending it to the daemon.
 *
 * @param[in]   ctx     Pointer to the context
 * @param[in]   type    Type of the request, see \ref x52d_comm_req_type
 * @param[in]   req_id  Request identifier
 * @param[in]   sub_id  Request sub-identifier
 * @param[in]   ...     Request specific arguments. See protocol
 *
 * @returns \ref x52d_comm_error_code indicating status
 */
int x52d_comm_send(x52d_comm_ctx *ctx, x52d_comm_req_type type,
                   int req_id, int sub_id, ...);

/**
 * @brief Receive a response for the last sent request
 *
 * This function receives the response from the daemon for the previous
 * call to \ref x52d_comm_send. It will unmarshal the response and save
 * any response data based on the original request.
 *
 * @param[in]   ctx     Pointer to the context
 * @param[out]  ...     Request specific pointer arguments. See protocol
 *
 * @returns \ref x52d_comm_error_code indicating status
 */
int x52d_comm_recv(x52d_comm_ctx *ctx, ...);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // !defined X52D_COMM_H
