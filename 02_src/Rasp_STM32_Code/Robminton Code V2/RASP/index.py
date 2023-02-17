import serial
import time
ser = serial.Serial(port='/dev/ttyACM0',baudrate = 9600, timeout=1)
print("In Program")
data = 'b'
while 1:
	x=str(input("Please Enter directions : "))
	if(x==str('f')):
		ser.write(str('f').encode())
	elif(x==str('b')):
		ser.write(str('b').encode())
	elif(x==str('s')):
                ser.write(str('s').encode())
	else:
                ser.write(str('s').encode())
