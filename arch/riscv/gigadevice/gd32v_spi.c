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
 * ARE DISCLAIMED.  IN NO SPI SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include <dev/spi/spi.h>

#include "gd32v_spi.h"

#define	SPI_DEBUG
#undef	SPI_DEBUG

#ifdef	SPI_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val
#define	RD2(_sc, _reg)		\
	*(volatile uint16_t *)((_sc)->base + _reg)
#define	WR2(_sc, _reg, _val)	\
	*(volatile uint16_t *)((_sc)->base + _reg) = _val

void
gd32v_spi_intr(void *arg)
{

}

static void
gd32v_spi_setup(struct gd32v_spi_softc *sc)
{
	uint32_t reg;

	/* TODO */

	reg = CTL0_MSTMOD | CTL0_SWNSSEN | CTL0_PSC_8;
	reg = CTL0_MSTMOD | CTL0_PSC_8;
	WR4(sc, SPI_CTL0, reg);

	reg |= CTL0_SPIEN;
	WR4(sc, SPI_CTL0, reg);
}

static int
gd32v_spi_transfer(mdx_device_t dev, uint8_t *out, uint8_t *in, uint32_t len)
{
	struct gd32v_spi_softc *sc;
	int i;

	sc = mdx_device_get_softc(dev);

	for (i = 0; i < len; i++) {
		dprintf("stat %x\n", RD4(sc, SPI_STAT));
		while ((RD4(sc, SPI_STAT) & STAT_TBE) == 0);
		WR4(sc, SPI_DATA, out[i]);
		while (RD4(sc, SPI_STAT) & STAT_TRANS);
	}

	return (0);
}

static struct mdx_spi_ops gd32v_spi_ops = {
	.xfer = gd32v_spi_transfer,
};

void
gd32v_spi_init(mdx_device_t dev, uint32_t base)
{
	struct gd32v_spi_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(struct gd32v_spi_softc));
	sc->base = base;

	dev->ops = &gd32v_spi_ops;

	gd32v_spi_setup(sc);
}
