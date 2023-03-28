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

print("Linear Velocity: s/z  Angular Valocity: a/e \n")

while True:
    COS = ''
    COS = input(':')
    if(type(COS)=='str'):
        ser.write(str(COS).encode())
    else:
        ser.write(str('').encode())
