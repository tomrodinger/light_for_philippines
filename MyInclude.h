#ifndef MyInclude_h
#define MyInclude_h

typedef enum AdcStatus_enum { 	Measure_VS1, Measure_VS2, Measure_VS3, Measure_VSB, Measure_VSS, Measure_CS
} AdcStatus_t;

// #define Use_USART		// Removed 2019-01-04

#define dutyChargerStart	0x10
#define dutyChargerMax		0xEA
#define PowerDownTimeShort	5400		// In units of ~ 16sec: 5400 is about 1 day
// #define PowerDownTimeShort	1
#define PowerDownTimeLong	37800		// About 7 days

// In HW_Init.c:
void hw_init(void);

// In HW.c:
void CS_prepare(void);
void ADC_prepare(void);
void LampOn(uint8_t brightness);
void LampOff(void);
void ChargerOn(void);
void ChargerOff(void);
void SetPowerDownTime(uint16_t TimeOut);
void SetPowerDownOff(void);
void Enter_PD(void);
#ifdef Use_USART
void USART_0_write(const uint8_t data);
void Print8bit( uint8_t data, uint8_t eol );
void Print16bit( uint16_t data, uint8_t eol );
#endif // Use_USART

// In CS.c:
void ProcessCS(uint16_t NewReading);

// In ISR.c:
void CS_Trigger(void);
void LongDelay_Trigger(void);
void CS_StuckOn_Trigger(void);
#ifdef Use_USART
void PrintStats(void);
#endif

// In Feedback.c:
uint8_t IsBatteryOK(void);
uint8_t ProcessLampBrightness(void);
void LampOnOff(void);
void ChargerOnOff(void);
void ProcessChargingPWM(void);

#endif
