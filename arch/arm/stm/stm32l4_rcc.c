/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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
#include <arm/stm/stm32l4_rcc.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
stm32l4_rcc_dsisel(struct stm32l4_rcc_softc *sc)
{
	uint32_t reg;

	reg = RD4(sc, RCC_CCIPR2);
	reg |= CCIPR2_DSISEL;
	WR4(sc, RCC_CCIPR2, reg);
}

void
stm32l4_rcc_hse_enable(struct stm32l4_rcc_softc *sc)
{
	uint32_t reg;

	/* Enable High-Speed External (HSE) ocsillator */
	reg = RD4(sc, RCC_CR);
	reg |= CR_HSEON;
	WR4(sc, RCC_CR, reg);

	while ((RD4(sc, RCC_CR) & CR_HSERDY) == 0)
		;
}

/* MSI (multispeed internal) RC oscillator clock configuration */
void
stm32l4_rcc_msi_configure(struct stm32l4_rcc_softc *sc,
    uint32_t freq)
{
	uint32_t reg;

	do {
		reg = RD4(sc, RCC_CR);
	} while ((reg & CR_MSIRDY) == 0);

	reg &= ~CR_MSIRANGE_M;

	switch (freq) {
	case 48000000:
		reg |= CR_MSIRANGE_48MHZ;
		break;
	case 24000000:
		reg |= CR_MSIRANGE_24MHZ;
		break;
	case 16000000:
		reg |= CR_MSIRANGE_16MHZ;
		break;
	case 8000000:
		reg |= CR_MSIRANGE_8MHZ;
		break;
	case 4000000:
		reg |= CR_MSIRANGE_4MHZ;
		break;
	case 2000000:
		reg |= CR_MSIRANGE_2MHZ;
		break;
	case 1000000:
		reg |= CR_MSIRANGE_1MHZ;
		break;
	case 800000:
		reg |= CR_MSIRANGE_800KHZ;
		break;
	case 400000:
		reg |= CR_MSIRANGE_400KHZ;
		break;
	case 200000:
		reg |= CR_MSIRANGE_200KHZ;
		break;
	case 100000:
		reg |= CR_MSIRANGE_100KHZ;
		break;
	default:
		panic("Unsupported range\n");
	}

	reg |= CR_MSIRGSEL | CR_MSION;
	WR4(sc, RCC_CR, reg);

	do {
		reg = RD4(sc, RCC_CR);
	} while ((reg & CR_MSIRDY) == 0);

	printf("msi %x\n", RD4(sc, RCC_CR));
}

int
stm32l4_rcc_lsi_enable(struct stm32l4_rcc_softc *sc)
{
	int timeout;
	int reg;

	reg = RD4(sc, RCC_CSR);
	reg |= CSR_LSION;
	WR4(sc, RCC_CSR, reg);

	timeout = 1000;	/* ~480 is required on stm32l431kc */
	do {
		reg = RD4(sc, RCC_CSR);
		if (reg & CSR_LSIRDY)
			break;
	} while (timeout--);

	if (timeout <= 0) {
		printf("Can't enable LSI clock\n");
		return (-1);
	}

	return (0);
}

void
stm32l4_rcc_pll_configure(struct stm32l4_rcc_softc *sc,
    int pllm, int plln, int pllq, int pllp, uint8_t external,
    uint32_t rcc_cfgr)
{

	/* TODO */
}

void
stm32l4_rcc_setup(struct stm32l4_rcc_softc *sc,
    struct rcc_config *cfg)
{

	WR4(sc, RCC_AHB1ENR, cfg->ahb1enr);
	WR4(sc, RCC_AHB2ENR, cfg->ahb2enr);
	WR4(sc, RCC_AHB3ENR, cfg->ahb3enr);
	WR4(sc, RCC_APB1ENR1, cfg->apb1enr1);
	WR4(sc, RCC_APB1ENR2, cfg->apb1enr2);
	WR4(sc, RCC_APB2ENR, cfg->apb2enr);
}

void
stm32l4_rcc_bdcr_setup(struct stm32l4_rcc_softc *sc,
    uint32_t bdcr)
{

	WR4(sc, RCC_BDCR, bdcr);
}

int
stm32l4_rcc_init(struct stm32l4_rcc_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
