#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

float xG;
float yG;
float zG;

float prev = 9.81;

//Gyro Callibration variables
float gyro_prev_guessX = 0;
float gyro_prev_guessY = 0;
float gyro_prev_guessZ = 0;

float gyro_meas_biasX = 0;
float gyro_meas_biasY = 0;
float gyro_meas_biasZ = 0;

float gyro_biasX;
float gyro_biasY;
float gyro_biasZ;

//Accelerometer callibration variables
float acc_prev_guessX = -0.12;
float acc_prev_guessY = -0.2;
float acc_prev_guessZ = 0.5;

float acc_meas_biasX = 0;
float acc_meas_biasY = 0;
float acc_meas_biasZ = 0;

float acc_biasX;
float acc_biasY;
float acc_biasZ;

float acc_avgX = 0;
float acc_avgY = 0;
float acc_avgZ = 0;

float data[6];

int count = 0;

Adafruit_MPU6050 mpu;
void setup(void) {
  Serial.begin(19200);
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  delay(100);

  for(int i =0;i<1000;i++){
    Serial.println(i);
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);


    gyro_meas_biasX = g.gyro.x;
    gyro_meas_biasY = g.gyro.y;
    gyro_meas_biasZ = g.gyro.z;

    gyro_biasX = kalman(gyro_meas_biasX, gyro_prev_guessX);
    gyro_prev_guessX = gyro_biasX;

    gyro_biasY = kalman(gyro_meas_biasY, gyro_prev_guessY);
    gyro_prev_guessY = gyro_biasY;

    gyro_biasZ = kalman(gyro_meas_biasZ, gyro_prev_guessZ);
    gyro_prev_guessZ = gyro_biasZ;
  }

  delay(7000);

}
void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  data[0] = a.acceleration.x - acc_biasX;
  data[1] = a.acceleration.y - acc_biasY;
  data[2] = a.acceleration.z - acc_biasZ;
  data[3] = g.gyro.x - gyro_biasX;
  data[4] = g.gyro.y - gyro_biasY;
  data[5] = g.gyro.z - gyro_biasZ;

  if(count<101){
    acc_meas_biasX = data[0];
    acc_meas_biasY = data[1];
    acc_meas_biasZ = 9.81-data[2];

    acc_avgX = (acc_avgX + acc_meas_biasX)/2;
    acc_avgY = (acc_avgY + acc_meas_biasY)/2;
    acc_avgZ = (acc_avgZ + (9.81-acc_meas_biasZ))/2;

    acc_biasX = acc_avgX;
    acc_biasY = acc_avgY;
    acc_biasZ = acc_avgZ;
    count +=1;

    //Serial.println(acc_biasX);
  }



  //angle = rad2deg(get_angle(data[2]));



  Serial.print(data[0]);
  Serial.print("   :   ");
  Serial.print(data[1]);
  Serial.print("   :   ");
  Serial.println(data[2]);
  //Serial.println(angle);
  
}


float get_angle(float z){
  float angle;
  if(z<=9.81){
    angle = acos(z/9.81);
  }
  else{
    angle = 0;
  }
  return angle;
}

float rad2deg(float rad){
  return rad*(180/3.14);
}

float kalman(float meas, float prev_guess){
  float new_guess = prev_guess + 0.01*(meas-prev_guess);
  return new_guess;
}