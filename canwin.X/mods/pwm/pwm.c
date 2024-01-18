#include <stdio.h>
#include <xc.h>
#include "pwm.h"
#define _XTAL_FREQ 10000000
#define OSC_CLOCK 10
#define Count 0x00

void delay_ms(unsigned long A);
int intToBinary(int num);
double math_duty_cycle(double duty_cycle);
double math_period(double period);
void pwm_init(double duty_cycle, double period){
    math_period(period);           // Set PWM period for a frequency of 4kHz
    math_duty_cycle(duty_cycle);
    ANSELC &= ~(1 << ANSC2);    // Set RC2 as digital output
    TRISC &= ~(1 << 2);
    CCP1CON = 0b00001100;   // Set CCP1 as PWM mode with duty cycle bits at 00
    T2CON = 0b00000101;     // Start TIMER2 with a prescaler of 4
//    ADCON0bits.GO = 1;  // Start A/D conversion
    }


void delay_ms(unsigned long A) {
    unsigned long i;
    for(i=0;i<A;i++) __delay_ms(1);
}
int intToBinary(int num) {
    int binary = 0;
    int base = 1;  // 二進制的基數

    while (num > 0) {
        int remainder = num % 2;
        binary += remainder * base;
        num /= 2;
        base *= 10;
    }

    return binary;
}

double math_duty_cycle(double duty_cycle) {
    double x, y, z;
    y = 1.0 / duty_cycle;
    x = y / 0.0000004;
    // 使用強制轉型將 double 轉換為 int
    CCPR1L = intToBinary((int)x) & ~(0b0000000011);
    // 使用強制轉型將 double 轉換為 int，同時進行位元操作
    CCP1CON = (CCP1CON & 0b11001111) | (((intToBinary((int)x) & 0b0000000011) << 4) & 0b00110000);
}
double math_period(double period) {
    double x, y, z;
    y = 1.0 / period;
    x = y / 0.0000004/4;
    PR2 = intToBinary((int)x - 1);
}
