#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>

#include "MyInclude.h"

extern AdcStatus_t AdcStatus;
extern uint8_t duty_Charger;

#include "HW_quick.h"

static void SetupPins(void) {
/*	1:	PA2:	Dg (Green Led):	Digital Out
	2:	PA3:	Dr (Red Led):	Digital Out
	3:	Gnd
	4:	Vdd
	5:	PA4:	CS:				Analog
	6:	PA5:	Vdc:			Digital Out	-- TCD WOB
	7:	PA6:	Vsoo:			Digital In
	8:	PA7:	Vss:			Analog In -- AIN7
	9:	PB5:	Vs1:			Analog In
	10:	PB4:	Vs2:			Analog In
	11:	PB3:	Rx:				Digital In: Pull-up enable
	12:	PB2:	Tx:				Digital Out: Pull-up enable
	13:	PB1:	Vs3:			Analog In
	14:	PB0:	Vsb:			Analog In
	15:	PC0:	Vr3:			Digital Out
	16:	PC1:	Vdl:			Digital Out -- TCD WOD
	17:	PC2:	None:			Pull-up enable
	18:	PC3:	None:			Pull-up enable
	19:	PA0:	UPDI:			Pull-up enable
	20:	PA1:	Vst:			Pull-up enable (to be changed later to digital output)
*/
	// Set outputs:
	PORTA.DIR = 0x2C;			// PA2, PA3, PA5
	PORTB.DIR = 0x04;			// PB2
	PORTC.DIR = 0x03;			// PC0, PC1
	// Set pull-ups, interrupts and disable inputs:
	PORTA.PIN0CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTA.PIN1CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTA.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;	
	PORTA.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTA.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTA.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTA.PIN6CTRL = PORT_ISC_BOTHEDGES_gc;		// Enable interrupt
	PORTA.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN3CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;	// Disable input buffer since we don't use Rx function
	PORTB.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTC.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTC.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTC.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTC.PIN3CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
}

void hw_init(void) {
	SetupPins();
	LED_Red(0);
	LED_Green(0);

	// Enable main clock divisor and select division by 8 => 2.5 MHz
	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, CLKCTRL_PDIV_8X_gc | CLKCTRL_PEN_bm);

	// Configure RTC:
	while ( RTC.STATUS & RTC_CTRLABUSY_bm )
		LED_Red(1);					// Wait for RTC write synchronization
	LED_Red(0);
	RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;			// Select clock of 1KHz
	RTC.CTRLA =		RTC_PRESCALER_DIV16384_gc |	// Divide by 16384 => Clock  ~ 1/16 Hz
					RTC_RTCEN_bm |				// Enable RTC
					RTC_RUNSTDBY_bm;			// Enable Run in Standby
	while (RTC.PITSTATUS > 0) ;					// Wait for PIT to be writeable
	RTC.PITCTRLA =	RTC_PERIOD_CYC256_gc |		// Periodic interrupts every 256 RTC cycles (~4Hz)
	RTC_PITEN_bm;				// Enable PIT
	RTC.PITINTCTRL = RTC_PITEN_bm;				// Enable PIT interrupts
	LED_Red(0);

	// Configure ADC and Capacitive Sense:
	ADC0.INTCTRL = ADC_RESRDY_bm;				// Enable ADC Interrupts
	ADC0.SAMPCTRL = 0x04;						// Increase sample time by 4 ADC clock cycles
	*(uint8_t*)0x622 = 0x01;
	VREF.CTRLA = VREF_ADC0REFSEL_1V1_gc;		// Set internal reference voltage to 1.1V

	// Configure TCD0:
	TCD0.CTRLC = 0;										// Output waveform A on output WOD
	_PROTECTED_WRITE(TCD0.FAULTCTRL, TCD_CMPDEN_bm |	// Enable WOD output
					//	TCD_CMPB_bm |
						TCD_CMPBEN_bm );				// Enable WOB output
	TCD0.CMPBCLRL = 0xFF;						// Period of 256
	TCD0.CMPASETL = 0;							// Start WOA at clock 0
	
#ifdef Use_USART
	// Configure USART:
	USART0.BAUD = (uint16_t)260;				// Baud ~ 38400
	USART0.CTRLB = USART_TXEN_bm;				// Enable Transmitter
#endif	

	sei();										// Enable Interrupts
}
