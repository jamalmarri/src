// Definitions imported from OpenBSD

#ifndef _PCF8523REG_H
#define _PCF8523REG_H

/*
 * PCF8523 Real-Time Clock
*/

#define	PCF8523_ADDR		0x68	/* Fixed I2C Slave Address */

#define PCF8523_CONTROL1	0x00
#define PCF8523_CONTROL2	0x01
#define PCF8523_CONTROL3	0x02
#define PCF8523_SECONDS		0x03
#define PCF8523_MINUTES		0x04
#define PCF8523_HOURS		0x05
#define PCF8523_DAY		0x06
#define PCF8523_WDAY		0x07
#define PCF8523_MONTH		0x08
#define PCF8523_YEAR		0x09
#define PCF8523_ALARM_MIN	0x0a
#define PCF8523_ALARM_HOUR	0x0b
#define PCF8523_ALARM_DAY	0x0c
#define PCF8523_ALARM_WDAY	0x0d
#define PCF8523_OFFSET		0x0e

#define	PCF8523_NREGS		20
#define	PCF8523_NRTC_REGS	7

/*
 * Bit definitions.
 */
#define	PCF8523_CONTROL1_12_24	(1 << 3)
#define	PCF8523_CONTROL1_STOP	(1 << 5)
#define	PCF8523_CONTROL3_PM_MASK 0xe0
#define PCF8523_CONTROL3_PM_BLD	(1 << 7)
#define PCF8523_CONTROL3_PM_VDD	(1 << 6)
#define PCF8523_CONTROL3_PM_DSM	(1 << 5)
#define PCF8523_CONTROL3_BLF	(1 << 2)
#define	PCF8523_SECONDS_MASK	0x7f
#define	PCF8523_SECONDS_OS	(1 << 7)
#define	PCF8523_MINUTES_MASK	0x7f
#define	PCF8523_HOURS_12HRS_PM	(1 << 5)	/* If 12 hr mode, set = PM */
#define	PCF8523_HOURS_12MASK	0x1f
#define	PCF8523_HOURS_24MASK	0x3f
#define	PCF8523_DAY_MASK	0x3f
#define	PCF8523_WDAY_MASK	0x07
#define	PCF8523_MONTH_MASK	0x1f

// End Definitions imported from OpenBSD
