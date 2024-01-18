#include <stdio.h>
#include <xc.h>
#include "ccp2capture.h"
// 配置 CCP2 用作輸入捕捉
void configureCCP2ForCapture() {
    PIR2bits.TMR3IF = 0;
//     配置 CCP2 为输入捕获模式，下降沿触发
    CCP2CON = 0x04; 
    ANSELC &= ~(1 << 1);    // Set RC1 as digital input
    TRISC |= 0b00000010;
    // 使能 CCP2 中断
    PIE2bits.CCP2IE = 1;
    CONFIG3H &= 0b11111110;
    // 使能全局和低优先级中断
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    CCPTMRS0 = (CCPTMRS0 & 0b11100111) | 00001000;
    T3CONbits.TMR3ON = 1;  // 启动 TMR3
    T3CONbits.TMR3CS = 0;  // 使用内部时钟
    T3CONbits.T3CKPS = 0b01;  // 分频 1:2
}

// CCP2 中斷服務程序


//// 主函數
//void main() {
//    // 初始化
//    configureCCP2ForCapture();
//
//    // 這裡可以加入你的主要代碼
//
//    while (1) {
//        // 你的主要代碼循環
//    }
//}
