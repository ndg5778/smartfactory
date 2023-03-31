import serial
import time

# Set up the serial port
ser = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)
time.sleep(1)

num = -1

while (num != 0):

    num = int(input("1:ON 2:OFF 0:EXIT : "))

    if (num == 1): 
        ser.flush()
        ser.write(b'h')
        print("h")
    elif (num == 2):
        ser.flush()
        ser.write(b'l')
        print("l")
    elif (num == 3):
        ser.flush()
        ser.write(b'f')
        print("f")
    elif (num == 0):
        print("OUT!")
        ser.close()