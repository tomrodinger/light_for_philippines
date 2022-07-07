#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "MyInclude.h"

volatile uint8_t RecentChange;
uint8_t LampStatus = 0;		// 0=Off, 1=Dim, 2=OnFull
uint8_t Charging = 0;		// 0=Off, 1=On
uint8_t BatteryFull = 0;
uint8_t duty_Charger;
uint8_t duty_Lamp;
volatile uint8_t NextAction = 0;	// 0 = Nothing, 1 = Turn lamp on, 2 = Turn lamp dim, 3 = Turn lamp off
									// 4 = Turn charger on, 5 = Turn charger off, 6 = Goto PowerDown
uint16_t VSB_reading = 0;
uint16_t VS3_reading = 0;
uint16_t VSS_reading = 0;
uint16_t high_reading = 0;		// Reading on VS1
uint16_t low_reading = 0;		// Reading on VS2
AdcStatus_t AdcStatus = Measure_VSS;

#include "HW_quick.h"

static void IdleOrStandby(void) {
#ifdef Use_USART
	if ( (USART0.STATUS & USART_DREIF_bm) == 0 ) {
		Enter_Idle();
		return;
	}
#endif
	if ( (LampStatus == 0) && (Charging==0) ) {
		if ( BatteryFull == 0 ) {
				SetVR3(1);
		}
		Enter_Standby();
		SetVR3(0);
	} else {
		Enter_Idle();
	}
}

int main(void){
	hw_init();
	SetVR3(0);
	SetPowerDownTime(PowerDownTimeShort);
	RecentChange = 5;

    while (1) {
		cli();				// Clear interrupts
		switch (NextAction) {
// Modified 2019-01-04:
		case 1:				// Turn lamp on full
			if ( LampStatus == 1 ) {
					LampStatus = 2;
					RecentChange = 5;
				}
			break;
		case 2:			// Turn lamp on and set to dim
			if ( LampStatus == 0 ) {	// If lamp was not on
				if ( IsBatteryOK() ) {
					BatteryFull = 0;
					LampOn(0x70);
				}
				LampStatus = 1;	
				RecentChange = 5;
			}
			break;
/* Original version:
		case 1:				// Turn lamp on full
			if ( LampStatus < 2 ) {
				if ( IsBatteryOK() ) {
					LampStatus = 2;
					BatteryFull = 0;
					LampOn(0x70);
					RecentChange = 5;
				}
			}
			break;
		case 2:			// Set lamp to dim
			if ( LampStatus == 2 ) {
				LampStatus = 1;
				RecentChange = 5;
			}
			break;
*/
		case 3:			// Set lamp to off
			if ( LampStatus > 0 ) {
				LampStatus = 0;
				LampOff();
				RecentChange = 5;
			}
			break;
		case 4:		// Turn charger on
			if ( Charging == 0 ) {
				Charging = 1;
				ChargerOn();
				RecentChange = 5;
				LED_Red(1);
			}
			break;
		case 5:		// Turn charger off
			if ( Charging > 0 ) {
				Charging = 0;
				ChargerOff();
				RecentChange = 5;
				LED_Red(0);
				LED_Green(0);
			}
			break;
		case 6:
			Enter_PD();	
			break;		
		default:
			break;
		}
		NextAction = 0;
		sei();
		IdleOrStandby();
	}
}

