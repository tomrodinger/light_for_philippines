#ifndef HW_QUICK_H_
#define HW_QUICK_H_

#include <avr/sleep.h>

static inline void LED_Green(uint8_t level) {
	if ( level == 0 ) {
		PORTA.OUTSET = 0x04;
		} else if ( level == 1) {
		PORTA.OUTCLR = 0x04;
	}
}

static inline void LED_Red(uint8_t level) {
	if ( level == 0 ) {
		PORTA.OUTSET = 0x08;
		} else if ( level == 1) {
		PORTA.OUTCLR = 0x08;
	}
}

static inline void ToggleLED_Green(void) {
	PORTA.OUTTGL = 0x04;
}

static inline void ToggleLED_Red(void) {
	PORTA.OUTTGL = 0x08;
}

static inline void SetVR3(uint8_t level) {
	if ( level == 0 ) {
		PORTC.OUTCLR = 0x01;						// Pin PC0
	} else {
		PORTC.OUTSET = 0x01;
	}
}

static inline uint8_t GetVsOnOff(void) {
	if ( VPORTA.IN & 0x40 ) {								// VSoo on pin PA6
		return 1;
	} else {
		return 0;
	}
}

static inline void SetVst(uint8_t level) {			// Startup leak pin on PA1
	PORTA.DIRSET = 0x02;		// set pin to output
	if ( level ) {
		PORTA.OUTSET = 0x02;
	} else {
		PORTA.OUTCLR = 0x02;
	}
}

static inline void SetADC_VS1(void) {
	if ( AdcStatus == Measure_CS ) {	// Need to switch entire config over
		ADC_prepare();
	}
	ADC0.MUXPOS = ADC_MUXPOS_AIN8_gc;
}

static inline void SetADC_VS2(void) {				// Set ADC to VS2 and start conversion
	if ( AdcStatus == Measure_CS ) {	// Need to switch entire config over
		ADC_prepare();
	}
	ADC0.MUXPOS = ADC_MUXPOS_AIN9_gc;
}

static inline void SetADC_VSB(void) {
	if ( AdcStatus == Measure_CS ) {	// Need to switch entire config over
		ADC_prepare();
	}
	ADC0.MUXPOS = ADC_MUXPOS_AIN11_gc;
}

static inline void SetADC_VS3(void){
	if ( AdcStatus == Measure_CS ) {	// Need to switch entire config over
		ADC_prepare();
	}
	ADC0.MUXPOS = ADC_MUXPOS_AIN10_gc;
}

static inline void SetADC_VSS(void) {
	if ( AdcStatus == Measure_CS ) {	// Need to switch entire config over
		ADC_prepare();
	}
	ADC0.MUXPOS = ADC_MUXPOS_AIN7_gc;
}

static inline uint16_t ADC_read(void) {
	uint16_t result;
	result = ADC0.RES >> 2;
	ADC0.CTRLA = 0;
	return result;
}

static inline void ADC_start(void) {
	ADC0.CTRLA =	ADC_RUNSTBY_bm |		// Run ADC in Standby
					ADC_ENABLE_bm;			// Enable ADC
	ADC0.COMMAND = ADC_STCONV_bm;			// Start conversion
}

static inline void SetLamp(uint8_t brightness) {
	TCD0.CMPACLRL = brightness;
	while ( (TCD0.STATUS & TCD_CMDRDY_bm) == 0 ) ;
	TCD0.CTRLE = TCD_SYNCEOC_bm;		// Sync register values to PWMs
}

static inline void SetCharger(void) {
	TCD0.CMPBSETL = 0xFF - duty_Charger;
	while ( (TCD0.STATUS & TCD_CMDRDY_bm) == 0 ) ;
	TCD0.CTRLE = TCD_SYNCEOC_bm;		// Sync register values to PWMs
}

static inline void Clear_RTC(void) {
	RTC.INTFLAGS = RTC_OVF_bm;		// Clear interrupt flag
}

static inline void Clear_PIT(void) {
	RTC.PITINTFLAGS = RTC_PI_bm;	// Clear interrupt flag
}

static inline void Enter_Standby(void) {
	SLPCTRL.CTRLA = SLEEP_MODE_STANDBY |
					SLPCTRL_SEN_bm;				// Enable sleep
	sleep_cpu();
}

static inline void Enter_Idle(void) {
	SLPCTRL.CTRLA = SLEEP_MODE_IDLE |
					SLPCTRL_SEN_bm;				// Enable sleep
	sleep_cpu();
}

#endif /* HW_QUICK_H_ */
