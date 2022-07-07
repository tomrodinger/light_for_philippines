#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "MyInclude.h"

extern volatile uint8_t RecentChange;
extern uint8_t Charging;		// 0=Off, 1=On
extern uint8_t LampStatus;		// 0=Off, 1=Dim, 2=OnFull
extern uint8_t duty_Charger;
extern uint8_t duty_Lamp;
extern AdcStatus_t AdcStatus;

#include "HW_quick.h"

void CS_prepare(void) {
	uint8_t ReadVal;
	*(uint8_t*)0x618 = 0xC0;
	ReadVal = *(uint8_t*)0x618;
	ReadVal |= 0x28;
	*(uint8_t*)0x618 = ReadVal;
	*(uint8_t*)0x61E = 0x00;
	*(uint8_t*)0x62A = 0x01;
	*(uint8_t*)0x626 = 0x00;
	*(uint8_t*)0x61A = 0x97;
	*(uint8_t*)0x61B = 0x29;
	*(uint8_t*)0x61C = 0x3F;
	ADC0.CTRLB = ADC_SAMPNUM_ACC16_gc;		// Take 16 samples
	*(uint8_t*)0x619 = 0x00;
	ADC0.CTRLC =	ADC_REFSEL_VDDREF_gc |	// Use Vdd as voltage reference
					ADC_PRESC_DIV8_gc;		// Divide clock by 8
	ReadVal = *(uint8_t*)0x618;
	ReadVal |= 0x03;
	*(uint8_t*)0x618 = ReadVal;
	ADC0.CTRLA =	ADC_RUNSTBY_bm |		// Run ADC in Standby
					ADC_ENABLE_bm;			// Enable ADC
}

void ADC_prepare(void) {
	ADC0.CTRLB = ADC_SAMPNUM_ACC4_gc;		// Take 4 samples
	ADC0.CTRLC =	ADC_REFSEL_INTREF_gc |	// Use internal reference
					ADC_PRESC_DIV8_gc;		// Divide clock by 8
	*(uint8_t*)0x618 = 0;			// Turn off CS
	*(uint8_t*)0x61A = 0;
	*(uint8_t*)0x61B = 0;
	*(uint8_t*)0x61C = 0;
	*(uint8_t*)0x62A = 0;
}

void LampOn(uint8_t brightness) {
	duty_Lamp = brightness;
	TCD0.CMPACLRL = brightness;
	if ( Charging ) {			// PWMs are already running
		while ( (TCD0.STATUS & TCD_CMDRDY_bm) == 0 ) ;
		TCD0.CTRLE = TCD_SYNCEOC_bm;		// Sync register values to PWMs
	} else {					// PWMs not running
		TCD0.CMPBSETL = 0xFF;
		while ( (TCD0.STATUS & TCD_ENRDY_bm) == 0 ) ;
		TCD0.CTRLA = TCD_ENABLE_bm;			// Turn on PWMs
	}
}
	
void LampOff(void) {
	if ( Charging ) {
		TCD0.CMPACLRL = 0;
		while ( (TCD0.STATUS & TCD_CMDRDY_bm) == 0 ) ;
		TCD0.CTRLE = TCD_SYNCEOC_bm;		// Sync register values to PWMs
	} else {
		while ( (TCD0.STATUS & TCD_ENRDY_bm) == 0 ) ;
		TCD0.CTRLE = TCD_DISEOC_bm;			// Turn off PWM at end of cycle
	}
}

void ChargerOn(void) {						// Pin 6, PA5, TCD WOB
	duty_Charger = dutyChargerStart;
	TCD0.CMPBSETL = 0xFF - duty_Charger;
	if ( LampStatus == 0 ) {
		TCD0.CMPBCLRL = 0xFF;
		TCD0.CMPACLRL = 0x00;
		while ( (TCD0.STATUS & TCD_ENRDY_bm) == 0 ) ;
		TCD0.CTRLA = TCD_ENABLE_bm;			// Turn on PWMs
	} else {	// LampStatus > 0
		while ( (TCD0.STATUS & TCD_CMDRDY_bm) == 0 ) ;
		TCD0.CTRLE = TCD_SYNCEOC_bm;		// Sync register values to PWMs
	}
}

void ChargerOff(void) {
	if ( LampStatus == 0 ) {	// Can turn everything off
		while ( (TCD0.STATUS & TCD_ENRDY_bm) == 0 ) ;
		TCD0.CTRLE = TCD_DISEOC_bm;			// Turn off PWM at end of cycle
	} else {		// Lamp must stay on
		TCD0.CMPBSETL = 0xFF;
		while ( (TCD0.STATUS & TCD_CMDRDY_bm) == 0 ) ;
		TCD0.CTRLE = TCD_SYNCEOC_bm;		// Sync register values to PWMs
	}
}

void SetPowerDownTime(uint16_t TimeOut) {
	while (RTC.STATUS > 0) ;	
	RTC.CNT = 0;
	RTC.PER = TimeOut;
	while (RTC.STATUS > 0) ;		// Make sure parameters have updated before enabling Interrupt
	RTC.INTFLAGS = 	RTC_OVF_bm;		// Get rid of any pending interrupts		
	RTC.INTCTRL = RTC_OVF_bm;		// Enable interrupts
}

void SetPowerDownOff(void) {
	RTC.INTCTRL = 0;				// Turn off interrupts
	RTC.INTFLAGS = RTC_OVF_bm;		// Get rid of any pending interrupts
}

void Enter_PD(void) {
	LampOff();
	LampStatus = 0;
	ChargerOff();
	Charging = 0;
	SetVR3(1);
	LED_Red(0);
	LED_Green(0);
	while (RTC.STATUS > 0) ;
	RTC.CTRLA = 0;
	RTC.INTCTRL = 0;
	while (RTC.STATUS > 0) ;
	while (RTC.PITSTATUS > 0) ;
	RTC.PITCTRLA = 0; 
	RTC.PITINTCTRL = 0;
	while (RTC.PITSTATUS > 0) ;
	ADC0.CTRLA = 0;
	ADC0.INTCTRL = 0;
	while ( TCD0.CTRLA & TCD_ENABLE_bm ) ;
	TCD0.CMPBCLRL = 0;
	_PROTECTED_WRITE(TCD0.FAULTCTRL, 0 );
	SLPCTRL.CTRLA = SLEEP_MODE_PWR_DOWN |
					SLPCTRL_SEN_bm;				// Enable sleep
	sei();
	sleep_cpu();
	hw_init();
	SetVR3(0);
	SetPowerDownTime(PowerDownTimeShort);
	RecentChange = 5;
}

#ifdef Use_USART
void USART_0_write(const uint8_t data) {
	while (!(USART0.STATUS & USART_DREIF_bm)) ;
	USART0.TXDATAL = data;
}

void Print8bit( uint8_t data, uint8_t eol ) {
	char str[6];
	itoa(data, str, 10);
	for (uint8_t ind = 0; (ind < 6) && (str[ind]) ; ind++ ) {
		USART_0_write(str[ind]);
	}
	if ( eol ) {
		USART_0_write('\n');
		USART_0_write('\r');
	} else {
		USART_0_write(' ');
	}
}

void Print16bit( uint16_t data, uint8_t eol ) {
	static char str[8];
	itoa(data, str, 10);
	for (uint8_t ind = 0; (ind < 8) && (str[ind]) ; ind++ ) {
		USART_0_write(str[ind]);
	}
	if ( eol ) {
		USART_0_write('\n');
		USART_0_write('\r');
	} else {
		USART_0_write(' ');
	}
}

#endif // Use_USART
