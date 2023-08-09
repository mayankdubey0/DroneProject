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
float acc_prev_guessX = 0.3;
float acc_prev_guessY = -0.2;
float acc_prev_guessZ = 1.5;

float acc_meas_biasX = 0;
float acc_meas_biasY = 0;
float acc_meas_biasZ = 0;

float acc_biasX;
float acc_biasY;
float acc_biasZ;

float acc_avgX = 0;
float acc_avgY = 0;
float acc_avgZ = 0;

bool cal = true;

float gyro_angleX;
float gyro_angleY;
float gyro_angleZ;

float data[6];

float time1 = millis()/1000;
float time2 = 0;
float elapsed_time = 0;

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

  callibrate_gyro();
}


void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  callibrate_acc(a, g, temp);

  data[0] = a.acceleration.x - acc_biasX;
  data[1] = a.acceleration.y - acc_biasY;
  data[2] = a.acceleration.z - acc_biasZ;
  data[3] = g.gyro.x - gyro_biasX;
  data[4] = g.gyro.y - gyro_biasY;
  data[5] = g.gyro.z - gyro_biasZ;

  time2 = millis()/1000;
  elapsed_time = time2 - time1;
  gyro_angleX = data[3] * elapsed_time;
  gyro_angleY = data[4] * elapsed_time;
  gyro_angleZ = data[5] * elapsed_time;
  time1 = time2;

 

  Serial.print(rad2deg(data[3]));
  Serial.print("/");
  Serial.print(rad2deg(data[4]));
  Serial.print("/");
  Serial.println(rad2deg(data[5]));

}

void callibrate_acc(sensors_event_t a, sensors_event_t g, sensors_event_t temp){
  mpu.getEvent(&a, &g, &temp);
  if(cal){
    acc_biasX = a.acceleration.x;
    acc_biasY = a.acceleration.y;
    acc_biasZ = a.acceleration.z - 9.81;
    cal = false;
  }
}

void callibrate_gyro(){
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
