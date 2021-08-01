/**
 * Constant expression checks at compile time
 *
 * Borrowed from hints on StackOverflow:
 * https://stackoverflow.com/a/807586
 *
 * Copyright (C) 2021 Nirenjan Krishnan
 */

#ifndef _COMP_ASSERT_H
#define _COMP_ASSERT_H

// Combine arguments
#define COMBINE(a, b) _COMBINE(a, b)
#define _COMBINE(a, b) a ## b

#define COMP_ASSERT_MSG(expr, msg) CASSERT(expr, msg)
#define COMP_ASSERT(expr) CASSERT(expr, __LINE__)

#define CASSERT(expr, msg) typedef char COMBINE(_assert_, msg)[2*!!(expr) - 1];

#endif // !defined _COMP_ASSERT_H
