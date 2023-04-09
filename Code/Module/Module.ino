#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include "Wire.h"
#include <MPU6050_light.h>

//trust
//500 promenlivi
#define WIFI_SSID "A1_BDF6"
#define WIFI_PASSWORD "48575443203A96AA"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "noreplyxoblack@gmail.com"
#define AUTHOR_PASSWORD "aqghxsqozgzefmex"

/* Recipient's email*/
#define RECIPIENT_EMAIL "nikiyord1@gmail.com"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

//trust

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

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

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

  //aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  ESP_Mail_Session session;

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = "ESP";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "ESP Test Email";
  message.addRecipient("Sara", RECIPIENT_EMAIL);

  /*Send HTML message*/
  String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP board</p></div>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  /*
  //Send raw text message
  String textMsg = "Hello World! - Sent from ESP board";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;*/

  /* Set the custom message header */
  //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
  
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
      Serial.println("Mic detect");
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
    Serial.println("Gyros detect");
    accDetect = 1;
    x++;
    delay(1000);
  }
  if (mpu.getAccZ() > 1.1 || mpu.getAccZ() < -1) {
    Serial.println("Gyros detect");
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

void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");
  }
}
