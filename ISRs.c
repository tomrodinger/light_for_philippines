#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "MyInclude.h"

extern uint8_t LampStatus;		// 0=Off, 1=Dim, 2=OnFull
extern uint8_t Charging;		// 0=Off, 1=On
extern uint8_t duty_Charger;
extern volatile uint8_t RecentChange;
extern volatile uint8_t NextAction;
extern uint16_t VSB_reading;
extern uint16_t VS3_reading;
extern uint16_t VSS_reading;
extern uint16_t high_reading;		// Reading on VS1
extern uint16_t low_reading;		// Reading on VS2
extern AdcStatus_t AdcStatus;

static uint8_t ADC_busy = 0;
static uint8_t CS_request = 0;

#include "HW_quick.h"

ISR(PORTA_PORT_vect) {
	PORTA.INTFLAGS = 0xFF;	// Clear interrupts on all pins
}

ISR(RTC_CNT_vect) {
	Clear_RTC();		// Clear interrupt flag
	NextAction = 6;
}

ISR(RTC_PIT_vect) {
	if ( ADC_busy ) {	// Should do CS measurement, but if ADC is busy, only set a flag so we can do it later.
		CS_request = 1;
	} else {
		if ( AdcStatus != Measure_CS ) {
			CS_prepare();
		}
		AdcStatus = Measure_CS;
		ADC_start();
		ADC_busy = 1;
	}
	if ( RecentChange > 0 ) {
		RecentChange--;
	}
	Clear_PIT();	// Clear interrupt flag
}

ISR(ADC0_RESRDY_vect) {
	static uint8_t brightness_Seeking = 0;

	if ( CS_request ) {					// It's time to measure CS. Throw away latest measurement and do CS.
		ADC_read();
		if ( AdcStatus != Measure_CS ) {
			CS_prepare();
		}
		AdcStatus = Measure_CS;
		ADC_start();
		ADC_busy = 1;
		CS_request = 0;
		return;
	}

	switch (AdcStatus) {
	
	case Measure_CS:
		ProcessCS( ADC_read() );
		if ( LampStatus > 0 ) {
			SetADC_VS1();
			AdcStatus = Measure_VS1;
			ADC_start();
		} else if ( (Charging > 0) || (RecentChange > 0) ) {
			SetADC_VSB();
			AdcStatus = Measure_VSB;
			ADC_start();
		} else {
			ADC_busy = 0;
		}
		break;

	case Measure_VS1:
		high_reading = ADC_read();
		SetADC_VS2();
		AdcStatus = Measure_VS2;
		ADC_start();
		break;

	case Measure_VS2:
		low_reading = ADC_read();
		brightness_Seeking = ProcessLampBrightness();
		SetADC_VSB();
		AdcStatus = Measure_VSB;
		ADC_start();
		break;

	case Measure_VSB:
		VSB_reading = ADC_read();
		LampOnOff();
		ChargerOnOff();
		if ( Charging ) {
			SetADC_VS3();
			AdcStatus = Measure_VS3;
			ADC_start();
		} else if ( brightness_Seeking && LampStatus ) {
				SetADC_VS1();
				AdcStatus = Measure_VS1;
				ADC_start();
		} else {
			ADC_busy = 0;
		}
		break;

	case Measure_VS3:
		VS3_reading = ADC_read();
		SetADC_VSS();
		AdcStatus = Measure_VSS;
		ADC_start();
		break;

	case Measure_VSS:
		VSS_reading = ADC_read();
		ProcessChargingPWM();
		if ( brightness_Seeking && LampStatus ) {
				SetADC_VS1();
				AdcStatus = Measure_VS1;
				ADC_start();
		} else {
			ADC_busy = 0;
		}
		break;
	}
}

// Modified 2019-01-04:
void CS_Trigger(void) {
	if ( LampStatus == 0 ){	// If lamp was off
		NextAction = 2;		// Turn lamp dim
		SetPowerDownOff();
	}
	else if ( LampStatus == 2 ){ //If lamp was full on
		NextAction = 3;		// Turn lamp off
		if ( Charging == 0 ) {
			SetPowerDownTime(PowerDownTimeLong);
		}
	}
	else{ 					// lamp status == 1 (dim)
		NextAction = 1;		// Turn lamp on full
	}
}
/* Original version
void CS_Trigger(void) {
	if ( LampStatus == 0 ){
		NextAction = 1;		// Turn lamp on
		SetPowerDownOff();
	}
	else if ( LampStatus == 2 ){
		NextAction = 2;		// Turn lamp dim
	}
	else{ // lamp status = 1 (dim)
		NextAction = 3;		// Turn lamp off
		if ( Charging == 0 ) {
			SetPowerDownTime(PowerDownTimeLong);
		}
	}
}
*/

void LongDelay_Trigger(void) {
#ifdef Use_USART
	PrintStats();
#endif
	ChargerOnOff();
}

void CS_StuckOn_Trigger(void) {
	NextAction = 3;
	if ( Charging == 0 ) {
		SetPowerDownTime(PowerDownTimeLong);
	}
}
