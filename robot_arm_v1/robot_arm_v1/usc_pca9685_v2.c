//#define F_CPU 16000000L
//#include <avr/io.h>
//#include <util/delay.h>
//#include <stdlib.h>
//
//#define PCA9685_ADDR 0x40
//#define PRESCALE 121 // corresponds to 50 Hz frequency
//#define SERVO_PIN 0
//
//void i2c_init() {
	//// initialize I2C bus at 100 kHz
	//TWSR = 0;
	//TWBR = ((F_CPU / 100000L) - 16) / 2;
//}
//
//void i2c_start() {
	//// send start condition
	//TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	//while (!(TWCR & (1<<TWINT)));
//}
//
//void i2c_write(uint8_t data) {
	//// send data byte
	//TWDR = data;
	//TWCR = (1<<TWINT) | (1<<TWEN);
	//while (!(TWCR & (1<<TWINT)));
//}
//
//void i2c_stop() {
	//// send stop condition
	//TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
//}
//
//void pca9685_init() {
	//// set prescale value
	//i2c_start();
	//i2c_write(PCA9685_ADDR << 1);
	//i2c_write(0xFE); // prescale register
	//i2c_write(PRESCALE);
	//i2c_stop();
	//_delay_ms(5);
	//
	//// set sleep mode to off
	//i2c_start();
	//i2c_write(PCA9685_ADDR << 1);
	//i2c_write(0x00); // mode1 register
	//i2c_write(0x01); // restart and auto-increment mode
	//i2c_stop();
	//_delay_ms(5);
//}
//
//void set_servo_angle(uint8_t angle) {
	//// convert angle to pulse width (1-2 ms)
	////uint16_t pulse_width = 500 + (angle * 11) / 10;
	//// set PWM signal on PCA9685
	//i2c_start();
	//i2c_write(PCA9685_ADDR << 1);
	//i2c_write(SERVO_PIN * 4 + 6); // LED0_ON_L register
	//i2c_write(0); // low byte of pulse width
	//i2c_write(0); // high byte of pulse width
	//i2c_write(0); // low byte of pulse width (repeat)
	//i2c_write(0); // high byte of pulse width (repeat)
	//i2c_stop();
//}
//
//int main(void) {
	//i2c_init();
	//pca9685_init();
	//// move servo to 0 degrees
	//set_servo_angle(0);
	//_delay_ms(1000);
	//// move servo to 90 degrees
	//set_servo_angle(90);
	//_delay_ms(1000);
	//// move servo to 180 degrees
	//set_servo_angle(180);
	//_delay_ms(1000);
	//while(1) {
		//// do nothing
	//}
//}
