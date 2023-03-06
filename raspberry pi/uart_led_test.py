import serial

# Set up the serial port
ser = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)

num = -1

while (num != 0):

    num = int(input("1:ON 2:OFF 0:EXIT : "))

    if (num == 1):
        ser.write(b'h')
        print("h")
    elif (num == 2):
        ser.write(b'l')
        print("l")
    elif (num == 3):
        ser.write(b'f')
        print("f")
    elif (num == 0):
        print("OUT!")
        ser.close()