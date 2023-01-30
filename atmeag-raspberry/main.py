import serial
import time

ser = serial.Serial(port = '/dev/ttyAMA0',
                    baudrate = 9600,
                    timeout = 1)
num = -1

while (num != 0):

    num = int(input("1:ON 2:OFF 0:EXIT : "))

    if (num == 1):
        print("ON!")
        ser.write('h'.encode('utf-8'))
    elif (num == 2):
        print("OFF!")
        ser.write('l'.encode('utf-8'))
    elif (num == 0):
        print("OUT!")
        ser.close()
