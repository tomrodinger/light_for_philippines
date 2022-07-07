#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "MyInclude.h"

extern uint8_t duty_Charger;
extern uint8_t duty_Lamp;
extern uint8_t LampStatus;		// 0=Off, 1=Dim, 2=OnFull
extern uint8_t Charging;		// 0=Off, 1=On
extern uint8_t BatteryFull;
extern uint8_t NextAction;		// 0 = Nothing, 1 = Turn lamp on, 2 = Turn lamp dim, 3 = Turn lamp off
								// 4 = Turn charger on, 5 = Turn charger off
extern uint16_t VSB_reading;
extern uint16_t VS3_reading;
extern uint16_t VSS_reading;
extern uint16_t high_reading;		// Reading on VS1
extern uint16_t low_reading;		// Reading on VS2
extern AdcStatus_t AdcStatus;

static int16_t lamp_current;		// Difference between VS1 and VS2

#include "HW_quick.h"

// Modified 2019-01-04:
static uint8_t const LampLimits[2][2] PROGMEM =	{	{	15,		// Dim low limit
													18}, 	// Dim hi limit
												{	146, 	// Full low limit
													149} };	// Full hi limit
/* Original version:
static uint8_t const LampLimits[2][2] PROGMEM =	{	{	15,		// Dim low limit
													18}, 	// Dim hi limit
												{	183, 	// Full low limit
													186} };	// Full hi limit
*/

static uint16_t const targetVSS [] PROGMEM = {	648, //655,	// Vss = 4 V   (Vss = Reading * 150)
												662, //670,	// Vss = 4.1 V
												692, //705,	// Vss = 4.3 V
												719, //740,	// Vss = 4.5 V
												795, //820,	// Vss = 5 V
												795};//820 };	// Vss = 5 V

#define targetVSS_Limit			700		// 4.25V -- Lower limit for Vss when the battery is full
#define targetVSS_Limit_Battery	540		// Battery voltage above which to enforce the targetVSS_Limit ~3.3V

#define BatteryLimits0	583		// 3.55 Volts. (Reading = 150*VBattery), Battery Full
#define BatteryLimits1	580		// Battery almost full (for hysteresis)
#define BatteryLimits2	508		// 3.1 Volts, Battery Low
#define BatteryLimits3	493		// 3.0 Volts, Battery Empty

#define VS3Limits0 250	// Current ~ 2.5A
#define VS3Limits1 195	// Close to max current, for hysteresis
#define VS3Limits2 18	// Limit for reliable MPPT ~0.2A
#define VS3Limits3 8	// Stop charging

uint8_t IsBatteryOK(void) {
	if ( VSB_reading < BatteryLimits3 ) { //battery is empty
		return 0;
	} else {
		return 1;
	}
}

// Returns 1 if lamp is way off, needs to seek more quickly. 
// Returns 0 if lamp is close to target brightness, or have reached max duty ratio
uint8_t ProcessLampBrightness(void) {
	if ( LampStatus == 0 || NextAction == 2 || NextAction == 3 ) {
		return 0;
	}
	lamp_current = high_reading - low_reading;
	if ( lamp_current < 0 ) {
		return 0;
	}
	if ( lamp_current < pgm_read_byte( &(LampLimits[LampStatus-1][0]) ) ) {		// Below the low limit
		duty_Lamp++;				//increase the duty cycle
		if ( duty_Lamp > 0xFE ) {
			duty_Lamp = 0xFE;
			return 0;
		}
	} else if ( lamp_current > pgm_read_byte( &(LampLimits[LampStatus-1][1]) ) ){		// Too bright -- above high limit
		duty_Lamp--;
		if ( duty_Lamp < 0x02 ) {
			duty_Lamp = 0x02;
		}
	} else {
		return 0;
	}
	SetLamp( duty_Lamp );
	return 1;
}

void ProcessChargingPWM(void) {
	uint16_t VSB_Charging;									// VSB reading adjusted for extra losses (in R12 and elsewhere)
	VSB_Charging = VSB_reading - ((VS3_reading * 19)>>6);	// Reduce VSB_Reading by the 0.17 Ohm resistances in the circuit
	if (VSB_Charging > BatteryLimits0) {					// battery voltage too high
		LED_Green(0);
		if ( VS3_reading < (VS3Limits3) ) {					// Battery full (because high voltage and low current)
			NextAction = 5;
			BatteryFull = 1;
			SetVst(0);
			if (LampStatus == 0 ) {
				SetPowerDownTime(PowerDownTimeLong);
			}
			return;
		}
		duty_Charger--; 			//decrease PWM
	} else if (VS3_reading > VS3Limits0) {					//current exceeded its max
		LED_Green(0);
		duty_Charger--; 			//decrease PWM
	} else if ( VS3_reading < VS3Limits1 ) { // Neither voltage nor current too high -- attempt MPPT
//		Print16bit(VSB_reading, 0);
//		Print16bit(VSS_reading, 0);
//		Print16bit(VS3_reading, 1);
		uint16_t targetVSS_now;								// Vss to aim for, adjusted for full vs not full battery
		uint8_t targetIndex = VS3_reading >> 4;
		if ( Charging ) {
			LED_Green(1);
		}
		if (targetIndex > 5) {
			targetIndex = 5;
		}
		targetVSS_now = pgm_read_word( &(targetVSS[targetIndex]) );
		if ( (VSB_reading > targetVSS_Limit_Battery) && (targetVSS_now < targetVSS_Limit) ) {
			targetVSS_now = targetVSS_Limit;
		}
		if ( VSS_reading > targetVSS_now ) {	// Solar voltage too high
			duty_Charger++;
		} else if ( VSS_reading < (targetVSS_now - 2) ) {
			duty_Charger--;
		}
	}
	if ( duty_Charger < dutyChargerStart ) {		// Impose bounds
		duty_Charger = dutyChargerStart;
	}
	if ( duty_Charger > dutyChargerMax ) {
		duty_Charger = dutyChargerMax;
	}
	if(VSB_reading>=targetVSS_Limit)//4.25V -- when the VSB voltage is higher than 4.55 means charge full
    	{
		BatteryFull = 1;
		duty_Charger=0;
    	}
   	if(VSB_reading<=BatteryLimits0)//3.55 Volts. (Reading = 150*VBattery), Battery Full
    	{
		BatteryFull = 0;
    	}
	SetCharger();
}

void LampOnOff(void) {
	static uint16_t VSB_LampOn;							// VSB reading adjusted for resistive losses during lamp operation
	VSB_LampOn = VSB_reading + ( (lamp_current*9) >> 7 );	// Increase VSB_Reading by the 0.07 Ohm resistances in the circuit
	if ( VSB_LampOn < BatteryLimits3 ) {				//battery is empty
		NextAction = 3; 								// Turn lamp off
		if ( Charging == 0 ) {
			SetPowerDownTime(PowerDownTimeShort);
		}
// Modified 2019-01-04:
	} else if ( VSB_LampOn < BatteryLimits2 ) {			// low battery
		if ( LampStatus == 2 ) { 						// Lamp was on full
			NextAction = 3;									// Set lamp to off
			if ( Charging == 0 ) {
				SetPowerDownTime(PowerDownTimeShort);
			}
		}
	}
/* Original version:
	} else if ( VSB_LampOn < BatteryLimits2 ) {			// low battery
		NextAction = 2;									// Set lamp to dim
	}
*/
	if ( VSB_reading < BatteryLimits1 ) {
		BatteryFull = 0;
	}
}

void ChargerOnOff(void) {								// Detect when Q6 is turning on and off and adjust the PWM limits accordingly
	static uint8_t was_on = 0;
	static uint8_t OldBatteryFull = 0;
	cli();
	if ( was_on ) {
		if ( GetVsOnOff() == 0 ) {	// Have turned off just now
			NextAction = 5;
			if ( LampStatus == 0 ) {
				SetPowerDownTime(PowerDownTimeLong);
			}
			was_on = 0;
		} else {	// was_on is high and GetVSOnOff is high
			if ( (BatteryFull==0) && (Charging==0) ) {
				NextAction = 4;
				SetPowerDownOff();
			}
		}
	} else {						// was_on == 0
		if ( GetVsOnOff() ) {
			if ( !BatteryFull ) {
				was_on = 1;
				NextAction = 4;
				SetPowerDownOff();
			}
		}
	}
	if ( (was_on == 1 ) && (OldBatteryFull == 1) && (BatteryFull == 0) ) {
		NextAction = 4;
		SetPowerDownOff();
	}
	OldBatteryFull = BatteryFull;
	sei();
}

#ifdef Use_USART
void PrintStats(void) {
	static uint16_t time = 0;
	if ( LampStatus > 0 ) {
		Print16bit(time, 0);
		Print16bit(lamp_current, 0);
		Print8bit(duty_Lamp,0);
		Print16bit(VSB_reading, 1);
		time++;
	}
	if ( Charging ) {
		Print16bit(time, 0);
		Print16bit(VSS_reading, 0);
		Print16bit(VS3_reading, 0);
		Print8bit(duty_Charger, 0);
		Print16bit(VSB_reading, 1);
		time++;
	}
}
#endif
