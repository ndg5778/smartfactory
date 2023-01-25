#define F_CPU 160000L
#include <avr/io.h> // include the AVR IO library
#include <util/delay.h> // include the delay library
#include "pca9685.h" // include the PCA9685 library

// switch
void INIT_PORT();
// servo
void INIT_SERVO();

int main(void)
{
	INIT_PORT();
	INIT_SERVO();
	
	int8_t angle;
	
	while (1) {
		for (angle = -90; angle <= 90; ) {
			if(PIND & 0x04){
				_delay_ms(5);
			}
			else {
				pca9685_servo(0, angle);
				_delay_ms(200);
				 angle += 5;
			}
		}
	}
	
	return 0;
}


void INIT_PORT()
{
	DDRB = 0x20; // PB5 (������ 13�� ��) ������� ����
	PORTB = 0x00; // LED�� ���� ���¿��� ����
	DDRD = 0x00; // ��ư �Է�
	PORTD = 0x04; // PD2 (������ 2�� ��) Ǯ�� ���� ���
}

// servo
void INIT_SERVO()
{
	pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	_delay_ms(5);
	pca9685_servo(0, 0);
	_delay_ms(100);
}
