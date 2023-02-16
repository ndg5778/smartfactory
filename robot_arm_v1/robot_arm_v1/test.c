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
//void INIT_SERVO(){
	//
	//pca9685_init(0x00, 51); // start PCA9685 device 0x00 at 50 Hz output
	//
	//_delay_ms(5);
	//
	//int i;
	//
	//for (i = 0; i < 5; i++){
		//pca9685_pwm(i, 1300);
	//}
//}
//
//int main(void) {
	//LED_DDR |= (1 << LED1) | (1 << LED2);
	//DDRB &= ~(1 << switch1);
	//
	//PORTB |= (1 << LED1);
	//INIT_SERVO();
	//
	//_delay_ms(1000);
	//
	//while (1) {
		////INIT_SERVO();
		//int angle;
		//for (angle = 1800; angle <= 600; angle -= 20) {
			//PORTB |= 0x02;
			//pca9685_pwm(3, angle);
			//_delay_ms(20);
		//}
		//for (angle = 600; angle <= 1800; angle += 20) {
			//PORTB |= 0x02;
			////pca9685_pwm(0, angle);
			////pca9685_pwm(1, angle);
			////pca9685_pwm(2, angle);
			////pca9685_pwm(3, angle);
			//pca9685_pwm(4, angle);
			//_delay_ms(100);
		//}
		//_delay_ms(50);
	//}
//}