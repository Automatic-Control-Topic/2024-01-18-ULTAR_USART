#include <stdio.h>
#include <xc.h>
#include "init.h"
void init_main (void){
    ANSELD = 0x00;
    TRISD = 0x00;
    LATD = 0x01;
    ANSELB = 0X00;
    TRISB = 0X01;
}