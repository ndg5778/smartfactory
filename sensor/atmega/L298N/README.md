# ATmega328p로 L298N을 이용하여 stepping motor 돌리기

사용한 모터 : [유니폴라 스테핑 모터](https://www.devicemart.co.kr/goods/view?no=1357347)
하지만 [L298N](https://www.devicemart.co.kr/goods/view?no=1278835)은 바이폴라 드라이버.
유티폴라 모터는 바이폴라 형식으로 돌릴 수 있어 가능하였음.

ENA, ENB는 ATmega328p에 연결하지 않고 +에 연결하였음. (핀을 줄이기 위함)
상세하게 연결하고 싶으면 ATmega328p에 연결하여도 괜찮음.

스테핑모터를 같은 포트에 연결하고 싶어 PORTD에 모두 할당해줌.

아래는 main.c의 코드와 동일함.

아래의 코드를 사용하면 다음 영상과 같이 된다. (링크공개 영상)
https://youtu.be/AbVaCdjIxz4

``` C
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


```
