/*
Used to provide a clock interface with RTC 8523
*/

#include <stdio.h>
#include <dev/clock_subr.h>
#include <dev/i2c/i2cvar.h>
#include <dev/i2c/pcf8523reg.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <sys/kernel.h>

struct pcfrtc_softc {
	device_t sc_dev;
	i2c_tag_t sc_tag;
	int sc_address;
	int sc_open;
	struct todr_chip_handle sc_todr;
};

static int pcf8523_match(device_t, cfdata_t, void *);
static void pcf8523_attach(device_t, device_t, void *);
//static int pcf8523_clock_read(struct pcf8563rtc_softc *, struct clock_ymdhms *);
//static int pcf8523_clock_write(struct pcf8563rtc_softc *, struct clock_ymdhms *);
//static int pcf8523_gettime(struct todr_chip_handle *, struct clock_ymdhms *);
//static int pcf8523_settime(struct todr_chip_handle *, struct clock_ymdhms *);

void pcf8523_init(){
  return;
}

void pcf8523_attach(device_t parent, device_t self, void *aux){
  struct pcfrtc_softc *sc = device_private(self);
  struct i2c_attach_args *ia = aux;
  u_int8_t cmdbuf[1], csr;

  sc->sc_tag = ia->ia_tag;
  sc->sc_address = ia->ia_addr;
  sc->sc_dev = self;

  aprint_naive(": Real-time Clock\n");
  aprint_normal(": NXP PCF8523 Real-time Clock\n");

  cmdbuf[0] =n PCF8523_REG_CSR;
  if(iic_exec(sc->sc_tag, I2C_OP_READ_WITH_STOP, sc->sc_address, cmdbuf, 1 &csr, 1,0) != 0){
    aprint_error_dev(self, "unable to read CSR\n");
    return;
  }
  sc->sc_open = 0;
  sc->sc_todr.cookie = sc;
  sc->sc_todr.todr_gettime = pcfrtc_gettime;
  sc->sc_todr.todr_settime = pcfrtc_settime;
  sc->sc_todr.todr_setwen = NULL;

  todr_attach(&sc->sc_todr);
}

static int
pcf8523rtc_match(device_t parent, cfdata_t cf, void *aux)
{
	struct i2c_attach_args *ia = aux;
	int match_result;

	if (iic_use_direct_match(ia, cf, compat_data, &match_result))
		return match_result;

	/* indirect config - check typical address */
	if (ia->ia_addr == PCF8523_ADDR)
		return I2C_MATCH_ADDRESS_ONLY;

	return 0;
}
