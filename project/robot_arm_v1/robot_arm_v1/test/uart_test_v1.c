//#define F_CPU 16000000L
//#include <avr/io.h>
//#include <util/delay.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "UART.h"
//
//#define LED1 PB0
//#define LED2 PB1
//
//#define TERMINATOR '$'
//
//
//// uart
//void uart_RasToAt();
//
//// uart
//int index = 0;				// 수신 버퍼에 저장할 위치
//int process_data = 0;		// 문자열 처리
//char buffer[20] = "";		// 수신 데이터 버퍼
//char buffer_data[20];
//char data;					// 수신 데이터
//
//
//int main(int argc, char *argv[])
//{
	//UART_INIT();
	//
	//int pass = 0;
	//
	//while (pass == 0){
		//UART_printString("hello");
		//UART_printString("\n");
		//
		//uart_RasToAt();
		//
		//if ((strcmp(buffer_data, "0") == 0) || (strcmp(buffer_data, "1") == 0) || (strcmp(buffer_data, "2") == 0) || (strcmp(buffer_data, "3") == 0) || (strcmp(buffer_data, "4") == 0) || (strcmp(buffer_data, "5") == 0) || (strcmp(buffer_data, "6") == 0) || (strcmp(buffer_data, "7") == 0) || (strcmp(buffer_data, "8") == 0)){
			//UART_printString("pass");
			//UART_printString("\n");
			////UART_transmit(buffer_data);
			//pass = 1;
		//}
		//else if (strcmp(buffer, "9") == 0) {
			//UART_printString("end");
			//UART_printString(buffer);
			//UART_printString("\n");
			//
			//PORTB &= ~(1 << LED1);
			//return 0;
		//}
	//}
	//
	//
	//
//
	//return 0;
//}
//
//void uart_RasToAt(){
	//data = UART_receive();	// 데이터 수신
	//if (data != ""){
		//if(data == TERMINATOR) {	// 종료 문자를 수신한 경우
			//buffer[index] = '\0';
			//process_data = 1;		// 수신 문자열 처리 지시
			//index = 0;
		//}
		//else {
			//buffer[index] = data;	// 수신 버퍼에 저장
			//index++;
		//}
	//}
//}