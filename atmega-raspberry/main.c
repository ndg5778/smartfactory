#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART.h"

int main(int argc, char *argv[])
{
	int index = 0;
	char buffer[20] = "";
	
	unsigned char data;
	UART_INIT(); // UART 통신 초기화
	
	DDRB = 0x07; // set PB0, PB1, PB2 as output
	
	while (1) {
		PORTB |= 0x04;		// 신호 확인용. PB2 high
		data = UART_receive(); // 데이터 수신
		buffer[index] = data;
		
		if(strcmp(buffer, "h") == 0) {
			PORTB |= 0x02;	// PB1 high
			UART_transmit(data);	// 수신된 문자를 에코 백
			index = 0;
		}
		
		else if(strcmp(buffer, "l") == 0) {
			PORTB &= ~0x02;	// PB1 low
			UART_transmit(data);
			index = 0;
		}
		
		else{
			UART_transmit(data);
			index = 0;
		}
	}

	return 0;
}