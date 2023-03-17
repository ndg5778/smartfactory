//#define F_CPU 16000000L
//#include <avr/io.h> // include the AVR IO library
//#include <util/delay.h> // include the delay library
////#include <stdio.h>
//#include "pca9685.h" // include the PCA9685 library
//
//#define switch1 PB2
//
//#define LED_DDR DDRB
//#define LED_PORT PORTB
//#define LED1 PB0
//#define LED2 PB1
//
//// 서보모터 초기화
//void INIT_SERVO();
//
//
//int main(void) {
	////LED_DDR |= (1 << LED1) | (1 << LED2);
	////DDRB &= ~(1 << switch1);
	//
	//PORTB |= (1 << LED1);
	//
	//int i;
	//
	//INIT_SERVO();
	//_delay_ms(1000);
	//
	//while (1) {
		//for (i = 600; i <= 2400; ) {
			//if (PINB & (1 << switch1)) {
				//PORTB &= ~0x02;
				//pca9685_pwm(0, i);
				//i += 10;
			//}
			//else {
				//PORTB |= 0x02;
				//break;
			//}
			//_delay_ms(15);
		//}
		//
	//}
//}
//
//void INIT_SERVO(){
	//
	//pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	//int i;
	//
	//for (i = 1; i <= 5; i++){
		//pca9685_pwm(0, 1500);
	//}
//}