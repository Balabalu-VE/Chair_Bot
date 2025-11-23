  #include <Bluepad32.h>
  #include <RoboClaw.h>

  int left_speed = 0;
  int right_speed = 0;
  const int MAX_SPEED = 50;

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

  const uint8_t maxGamepads = 1;
  GamepadPtr myGamepads[maxGamepads];

  //Values for left joystick min, max, and idle values for mapping change based on controller or maybe run setup sequence idk
  const int leftAxisXLow = -512;
  const int leftAxisXHigh = 512;
  const int leftAxisYLow = -512;
  const int leftAxisYHigh = 512;
  //const int leftAxisXIdle = 0;
  //const int leftAxisYIdle = 0;

  //Values for right joystick idle values mapping change based on controller or maybe run setup sequence idk
  const int rightAxisXLow = -512;
  const int rightAxisXHigh = 512;
  const int rightAxisYLow = -512;
  const int rightAxisYHigh = 512;
  const int rightAxisXIdle = 0;
  const int rightAxisYIdle = 0;

  const int freq= 2000;
  const int pwmChannelA = 0;
  const int pwmChannelB = 1;
  const int resolution = 8;

  // This callback gets called any time a new gamepad is connected.
  // Up to 4 gamepads can be connected at the same time.
  void onConnectedGamepad(GamepadPtr gp) {
    bool foundEmptySlot = false;
    for (int i = 0; i < maxGamepads; i++) {
      if (myGamepads[i] == nullptr) {
        Serial.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
        // Additionally, you can get certain gamepad properties like:
        // Model, VID, PID, BTAddr, flags, etc.
        GamepadProperties properties = gp->getProperties();
        Serial.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n",
                      gp->getModelName().c_str(), properties.vendor_id,
                      properties.product_id);
        myGamepads[i] = gp;
        foundEmptySlot = true;
        break;
      }
    }
    if (!foundEmptySlot) {
      Serial.println(
          "CALLBACK: Gamepad connected, but could not found empty slot");
    }
  }


  void onDisconnectedGamepad(GamepadPtr gp) {
    bool foundGamepad = false;

    for (int i = 0; i < maxGamepads; i++) {
      if (myGamepads[i] == gp) {
        Serial.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
        myGamepads[i] = nullptr;
        foundGamepad = true;
        break;
      }
    }

    if (!foundGamepad) {
      Serial.println(
          "CALLBACK: Gamepad disconnected, but not found in myGamepads");
    }
  }

  //View Gamepad State (axis, button press, etc.)
  void GamepadState() {
    for (int i = 0; i < maxGamepads; i++) {
      GamepadPtr myGamepad = myGamepads[i];
        if (myGamepad && myGamepad->isConnected()) {
        Serial.printf(
                "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: "
                "%4d, %4d, brake: %4d, throttle: %4d, misc: 0x%02x, gyro x:%6d y:%6d "
                "z:%6d, accel x:%6d y:%6d z:%6d\n",
                i,                        // Gamepad Index
                myGamepad->dpad(),        // DPAD
                myGamepad->buttons(),     // bitmask of pressed buttons
                myGamepad->axisX(),       // (-511 - 512) left X Axis
                myGamepad->axisY(),       // (-511 - 512) left Y axis
                myGamepad->axisRX(),      // (-511 - 512) right X axis
                myGamepad->axisRY(),      // (-511 - 512) right Y axis
                myGamepad->brake(),       // (0 - 1023): brake button
                myGamepad->throttle(),    // (0 - 1023): throttle (AKA gas) button
                myGamepad->miscButtons(), // bitmak of pressed "misc" buttons
                myGamepad->gyroX(),       // Gyro X
                myGamepad->gyroY(),       // Gyro Y
                myGamepad->gyroZ(),       // Gyro Z
                myGamepad->accelX(),      // Accelerometer X
                myGamepad->accelY(),      // Accelerometer Y
                myGamepad->accelZ()       // Accelerometer Z
            );
          }
      }
  }
  //maps Gamepad Values to Motor Output
  void GamepadMapping() {
    GamepadPtr myGamepad = myGamepads[0];  // Since maxGamepads = 1

    if (myGamepad && myGamepad->isConnected()) {
      int axisY = myGamepad->axisY();
      int axisRY = myGamepad->axisRY();
      if (axisY < -60) {
        //Forward Left
        int left_speed = map(axisY, leftAxisYLow, -60, 128, 30);
        Serial.println(left_speed);
        roboclaw2.BackwardM1(controller2,left_speed);
        roboclaw2.ForwardM2(controller2,left_speed);
        Serial.println("UP Left");
      } 
      else if (axisY > 60) {
        //Reverse Left
        int left_speed = map(axisY, 60, leftAxisYHigh, 30, 128);
        Serial.println(left_speed);
        roboclaw2.ForwardM1(controller2,left_speed);
        roboclaw2.BackwardM2(controller2,left_speed);
        Serial.println("DOWN Left");
      }
      else{
        roboclaw2.ForwardM1(controller2,0);
        roboclaw2.BackwardM2(controller2,0);
      } 
      if (axisRY > 60) {
        //Reverse Right
        int right_speed = map(axisRY, 60, rightAxisYHigh, 30, 128);
        roboclaw1.ForwardM1(controller1,right_speed);
        roboclaw1.ForwardM2(controller1,right_speed);
        Serial.println("DOWN Right");
      } 
      else if (axisRY < -60) {
        int right_speed = map(axisRY, rightAxisYLow, -60, 128, 30);
        roboclaw1.BackwardM1(controller1,right_speed);
        roboclaw1.BackwardM2(controller1,right_speed);
        Serial.println("UP Right");
      }
      else {
        // Stop motor
        roboclaw1.BackwardM1(controller1,0);
        roboclaw1.BackwardM2(controller1,0);
      }
    }
  }
    void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200); 
    Serial1.begin(38400, SERIAL_8N1, RX0, TX0);
    roboclaw1.begin(38400); //set roboclaw baud rate
    Serial2.begin(38400, SERIAL_8N1, RX2, TX2);
    roboclaw2.begin(38400);
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t *addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2],
                  addr[3], addr[4], addr[5]);

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    BP32.forgetBluetoothKeys();
  }

  unsigned long lastPrint = 0;

  void loop() {
    BP32.update();
    GamepadMapping();
    //GamepadState();
    //if (millis() - lastPrint > 500) {
      //GamepadState();
      //lastPrint = millis();
    //}
  }