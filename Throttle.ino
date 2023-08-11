#include "ppm.h"

// PPM channel layout (update for your situation)
#define THROTTLE        2
#define ROLL            4
#define PITCH           3
#define YAW             1
#define SWITCH3WAY_1    5
#define BUTTON          6
#define SWITCH3WAY_2    7     // trim-pot for left/right motor mix  (face trim)
#define POT             8     // trim-pot on the (front left edge trim)

// Loop interval time
const long interval = 50;
unsigned long previousMillis = 0;

void setup()
{
  // Start the serial port to display data 
  Serial.begin(115200);

  // Start the PPM function on PIN A0
  ppm.begin(A0, false);
  pinMode(13, OUTPUT);
}

void loop()
{
  // Interval at which the PPM is updated
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Acquiring all the channels values
    short throttle      =   (-ppm.read_channel(THROTTLE)+1500)/2;
    short roll          =   (-ppm.read_channel(ROLL)+1500)/2;
    short pitch         =   (-ppm.read_channel(PITCH)+1500)/2;
    short yaw           =   (-ppm.read_channel(YAW)+1500)/2;

    // Print the values for the Arduino Serial Plotter
    Serial.print("Throttle:");        Serial.print(throttle);       Serial.print(" ");
    Serial.print("Roll:");            Serial.print(roll);           Serial.print(" ");
    Serial.print("Pitch:");           Serial.print(pitch);          Serial.print(" ");
    Serial.print("Yaw:");             Serial.print(yaw);            Serial.print(" ");
  
    Serial.println();
  }
}
