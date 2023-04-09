#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
TinyGPSPlus gps;
//SoftwareSerial ss(5, 18);

long timer = 0;
int Sen = 36;
int RLed = 32;
int GLed = 14;
int BLed = 27;


int countdown = 5;
int x = 0;

int micDetect = 0;
int accDetect = 0;

int SMStx = 0;

void setup() {
  pinMode(Sen, INPUT);
  pinMode(RLed, OUTPUT);
  pinMode(GLed, OUTPUT);
  pinMode(BLed, OUTPUT);
  //Serial.begin(9600);
  Serial.begin(115200); 
  //ss.begin(115200);
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");
  
}

void loop() {
  mpu.update();
  delay(1000);
  
  Serial.print(0);
  Serial.print(" ");
  Serial.print(analogRead(Sen));
  Serial.print(" ");
  Serial.println(4096);
  delay(100);
 
    if(analogRead(Sen) > 2500)
    {
      micDetect = 1;
      x++;
      delay(1000);
      
      //countdown = 10000;
    }

  if (mpu.getAccX() > 1 || mpu.getAccX() < -1) {
    Serial.println("Gyros detect");
    accDetect = 1;
    x++;
    delay(1000);
  }
   if (mpu.getAccY() > 1 || mpu.getAccY() < -1) {
    accDetect = 1;
    x++;
    delay(1000);
  }
  if (mpu.getAccZ() > 1.1 || mpu.getAccZ() < -1) {
    accDetect = 1;
    x++;
    delay(1000);
  }


  if(x >= 2)
  {
    Serial.println("katastrofirahme ;d");
    Serial.print(F("Location: ")); 
    if (gps.location.isValid())
    {
      Serial.print(gps.location.lat(), 6);
      Serial.print(F(","));
      Serial.print(gps.location.lng(), 6);
      Serial.println("");
    }
    else
    {
      Serial.print(F("INVALID"));
    }
    //todo red led
    digitalWrite(GLed, HIGH);
  }
  if(x == 0)
  {
    //digitalWrite(GLed, HIGH);
  }
  if(x == 1)
  {
    Serial.println("Edin priznak za katastrofa :X");
    //digitalWrite(BLed, HIGH);
    //digitalWrite(GLed, LOW);
  }
  
  if(countdown == 0)
  {
    x = 0;
  }
 if(millis() - timer > 1000){ // print data every second
    Serial.print(F("ACCELERO  X: "));Serial.print(mpu.getAccX());
    Serial.print("\tY: ");Serial.print(mpu.getAccY());
    Serial.print("\tZ: ");Serial.println(mpu.getAccZ());
    Serial.println(F("=====================================================\n"));
    timer = millis();
  }
  //countdown--;
}
