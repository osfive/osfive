/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef	_SYS_TIME_H_
#define	_SYS_TIME_H_

#include <sys/types.h>

struct timespec {
	time_t	tv_sec;		/* Seconds. */
	long	tv_nsec;	/* Nanoseconds. */
};

struct timeval {
	time_t	tv_sec;		/* Seconds. */
	long	tv_usec;	/* Microseconds. */
};

#define	CLOCK_REALTIME	0

int mdx_clock_gettime(clockid_t clk_id, struct timespec *tp);
int mdx_clock_settime(clockid_t clk_id, const struct timespec *tp);
int mdx_clock_register(clockid_t clk_id, void *drv, void *arg);

struct rtc_driver {
	int (*gettime)(struct timespec *tp, void *arg);
	int (*settime)(const struct timespec *tp, void *arg);
};

#endif /* !_SYS_TIME_H_ */
