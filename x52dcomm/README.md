X52 Daemon Communications library
=================================

This is a convenience library which allows the X52 daemon and client
applications to communicate with each other. The library provides standardized
APIs to wrap the calls to the socket communication.

# Supported functions

* `x52d_comm_init(libx52comm_ctx **ctx)`
* `x52d_comm_exit(libx52comm_ctx *ctx)`
* `x52d_comm_connect(libx52comm_ctx *ctx)`
* `x52d_comm_disconnect(libx52comm_ctx *ctx)`
* `x52d_comm_send(libx52comm_ctx *ctx, type, request_id, sub_id, ...)`
* `x52d_comm_recv(libx52comm_ctx *ctx, ...)`

# Protocol

The protocol uses a simple CRUD style syntax to implement requests to the daemon
and returns responses using a status code and a library identifier to identify
which of the individual libraries handled the request, and what the error was,
if any.

Requests are identified by a request class identifier, and a sub-identifier for
the individual method. Responses will indicate if the request was successful, or
if the corresponding library could not handle the request.

## Supported requests

As of this writing, the only supported requests are those supported by the
daemon configuration.

* MFD/LED update
    * LED state
    * LED brightness
    * MFD brightness
* Clock state
    * Enable - on/off
    * Primary clock timezone - local/UTC
    * Clock timezone
    * Clock format
    * Date format
* Profile configuration
    * Profile directory
    * Clutch state

# Wire format

libx52comm uses a binary protocol to implement a CRUD style interface. The
client initiates all requests, and the wire format is as follows:

       3                   2                   1
     1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |Version|0 0|Typ|   Message ID  |  Request ID   | Request SubID |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | Request specific data ...

Responses use a different wire format as follows:

       3                   2                   1
     1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |Version| LibID |   Stat Code   |   Message ID  |  Response data ...
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Version is 0000.

The message ID is a monotonically increasing counter that identifies the current
message being sent between client and server. If this is less than or equal to
the last message received, then it is ignored. Given that the interface runs on
Unix Domain sockets, the chances of a message getting dropped are next to nil.
This therefore serves as a rudimentary check on the communication channel.

The context maintains a longer version of the message ID internally - the
message ID in the message is simply the last 8 bits. This is similar to the OFB
cryptographic mode, where only a few bits of the message ID are sent in the
message.

Type is one of the following bit sequences:

* `00` - read
* `01` - update
* `10` - create
* `11` - delete

