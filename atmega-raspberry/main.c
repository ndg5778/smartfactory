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
	UART_INIT(); // UART ��� �ʱ�ȭ
	
	DDRB = 0x07; // set PB0, PB1, PB2 as output
	
	while (1) {
		PORTB |= 0x04;		// ��ȣ Ȯ�ο�. PB2 high
		data = UART_receive(); // ������ ����
		buffer[index] = data;
		
		if(strcmp(buffer, "h") == 0) {
			PORTB |= 0x02;	// PB1 high
			UART_transmit(data);	// ���ŵ� ���ڸ� ���� ��
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