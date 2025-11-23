# Chair_Bot
Control a moving chair using your body.

## Setting up the ESP32 for Bluetooth

1. Open the **bluetooth_upload** folder and upload the code to your ESP32.  
   - **Board:** `DOIT ESP32 DEVKIT V1`

2. Pair the ESP32 with your computer via Bluetooth.  
   - It may briefly connect and disconnect, but it should remain paired.

3. On Windows, identify the **COM port** assigned to the Bluetooth connection.  
   - It will appear similar to a standard USB COM port.

4. You can communicate with the ESP32 using Python. Example:

   ```python
   import serial
   import time

   ser = serial.Serial("COM4", 115200)
   time.sleep(2)

   ser.write(b"hey")

   ser.close()

