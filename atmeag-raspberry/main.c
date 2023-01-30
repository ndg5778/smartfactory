#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART.h"

#define ROTATION_DELAY 	1500	// 1�� ���
#define PULSE_MIN		(40000*0.0325)	// �ּ� �޽� ����
#define PULSE_MID		3000
#define PULSE_MAX		(40000*0.1175)	// �ִ� �޽� ����

void INIT_TIMER1(void)
{
	// Fast PWM ���, TOP = ICR1
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	TCCR1B |= (1 << CS11);		// ������ 8, 2MHz
	TCCR1A |= (1 << COM1A1);		// ����� ���
	ICR1 = 40000;				// 20ms �ֱ�
	
	DDRB = 0x07; 
	//DDRB |= (1 << PB1); //  OC1A (PB1, �Ƶ��̳� 9��) ���� ������� ����
}

int main(int argc, char *argv[])
{
	int index = 0;
	char buffer[20] = "";
	//DDRB = 0x03; // PB0���� led
	
	unsigned char data;
	UART_INIT(); // UART ��� �ʱ�ȭ
	INIT_TIMER1();
	
	while (1) {
		PORTB |= 0x04;
		data = UART_receive(); // ������ ����
		buffer[index] = data;
		
		if(strcmp(buffer, "h") == 0) {
			PORTB |= 0x01;
			//OCR1A = PULSE_MIN;
			//_delay_ms(ROTATION_DELAY);
			index = 0;
			UART_transmit(data);
		}
		
		else if(strcmp(buffer, "l") == 0) {
			PORTB &= ~0x01;
			//OCR1A = PULSE_MID;
			//_delay_ms(ROTATION_DELAY);
			index = 0;
			UART_transmit(data);
		}
		
		else{
			UART_transmit(data);
			index = 0;
		}
		
		//UART_transmit(data); // ���ŵ� ���ڸ� ���� ��
	}
	return 0;
}