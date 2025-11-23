# Chair_Bot
Control moving chair with your body


## Setting up ESP32 to enable bluetooth
Go to bluetooth_upload and load this code into the ESP32 with Board: DOIT ESP32 DEVKIT V1
Connect to the bluetooth through a computer. It will connect for a second and then disconnect but mantained paired.
Find the COM port used for the bluetooth connection in a Windows Computer it will be the same type of COM as an USB input.
Then you can talk to it in with code like this:

'''
import serial
import time
ser = serial.Serial("COM4", 115200)
time.sleep(2)
ser.write(b"hey")
ser.close()
'''
