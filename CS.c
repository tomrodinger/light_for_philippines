#include <avr/io.h>
#include <stdlib.h>
#include "MyInclude.h"

extern AdcStatus_t AdcStatus;
extern uint8_t duty_Charger;

#include "HW_quick.h"

static inline void FullProcessCS(uint16_t NewReading);

#define ThresholdDelta		200
#define FullProcessCount	40
#define BlindCount			6
#define StuckOnCount		20

static uint16_t OldValue = 0x00;
static uint16_t Threshold = 0x3FFF;
static uint16_t Baseline = 0x3FFF;

void ProcessCS(uint16_t NewReading) {
	static uint8_t FullProcessInd = 4;
	static uint8_t BlindInd = 0;
	static uint8_t StuckOnInd = 0;

#ifdef Use_USART
//	Print16bit(NewReading, 0);
//	Print16bit(Baseline, 1);
#endif

	if ( (NewReading > Threshold) && (OldValue > Threshold) ) {
		if ( BlindInd == 0 ) {
			// Have detected a new touch -- Call necessary function
			CS_Trigger();
			BlindInd = BlindCount;
			StuckOnInd = 0;
		} else {	// BlindInd > 0
			// This is not a new touch
			StuckOnInd++;
			if ( StuckOnInd > StuckOnCount ) {
				CS_StuckOn_Trigger();
				StuckOnInd = 0;
			}
		}
	} else {	// Threshold not reached
		if ( BlindInd > 0) {
			BlindInd--;
		}
	}
	if ( FullProcessInd == 0 ) {
		FullProcessCS(NewReading);
		FullProcessInd = FullProcessCount;
		LongDelay_Trigger();
	}
	FullProcessInd--;
	OldValue = NewReading;
}

static inline void FullProcessCS(uint16_t NewReading) {
	if ( (NewReading < Baseline) && (OldValue < Baseline) ) {	// Need to lower Baseline and Threshold
		if ( NewReading > OldValue ) {
				Baseline = NewReading;
		} else {
			Baseline = OldValue;
		}
	} else {	// Need to raise Baseline 
		Baseline = (uint16_t)( ( (7*((uint32_t)Baseline)) + NewReading) >> 3 );
	}
	Threshold = Baseline + ThresholdDelta;
}
