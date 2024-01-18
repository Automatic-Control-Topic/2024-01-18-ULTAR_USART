#ifndef __UR_H_
#define __UR_H_
#include <stdio.h>
void ur_main ();
// 定義狀態旗標結構
static struct Flag {
    unsigned One_S : 1; // One Second Passed
    unsigned TxD : 1;   // UART Tx Continue
} FLAGbits;
// 定義 result 變數
static unsigned char result;
extern struct Flag FLAGbits;
static unsigned int test = 0;

#endif // MOD_H