#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String buffer = "";      // line buffer
bool BT_connected = false;

int left_speed = 0;
int right_speed = 0;
const double MAX_SPEED = 10.0;

bool ForwardR = true;
bool ForwardL = true;

void setup() {
  Serial.begin(115200);          // use FAST serial!
  SerialBT.begin("ESP32test-pip");
  Serial.println("Bluetooth ready");
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

void loop() {

  while (SerialBT.available()) {
    BT_connected = true;
    char c = SerialBT.read();

    if (c == ';') {           // end of message
      buffer += c;
      parse_input(buffer);
      Serial.println("Left: " + String(left_speed) + " " + String(ForwardL) +
                     " Right: " + String(right_speed) + " " + String(ForwardR));

      buffer = "";            // reset for next msg
    }
    else {
      buffer += c;
    }
  }

  // small delay for stability
  delay(1);
}
