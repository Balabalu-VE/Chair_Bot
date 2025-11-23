#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String buffer = "";      // line buffer
bool BT_connected = false;

int left_speed = 0;
int right_speed = 0;
const int MAX_SPEED = 10;

bool ForwardR = true;
bool ForwardL = true;

#include <RoboClaw.h>

#define RX0 2  //These are the ESP32 Pins and the Motor Address
#define TX0 4
#define controller1 0x80
RoboClaw roboclaw1(&Serial1, 10000);   // This creates the RoboClaw serial comms

#define RX2 22
#define TX2 23
#define controller2 0x81
RoboClaw roboclaw2(&Serial2, 10000);

void setup() {
  Serial.begin(115200);          // use FAST serial!
  SerialBT.begin("ESP32-real");
  Serial.println("Bluetooth ready");
  Serial.begin(115200);   //Initiate usb serial for debug
  Serial1.begin(38400, SERIAL_8N1, RX0, TX0);
  roboclaw1.begin(38400); //set roboclaw baud rate
  Serial2.begin(38400, SERIAL_8N1, RX2, TX2);
  roboclaw2.begin(38400);
  roboclaw1.ForwardM1(controller1,0.0);
  roboclaw2.ForwardM2(controller2,0.0);
}

void parse_input(const String& input) {
  double L, R;

  int matched = sscanf(input.c_str(), "L:%lf, R:%lf", &L, &R);
  if (matched == 2) {
    ForwardL = (L >= 0);
    ForwardR = (R >= 0);

    L = abs(L);
    R = abs(R);

    left_speed  = int(L * 128 * (MAX_SPEED / 100.0));
    right_speed = int(R * 128 * (MAX_SPEED / 100.0));
  }
}

//Controller1 as right and Controller2 as Left
void loop() {
  while (SerialBT.available()) {
    BT_connected = true;
    char c = SerialBT.read();

    if (c == ';') {           // end of message
      buffer += c;
      parse_input(buffer);
    
      if(ForwardL){
        roboclaw2.ForwardM1(controller2,left_speed);
        roboclaw2.ForwardM2(controller2,left_speed);
      }else {
        roboclaw2.BackwardM1(controller2,left_speed);
        roboclaw2.BackwardM2(controller2,left_speed);
      }
      if(ForwardR){
        roboclaw2.ForwardM1(controller1,right_speed);
        roboclaw2.ForwardM2(controller1,right_speed);
      }else {
        roboclaw2.BackwardM1(controller1,right_speed);
        roboclaw2.BackwardM2(controller1,right_speed);
      }
      //Serial.println("Left: " + String(left_speed) + " " + String(ForwardL) +
      //                " Right: " + String(right_speed) + " " + String(ForwardR));
      // small delay for stability
      buffer = "";            // reset for next msg
    }
    else {
      buffer += c;
    }
  }
  
  
  delay(1);
}