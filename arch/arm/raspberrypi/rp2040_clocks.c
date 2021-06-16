/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/callout.h>

#include <arm/raspberrypi/rp2040_clocks.h>

#define	RP2040_CLOCKS_DEBUG
#undef	RP2040_CLOCKS_DEBUG

#ifdef	RP2040_CLOCKS_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
rp2040_clocks_init(struct rp2040_clocks_softc *sc,
    uint32_t base)
{

	sc->base = base;

	WR4(sc, RP2040_CLOCKS_CLK_SYS_RESUS_CTRL, 0);
	WR4(sc, RP2040_CLOCKS_CLK_REF_CTRL,
	    CLOCKS_CLK_REF_CTRL_SRC_XOSC_CLKSRC);
	WR4(sc, RP2040_CLOCKS_CLK_SYS_CTRL, 0);

	WR4(sc,	RP2040_CLOCKS_CLK_PERI_CTRL,
	    CLOCKS_CLK_PERI_CTRL_ENABLE | CLOCKS_CLK_PERI_CTRL_XOSC_CLKSRC);
}