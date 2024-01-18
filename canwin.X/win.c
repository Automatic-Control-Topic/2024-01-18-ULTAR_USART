//#include "Sonic_R_pin_manager.h"
//#include "txRing.h"
#include <xc.h>
#include <math.h>
#include <stdio.h>
#define _XTAL_FREQ 10000000
#define OSC_CLOCK 10
#include "mods/ur/ur.h"
#include "mod.h"
#include "mods/init/init.h"
#include "mods/pwm/pwm.h"
#include "mods/ccp2capture/ccp2capture.h"
void __interrupt(low_priority) LowISR(void)
{
    LATD = (LATD & 0b01111111) | 10000000;
	unsigned char RX_TEMP;
    RX_TEMP=RCREG1;	// 讀取USART資料位元組
	if(RX_TEMP=='c')	FLAGbits.TxD=0;	// 'c'則傳送資料
	if(RX_TEMP=='p')	FLAGbits.TxD=1;	// 'p'則停止傳送
    if(RX_TEMP=='a')	test = 1;	
    if(RX_TEMP=='b')	test = 0;	
    if (PIR2bits.CCP2IF == 1) {
        PIE2bits.CCP2IE = 0;
        // 在這裡處理 CCP2 捕捉事件
        // 清除 CCP2 中斷標誌
        while(PIR1bits.TX1IF==0); // 等待傳輸完成，緩衝器可輸入
            TXREG1 = 0x31;	// 傳出數值
        PIE2bits.CCP2IE = 1;
        PIR2bits.CCP2IF = 0;
        LATD += 1;
    }
    LATD = (LATD & 0b01111111) | 00000000;
}

// 定義高優先中斷執行程式位址及內容
// TIMER1
void __interrupt() HighISR(void)
{
    LATD = (LATD & 0b10111111) | 01000000;
	PIR1bits.TMR1IF = 0;		// 清除中斷旗標
	TMR1H=0x80;		// 寫入計時預設值0x8000
    TMR1L=0x00;
	FLAGbits.One_S=1;			// 設定整秒旗標以利正常程式更新資料
    LATD = (LATD & 0b10111111) | 00000000;
    if (PIR2bits.CCP2IF) {
        PIE2bits.CCP2IE = 0;
        // 在這裡處理 CCP2 捕捉事件
        // 清除 CCP2 中斷標誌
        while(PIR1bits.TX1IF==0); // 等待傳輸完成，緩衝器可輸入
            TXREG1 = 0x31;	// 傳出數值
        PIE2bits.CCP2IE = 1;
        PIR2bits.CCP2IF = 0;
        LATD += 1;
    }
}


void main(void){
    init_main();
    myFunction();
    pwm_init(4000.0, 4000.0);
    int count = 0;
    ur_main ();
    LATD = 0X01;
    configureCCP2ForCapture();
    while(1){
        if(test == 1){
        pwm_init(4000.0, 4000.0);
//        TRISC &= ~(1 << 2);
    }
    else{
//        TRISC |= 0b00000100;
        pwm_init(10000.0, 5000.0);
        delay_ms(1);
        pwm_init(4000.0, 4000.0);
        test = 1;
    }
        if(FLAGbits.TxD!=1){	// 判斷資料傳送狀態旗標
		if(FLAGbits.One_S==1) {	// 判斷是否整秒
			FLAGbits.One_S=0;	// 重置狀態旗標
			while(PIR1bits.TX1IF==0); // 等待傳輸完成，緩衝器可輸入
            TXREG1 = 0x30;	// 傳出數值
            Nop();  // 一定要有一個指令時間延遲，否則TXIF來不及轉換成1
			while(PIR1bits.TX1IF==0); //傳出格式符號
			TXREG1 = 0x0A; 
            Nop();  // 一定要有一個指令時間延遲，否則TXIF來不及轉換成1
            while(PIR1bits.TX1IF==0);
            TXREG1 = 0x0D;
            //
        }}
    
}
}
