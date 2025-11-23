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
    Serial.begin(115200);   //Initiate usb serial for debug
    Serial1.begin(38400, SERIAL_8N1, RX0, TX0);
    roboclaw1.begin(38400); //set roboclaw baud rate
    Serial2.begin(38400, SERIAL_8N1, RX2, TX2);
    roboclaw2.begin(38400);
}

void loop() {

    roboclaw1.ForwardM1(controller1, 64);
    //Serial.println("forward motor 1");

    roboclaw1.ForwardM2(controller1, 64);
    //Serial.println("forward motor 2");

    roboclaw2.ForwardM1(controller2, 64);
    //Serial.println("forward motor 3");

    roboclaw2.ForwardM2(controller2, 64);
    //Serial.println("forward motor 4");
}