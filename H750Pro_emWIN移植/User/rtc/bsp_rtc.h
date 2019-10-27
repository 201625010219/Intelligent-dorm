#ifndef __RTC_H__
#define __RTC_H__

#include "stm32h7xx.h"

extern RTC_HandleTypeDef Rtc_Handle;
//���ڽṹ��
typedef struct
{
    char year[5];
    char month[4];
    char day[4];
    char hour[3];
    char minute[3];
    char second[3];
}DateStruct;

//ʱ�ӽṹ��
typedef struct
{
    char hour[3];
    char minute[3];
    char second[3];
}TimeStruct;

// ʱ��Դ�궨��
#define RTC_CLOCK_SOURCE_LSE      
//#define RTC_CLOCK_SOURCE_LSI  

// �첽��Ƶ����
#define ASYNCHPREDIV         0X7F
// ͬ����Ƶ����
#define SYNCHPREDIV          0XFF

// ʱ��궨��
#define RTC_H12_AMorPM			  RTC_HOURFORMAT12_AM  
#define HOURS                     23         // 0~23
#define MINUTES                   18          // 0~59
#define SECONDS                   23          // 0~59


// ���ں궨��
#define WEEKDAY                   5         // 1~7
#define DATE                      6         // 1~31
#define MONTH                     9        // 1~12
#define YEAR                      19         // 0~99

// ������غ궨��
#define ALARM_HOURS               23        // 0~23
#define ALARM_MINUTES             22         // 0~59
#define ALARM_SECONDS             0          // 0~59

#define ALARM_MASK                RTC_ALARMMASK_DATEWEEKDAY
#define ALARM_DATE_WEEKDAY_SEL    RTC_ALARMDATEWEEKDAYSEL_WEEKDAY
#define ALARM_DATE_WEEKDAY        RTC_WEEKDAY_FRIDAY
#define RTC_Alarm_X               RTC_ALARM_A

// ʱ���ʽ�궨��
#define RTC_Format_BINorBCD  RTC_FORMAT_BIN

// ������Ĵ����궨��
#define RTC_BKP_DRX          RTC_BKP_DR0
// д�뵽���ݼĴ��������ݺ궨��
#define RTC_BKP_DATA         0X32F2 
                                  
void RTC_CLK_Config(void);
void RTC_TimeAndDate_Set(void);
void RTC_TimeAndDate_Show(void);
void RTC_AlarmSet(void);

#endif // __RTC_H__
