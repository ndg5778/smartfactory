#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART.h"

#define LED_PIN1 PB0
#define LED_PIN2 PB1

int main(int argc, char *argv[])
{
	int index = 0;
	char buffer[20] = "";
	
	unsigned char data;
	UART_INIT(); // UART ��� �ʱ�ȭ
	
	DDRB |= (1 << LED_PIN1) | (1 << LED_PIN2); // set PB0, PB1 as output
	
	while (1) {
		PORTB |= (1 << LED_PIN1);		// ��ȣ Ȯ�ο�. PB0 high
		data = UART_receive(); // ������ ����
		buffer[index] = data;
		
		if(strcmp(buffer, "h") == 0) {
			PORTB |= (1 << LED_PIN2);	// PB1 high
			UART_transmit(data);	// ���ŵ� ���ڸ� ���� ��
			index = 0;
		}
		
		else if(strcmp(buffer, "l") == 0) {
			PORTB &= ~(1 << LED_PIN2);;	// PB2 low
			UART_transmit(data);
			index = 0;
		}
		
		else{
			UART_transmit(data);
			index = 0;
		}
		PORTB &= ~0x01;	
	}

	return 0;
}