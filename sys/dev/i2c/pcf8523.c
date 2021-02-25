/*	$OpenBSD: pcf8523.c,v 1.4 2020/04/27 12:36:03 kettenis Exp $	*/

/*
 * Copyright (c) 2005 Kimihiro Nonaka
 * Copyright (c) 2016 Mark Kettenis
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
 * THIS SOFTWARE IS PROVIDED BY WASABI SYSTEMS, INC. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL WASABI SYSTEMS, INC
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <sys/kernel.h>
#include <sys/fcntl.h>
#include <sys/uio.h>
#include <sys/conf.h>
#include <sys/event.h>

#include <dev/clock_subr.h>

#include <dev/i2c/i2cvar.h>

struct pcf8523rtc_softc {
	struct device sc_dev;
	i2c_tag_t sc_tag;
	int sc_address;
	struct todr_chip_handle sc_todr;
};

int pcf8523rtc_match(struct device *, void *, void *);
void pcf8523rtc_attach(struct device *, struct device *, void *);

struct cfattach pcf8523rtc_ca = {
	sizeof(struct pcf8523rtc_softc), pcf8523rtc_match, pcf8523rtc_attach
};

struct cfdriver pcf8523rtc_cd = {
	NULL, "pcf8523rtc", DV_DULL
};

uint8_t pcf8523rtc_reg_read(struct pcf8523rtc_softc *, int);
void pcf8523rtc_reg_write(struct pcf8523rtc_softc *, int, uint8_t);
int pcf8523rtc_clock_read(struct pcf8523rtc_softc *, struct clock_ymdhms *);
int pcf8523rtc_clock_write(struct pcf8523rtc_softc *, struct clock_ymdhms *);
int pcf8523rtc_gettime(struct todr_chip_handle *, struct timeval *);
int pcf8523rtc_settime(struct todr_chip_handle *, struct timeval *);
int pcf8523rtc_getcal(struct todr_chip_handle *, int *);
int pcf8523rtc_setcal(struct todr_chip_handle *, int);

int
pcf8523rtc_match(struct device *parent, void *v, void *arg)
{
	struct i2c_attach_args *ia = arg;

	if (strcmp(ia->ia_name, "nxp,pcf8523") == 0 &&
	    ia->ia_addr == PCF8523_ADDR)
		return (1);

	return (0);
}

void
pcf8523rtc_attach(struct device *parent, struct device *self, void *arg)
{
	struct pcf8523rtc_softc *sc = (struct pcf8523rtc_softc *)self;
	struct i2c_attach_args *ia = arg;
	uint8_t reg;

	sc->sc_tag = ia->ia_tag;
	sc->sc_address = ia->ia_addr;
	sc->sc_todr.cookie = sc;
	sc->sc_todr.todr_gettime = pcf8523rtc_gettime;
	sc->sc_todr.todr_settime = pcf8523rtc_settime;
	sc->sc_todr.todr_getcal = pcf8523rtc_getcal;
	sc->sc_todr.todr_setcal = pcf8523rtc_setcal;
	sc->sc_todr.todr_setwen = NULL;

#if 0
	todr_attach(&sc->sc_todr);
#else
	/* XXX */
	{
	extern todr_chip_handle_t todr_handle;
	todr_handle = &sc->sc_todr;
	}
#endif

	/*
	 * Enable battery switch-over and battery low detection in
	 * standard mode, and switch to 24 hour mode.
	 */
	reg = pcf8523rtc_reg_read(sc, PCF8523_CONTROL3);
	reg &= ~PCF8523_CONTROL3_PM_MASK;
	pcf8523rtc_reg_write(sc, PCF8523_CONTROL3, reg);
	reg = pcf8523rtc_reg_read(sc, PCF8523_CONTROL1);
	reg &= ~PCF8523_CONTROL1_12_24;
	reg &= ~PCF8523_CONTROL1_STOP;
	pcf8523rtc_reg_write(sc, PCF8523_CONTROL1, reg);

	/* Report battery status. */
	reg = pcf8523rtc_reg_read(sc, PCF8523_CONTROL3);
	printf(": battery %s\n", (reg & PCF8523_CONTROL3_BLF) ? "low" : "ok");
}

int
pcf8523rtc_gettime(struct todr_chip_handle *ch, struct timeval *tv)
{
	struct pcf8523rtc_softc *sc = ch->cookie;
	struct clock_ymdhms dt;

	memset(&dt, 0, sizeof(dt));
	if (pcf8523rtc_clock_read(sc, &dt) == 0)
		return (-1);

	tv->tv_sec = clock_ymdhms_to_secs(&dt);
	tv->tv_usec = 0;
	return (0);
}

int
pcf8523rtc_settime(struct todr_chip_handle *ch, struct timeval *tv)
{
	struct pcf8523rtc_softc *sc = ch->cookie;
	struct clock_ymdhms dt;
	uint8_t reg;

	clock_secs_to_ymdhms(tv->tv_sec, &dt);
	if (pcf8523rtc_clock_write(sc, &dt) == 0)
		return (-1);

	/* Clear OS flag.  */
	reg = pcf8523rtc_reg_read(sc, PCF8523_SECONDS);
	if (reg & PCF8523_SECONDS_OS) {
		reg &= ~PCF8523_SECONDS_OS;
		pcf8523rtc_reg_write(sc, PCF8523_SECONDS, reg);
	}

	return (0);
}

int
pcf8523rtc_setcal(struct todr_chip_handle *ch, int cal)
{
	return (EOPNOTSUPP);
}

int
pcf8523rtc_getcal(struct todr_chip_handle *ch, int *cal)
{
	return (EOPNOTSUPP);
}

uint8_t
pcf8523rtc_reg_read(struct pcf8523rtc_softc *sc, int reg)
{
	uint8_t cmd = reg;
	uint8_t val;

	iic_acquire_bus(sc->sc_tag, I2C_F_POLL);
	if (iic_exec(sc->sc_tag, I2C_OP_WRITE_WITH_STOP, sc->sc_address,
	    NULL, 0, &cmd, sizeof cmd, I2C_F_POLL) ||
	    iic_exec(sc->sc_tag, I2C_OP_READ_WITH_STOP, sc->sc_address,
	    NULL, 0, &val, sizeof val, I2C_F_POLL)) {
		iic_release_bus(sc->sc_tag, I2C_F_POLL);
		printf("%s: pcf8523rtc_reg_read: failed to read reg%d\n",
		    sc->sc_dev.dv_xname, reg);
		return 0;
	}
	iic_release_bus(sc->sc_tag, I2C_F_POLL);
	return val;
}

void
pcf8523rtc_reg_write(struct pcf8523rtc_softc *sc, int reg, uint8_t val)
{
	uint8_t cmd = reg;

	iic_acquire_bus(sc->sc_tag, I2C_F_POLL);
	if (iic_exec(sc->sc_tag, I2C_OP_WRITE_WITH_STOP, sc->sc_address,
	    &cmd, sizeof cmd, &val, sizeof val, I2C_F_POLL)) {
		iic_release_bus(sc->sc_tag, I2C_F_POLL);
		printf("%s: pcf8523rtc_reg_write: failed to write reg%d\n",
		    sc->sc_dev.dv_xname, reg);
		return;
	}
	iic_release_bus(sc->sc_tag, I2C_F_POLL);
}

int
pcf8523rtc_clock_read(struct pcf8523rtc_softc *sc, struct clock_ymdhms *dt)
{
	uint8_t regs[PCF8523_NRTC_REGS];
	uint8_t cmd = PCF8523_SECONDS;

	iic_acquire_bus(sc->sc_tag, I2C_F_POLL);
	if (iic_exec(sc->sc_tag, I2C_OP_WRITE_WITH_STOP, sc->sc_address,
	    NULL, 0, &cmd, sizeof cmd, I2C_F_POLL) ||
	    iic_exec(sc->sc_tag, I2C_OP_READ_WITH_STOP, sc->sc_address,
	    NULL, 0, regs, PCF8523_NRTC_REGS, I2C_F_POLL)) {
		iic_release_bus(sc->sc_tag, I2C_F_POLL);
		printf("%s: pcf8523rtc_clock_read: failed to read rtc\n",
		    sc->sc_dev.dv_xname);
		return (0);
	}
	iic_release_bus(sc->sc_tag, I2C_F_POLL);

	/*
	 * Convert the PCF8523's register values into something useable
	 */
	dt->dt_sec = FROMBCD(regs[0] & PCF8523_SECONDS_MASK);
	dt->dt_min = FROMBCD(regs[1] & PCF8523_MINUTES_MASK);
	dt->dt_hour = FROMBCD(regs[2] & PCF8523_HOURS_24MASK);
	dt->dt_day = FROMBCD(regs[3] & PCF8523_DAY_MASK);
	dt->dt_mon = FROMBCD(regs[5] & PCF8523_MONTH_MASK);
	dt->dt_year = FROMBCD(regs[6]) + 2000;

	if ((regs[0] & PCF8523_SECONDS_OS))
		return (0);

	return (1);
}

int
pcf8523rtc_clock_write(struct pcf8523rtc_softc *sc, struct clock_ymdhms *dt)
{
	uint8_t regs[PCF8523_NRTC_REGS];
	uint8_t cmd = PCF8523_SECONDS;

	/*
	 * Convert our time representation into something the PCF8523
	 * can understand.
	 */
	regs[0] = TOBCD(dt->dt_sec);
	regs[1] = TOBCD(dt->dt_min);
	regs[2] = TOBCD(dt->dt_hour);
	regs[3] = TOBCD(dt->dt_day);
	regs[4] = TOBCD(dt->dt_wday);
	regs[5] = TOBCD(dt->dt_mon);
	regs[6] = TOBCD(dt->dt_year - 2000);

	iic_acquire_bus(sc->sc_tag, I2C_F_POLL);
	if (iic_exec(sc->sc_tag, I2C_OP_WRITE_WITH_STOP, sc->sc_address,
	    &cmd, sizeof cmd, regs, PCF8523_NRTC_REGS, I2C_F_POLL)) {
		iic_release_bus(sc->sc_tag, I2C_F_POLL);
		printf("%s: pcf8523rtc_clock_write: failed to write rtc\n",
		    sc->sc_dev.dv_xname);
		return (0);
	}
	iic_release_bus(sc->sc_tag, I2C_F_POLL);
	return (1);
}
