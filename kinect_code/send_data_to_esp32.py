import serial
import time

ser = serial.Serial("COM4", 115200)
time.sleep(2)
i=0
while i < 100:
    ser.write(b"L:1.0, R:1.0;")
    time.sleep(0.05)
    i = i + 1
ser.close()
