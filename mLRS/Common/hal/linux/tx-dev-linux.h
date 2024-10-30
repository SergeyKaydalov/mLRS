#pragma once

#include <unistd.h>

#define DEVICE_HAS_JRPIN5

#define SYSTICK_DELAY_MS(x)       (uint16_t)(((uint32_t)(x)*(uint32_t)1000)/SYSTICK_TIMESTEP)

//-------------------------------------------------------
//  STD MACROS
//-------------------------------------------------------

#define ABS(a)              ((a)<(0)?(-a):(a))

#define MIN(a,b)            ((a)<(b)?(a):(b))
#define MAX(a,b)            ((a)>(b)?(a):(b))

#define INCc(a,period)      (a)++; if((a)>=(period)){ (a)=0;}
#define DECc(a,period)      ((a)<=0?((a)=(period)-1):((a)--))

#define INCl(a)             if((a)<255){(a)++;}
#define DECl(a)             if((a)>0){(a)--;}

#define INClm(a,max)        if((a)<(max)){(a)++;}

#define INCln(x,n)          {for(i=0;i<n;i++) INCl(x);}
#define DECln(x,n)          {for(i=0;i<n;i++) DECl(x);}

#define INCOPTc(a,min,max)  if((a)<(max)){ (a)++; }else{ (a)=(min); }
#define DECOPTc(a,min,max)  if((a)>(min)){ (a)--; }else{ (a)=(max); }

#define LIMMIN(a,min)       if((a)<(min)){(a)=(min);}
#define LIMMAX(a,max)       if((a)>(max)){(a)=(max);}
#define LIMIT(a,min,max)    if((a)<(min)){(a)=(min);}else{if((a)>(max)){(a)=(max);}}

#define CLIPU8(x)           ((x)<UINT8_MAX)?(x):UINT8_MAX
#define CLIPS8(x)           ((x)>INT8_MAX)?UINT8_MAX:((x)<INT8_MIN)?INT8_MIN:(x)
#define CLIPU16(x)          ((x)<UINT16_MAX)?(x):UINT16_MAX

//-------------------------------------------------------
//  STD string handling routines
//-------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
//-- conversion of integers to BCD string

void u8toBCDstr(uint8_t n, char* s);
void s8toBCDstr(int8_t n, char* s);
void u16toBCDstr(uint16_t n, char* s);
void s16toBCDstr(int16_t n, char* s);
void u32toBCDstr(uint32_t n, char* s);
void s32toBCDstr(int32_t n, char* s);

void utoBCDstr(uint32_t n, char* s); // without leading zeros
void stoBCDstr(int32_t n, char* s); // without leading zeros

//-- conversion of u8 to HEX char

char u8toHEXchar(uint8_t n);

//-- conversion of integers to HEX string

void u8toHEXstr(uint8_t n, char* s);
void u16toHEXstr(uint16_t n, char* s);
void u32toHEXstr(uint32_t n, char* s);
void u64toHEXstr(uint64_t n, char* s);

//-- conversion of HEX string/char to integers

uint8_t HEXstrtou8(char* s);
uint16_t HEXstrtou16(char* s);
uint16_t HEXchartou16(uint16_t c);

//-- test functions

uint16_t isHEXchar(char c);
uint16_t isHEXstr(char* s);

//-- conversion of BCD string to integers

uint16_t BCDstrtou16(char* s);
uint32_t BCDstrtou32(char* s);

//-- implicit conversion of integers to BCD string

char* u8toBCD_s(uint8_t n);
char* s8toBCD_s(int8_t n);
char* u16toBCD_s(uint16_t n);
char* s16toBCD_s(int16_t n);
char* u32toBCD_s(uint32_t n);
char* s32toBCD_s(int32_t n);
char* utoBCD_s(uint32_t n);
char* u8toHEX_s(uint8_t n);
char* u16toHEX_s(uint16_t n);
char* u32toHEX_s(uint32_t n);
char* u64toHEX_s(uint64_t n);
#ifdef __cplusplus
}
#endif


#define __REV16(x)  __builtin_bswap16(x)
#define __REVSH(x)  __builtin_bswap16(x)
#define __REV(x)    __builtin_bswap32(x)

typedef enum {
    EE_STATUS_FLASH_FAIL = 0, // indicates failure in hal functions
    EE_STATUS_PAGE_UNDEF,
    EE_STATUS_PAGE_EMPTY,
    EE_STATUS_PAGE_FULL,
    EE_STATUS_OK
} EE_STATUS_ENUM;

EE_STATUS_ENUM ee_writedata(void* data, uint16_t datalen)
{
    return EE_STATUS_OK;
}

EE_STATUS_ENUM ee_readdata(void* data, uint16_t datalen)
{
    return EE_STATUS_OK;
}

EE_STATUS_ENUM ee_init(void)
{
    return EE_STATUS_OK;
}

bool button_pressed(void)
{
    return false;
}

void led_green_off(void) {  }
void led_green_on(void) {  }
void led_green_toggle(void) {  }

void led_red_off(void) {  }
void led_red_on(void) {  }
void led_red_toggle(void) {  }

void delay_ms(uint32_t ms)
{
    sleep(ms);
}

volatile uint32_t doSysTask = 0;

uint8_t restart_controller;
#define INITCONTROLLER_ONCE \
    if(restart_controller <= 1){ \
    if(restart_controller == 0){
#define RESTARTCONTROLLER \
    }
#define INITCONTROLLER_END \
    restart_controller = UINT8_MAX; \
    }
#define GOTO_RESTARTCONTROLLER \
    restart_controller = 1; \
    return;


typedef enum {
    SX_IRQ_TX_DONE = SX1280_IRQ_TX_DONE,
    SX_IRQ_RX_DONE = SX1280_IRQ_RX_DONE,
    SX_IRQ_TIMEOUT = SX1280_IRQ_RX_TX_TIMEOUT,
    SX_IRQ_ALL     = SX1280_IRQ_ALL,
} SX_IRQ_ENUM;

volatile uint32_t millis32()
{
    return 0;
}

uint16_t micros16(void)
{
    return 0;
}

void main_loop(void);
int main_main(void) { while(1) main_loop(); }

#define __disable_irq()
#define __enable_irq()
#define delay_init()
#define timer_init()
#define leds_init()
#define button_init()
#define stack_check_init()

#ifndef STATIC_ASSERT
#define STATIC_ASSERT_(cond,msg)  static_assert((cond),msg);
#define STATIC_ASSERT(cond,msg)   static_assert((cond), msg ": (" #cond ") failed");
#endif