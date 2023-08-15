#include <Servo.h>
#include "ppm.h"

// PPM channel layout (update for your situation)
#define THROTTLE        3
#define ROLL            1
#define PITCH           2
#define YAW             4
#define SWITCH3WAY_1    5
#define BUTTON          6
#define SWITCH3WAY_2    7     // trim-pot for left/right motor mix  (face trim)
#define POT             8     // trim-pot on the (front left edge trim)

// Loop interval time
const long interval = 50;
unsigned long previousMillis = 0;

bool callib = true;

Servo ESC1;
Servo ESC2;

void setup()
{
  // Start the serial port to display data 
  Serial.begin(115200);
  ESC1.attach(9, 1000, 2000);
  ESC2.attach(10, 1000, 2000);

  ppm.begin(A0, false);

  // Start the PPM function on PIN A0

}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Acquiring all the channels values
    short throttle      =   (-ppm.read_channel(THROTTLE)+1500)/2;
    short roll          =   (-ppm.read_channel(ROLL)+1500)/2;
    short pitch         =   (-ppm.read_channel(PITCH)+1500)/2;
    short yaw           =   (-ppm.read_channel(YAW)+1500)/2;

    ESC1.write(throttle);
    ESC2.write(throttle);
    Serial.print("Throttle:");        Serial.print(throttle);       Serial.print(" ");
    Serial.print("Roll:");            Serial.print(roll);           Serial.print(" ");
    Serial.print("Pitch:");           Serial.print(pitch);          Serial.print(" ");
    Serial.print("Yaw:");             Serial.print(yaw);            Serial.print("\n");
  }
}
