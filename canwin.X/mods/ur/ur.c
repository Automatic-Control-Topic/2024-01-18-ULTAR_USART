//**********************************************************
//*               Ex12_2_UART_Int.c
//*   利用UART中斷旗標RCxIF判斷UART資料的讀取
//**********************************************************
#include <xc.h> // 使用XC8編譯器定義檔宣告
#include "ur.h"
#define TMR1_VAL 	32768			// Timer1 設定為 1000ms 中斷一次 
#define _XTAL_FREQ 10000000 //使用 __delay_ms(x)時，一定要先定義此符號

//宣告函式原型
void Init_ADC(void);  // 初始化ADC
void Init_TMR1(void); // 初始化TIMER1

// 狀態旗標變數


// 定義低優先中斷執行程式位址及內容
// UART RX


void ur_main () {
  unsigned char result;
  
  ANSELD = 0x00;  // 將PORTD設為數位IO功能
  LATD = 0x00;    // 將LATD清除關閉LED
  TRISD = 0;      // 將TRISD設為0，PORTD設定為輸出

//  Init_ADC();  // 開啟類比訊號轉換模組
  Init_TMR1();// 初始化TIMER1

  //開啟USART通訊介面，設定為8-N-1-9600模式，開啟資料接收中斷功能
  ANSELCbits.ANSC6=0;  // 設定TX1為數位輸出
  TRISCbits.TRISC6=0;
  ANSELCbits.ANSC7=0;  // 設定RX1為數位輸入
  TRISCbits.TRISC7=1;  
	TXSTA1 = 0b00100100; // 8位元模式非同步傳輸，高鮑率設定，啟動傳輸功能
  RCSTA1 = 0b10010000; // 啟動8位元資料接收功能，連續接收模式，停止位址偵測點
  BAUDCON1 = 0x08;     // 設定16位元鮑率參數
	SPBRG1 = 0x03;       // 設定鮑率為9600，BRG16=1, BRGH=1, X=(10M/9600/4-1)=259
	SPBRGH1 = 0x01;      // SPBRGH1=0x01, SPBRG1=0x03
  
  PIR1bits.TX1IF = 0;   // 清除資料傳輸中斷旗標
	PIE1bits.TX1IE = 0;   // 停止資料傳輸中斷功能

	IPR1bits.RC1IP = 0;  // 設定資料接收低優先中斷
	PIR1bits.RC1IF = 0;  // 清除資料接收中斷旗標
	PIE1bits.RC1IE = 1;  // 啟動資料接收中斷

	RCONbits.IPEN=1;			// 開啟中斷優先功能
    INTCONbits.GIEL = 1;		// 開啟低優先中斷功能
	INTCONbits.GIEH = 1;		// 開啟高優先中斷功能

	FLAGbits.TxD = 0;			// 重置狀態旗標
	FLAGbits.One_S = 0;

//  while(1) {
//		while(FLAGbits.TxD==1);	// 判斷資料傳送狀態旗標
//		if(FLAGbits.One_S==1) {	// 判斷是否整秒
//			FLAGbits.One_S=0;	// 重置狀態旗標
//
//			ADCON0bits.GO = 1 ;		// 進行類比訊號轉換
//			while(ADCON0bits.DONE);	// 等待轉換完成
//
//			// 將十位元資料分三次傳出
//			while(!PIR1bits.TX1IF);	// 等待傳輸完成，緩衝器可輸入
//			TXREG1 = (ADRESH+0x30);	// 轉換成ASCII符號並傳出數值
//
//			result=(ADRESL>>4);	// 轉換成ASCII符號
//			if(result>9) result += 0x37;
//			else result+=0x30;
//			while(PIR1bits.TX1IF==0); // 等待傳輸完成，緩衝器可輸入
//			TXREG1 = (result);	// 傳出數值
//
//			result=(ADRESL & 0x0F);	// 轉換成ASCII符號
//			if(result>9) result += 0x37;
//			else result += 0x30;
//			while(PIR1bits.TX1IF==0); // 等待傳輸完成，緩衝器可輸入
//  		TXREG1 = (result);	// 傳出數值
//      Nop();  // 一定要有一個指令時間延遲，否則TXIF來不及轉換成1
//
//			while(PIR1bits.TX1IF==0); //傳出格式符號
//			TXREG1 = 0x0A; 
//	    Nop();  // 一定要有一個指令時間延遲，否則TXIF來不及轉換成1
//  		while(PIR1bits.TX1IF==0);
//			TXREG1 = 0x0D;
//		}
   }


// 初始化ADC模組
//void Init_ADC(void){
//  // 開啟類比訊號轉換模組
//  // 設定RA0為數位輸入，避免輸出干擾電壓。ANSELA0預設為類比可以使用預設值
//  TRISA = TRISA | 0x01;
//  // 選擇AN0通道轉換，開啟ADC模組 
//  ADCON0 = 0x01;
//  // 使用VDD，VSS為參考電壓; 
//  ADCON1 = 0x00;
//  // 結果向右靠齊並設定TAD時間為32Tosc，轉換時間為20TAD; 
//  ADCON2 = 0xBA;  
//}

// 初始化TIMER1
void Init_TMR1 (void){
  // 16位元模式、1倍前除器、非同步計數模式、使用外部震盪器，開啟計時器 
  T1CON =0b10001111;
  // Gate Control 沒有用到可以忽略而用預設值
  TMR1H = 0x80;  // 寫入預設值
  TMR1L = 0x00;
  PIR1bits.TMR1IF = 0;	// 清除中斷旗標
  IPR1bits.TMR1IP = 1;  // 設定Timer1為高優先中斷
  PIE1bits.TMR1IE = 1;	// 關閉Timer1中斷功能
}
