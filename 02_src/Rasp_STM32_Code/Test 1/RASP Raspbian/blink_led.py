import time
import serial

ser = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopnits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

while True:
    ser.write(str('h').encode())
    time.sleep(1)
    ser.write(str('n').encode())
    time.sleep(1)
