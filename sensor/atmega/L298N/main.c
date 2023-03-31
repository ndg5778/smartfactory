#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define STEPPING_DDR DDRD
#define STEPPING PORTD

#define STEPPING_A_IN1 PD0
#define STEPPING_A_IN2 PD1
#define STEPPING_A_IN3 PD2
#define STEPPING_A_IN4 PD3

#define STEPPING_B_IN1 PD4
#define STEPPING_B_IN2 PD5
#define STEPPING_B_IN3 PD6
#define STEPPING_B_IN4 PD7

void init_stepper()
{
	// stepping motor
	STEPPING_DDR |= 0xFF;
	// led
	DDRC |= 0x20;	// led 상태등 표시 PC5
}

void loop_stepper()
{
	STEPPING &= ~((1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN4));	// low
	STEPPING |= (1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN3);		// high
	STEPPING &= ~((1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN4));
	STEPPING |= (1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN3);
	//_delay_us(10000);
	_delay_ms(10);


	STEPPING &= ~((1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN3));
	STEPPING |= (1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN4);
	STEPPING &= ~((1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN3));
	STEPPING |= (1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN4);
	//_delay_us(10000);
	_delay_ms(10);

	
	STEPPING &= ~((1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN3));
	STEPPING |= (1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN4);
	STEPPING &= ~((1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN3));
	STEPPING |= (1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN4);
	//_delay_us(10000);
	_delay_ms(10);

	STEPPING &= ~((1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN4));
	STEPPING |= (1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN3);
	STEPPING &= ~((1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN4));
	STEPPING |= (1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN3);
	//_delay_us(10000);
	_delay_ms(10);
	
}

int main(void)
{
	init_stepper();
	
	// 타이머/카운터 2번을 고속 PWM 모드로 설정
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2A |= (1 << COM2A1);		// 비반전 모드
	TCCR2B |= (1 << CS22);			// 분주비 64

	while (1)
	{
		PORTC |= 0x20;
		loop_stepper();
		PORTC &= ~0x20;
		loop_stepper();
	}
	
	return 0;
}
