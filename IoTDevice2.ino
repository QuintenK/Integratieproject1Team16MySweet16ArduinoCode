#include <SPI.h>
#include <LoRa.h>
#define BUTTON2 5
#define LED2 4
#define BUTTON1 A3
#define LED1 A5
#define ioTProj 1 // id of the IoTProject
#define keyWord "<ms16>" //lora-topic
#define interval 60000 //delay between votes, default 60000ms = 1min
#define sendTime 3600000 //votes will be sent every x millseconds, default 3600000ms = 1hr
bool on;
long lastPressed;
int count1;
int count2;
int buttonValue1;
int buttonValue2;
int unpressed1;
int unpressed2;
int hours;

//blink led x times
void blink(int x, int led) {
  for (int i = 0 ; i < x ; i++)
  {
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(400);
  }
}

//send all current votes to LoRaWAN, clear all votes
void sendVotes() {
  Serial.println("Sending");
  int status = LoRa.beginPacket();
  if (status) {
    LoRa.write(keyWord);
    LoRa.write("-");
    LoRa.write(ioTProj);
    LoRa.write("-");
    LoRa.write(count1);
    LoRa.write("-");
    LoRa.write(count2);
    LoRa.endPacket();
    Serial.println("sent");
    count1 = 0;
    count2 = 0;
  } else {
    Serial.println("Error sending");
    blink(10, LED1);
  }
}

//main running loop
void run() {
  buttonValue1 = digitalRead(BUTTON1);
  buttonValue2 = digitalRead(BUTTON2);
  if (buttonValue1 && !unpressed1) {
    Serial.println("button 1 pressed");
    if ((millis() - lastPressed) > interval) {
      Serial.println("succesfully");
      Serial.println(millis());
      Serial.println(lastPressed);
      Serial.println(interval);
      Serial.println(count1);
      blink(3, LED1);
      count1++;
      lastPressed = millis();
    } else {
      Serial.println("but the timer was at");
      Serial.println(millis());
      Serial.println(lastPressed);
      Serial.println(interval);
      Serial.println(count1);
      blink(1, LED1);
      if(lastPressed+10000<=millis()){lastPressed += 10000;}
    }
  }
  if (buttonValue2 && !unpressed2) {
    Serial.println("button 2 pressed");
    if ((millis() - lastPressed) > interval) {
      Serial.println("succesfully");
      Serial.println(millis());
      Serial.println(lastPressed);
      Serial.println(interval);
      blink(3, LED2);
      count2++;
      lastPressed = millis();
    } else {
      Serial.println("but the timer was at");
      Serial.println(millis());
      Serial.println(lastPressed);
      Serial.println(interval);
      blink(1, LED2);
      if(lastPressed+10000<=millis()){lastPressed += 10000;}
    }
  }
  unpressed1 = buttonValue1;
  unpressed2 = buttonValue2;
  if ((hours * sendTime) <= millis()) {
    hours++;
    if(count1 > 0 || count2 > 0){
    sendVotes();
    }
  }
}

void initialise() {
  Serial.begin(9600);
  Serial.println("Setup LoRa shield...");
  if (!LoRa.begin(868100000)) {
    Serial.println("Starting LoRa failed!");
    while (1) {
      blink(1, LED1);
      blink(1, LED2);
    }
  }
  LoRa.setSyncWord(0x34);
  Serial.println("LoRa shield initialized");
  pinMode(BUTTON1, INPUT); //main button
  pinMode(BUTTON2, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  buttonValue1 = 0;
  buttonValue2 = 0;
  unpressed1 = 0;
  unpressed2 = 0;
  hours = 1;
  count1 = 0;
  count2 = 0;
  on = false;
  lastPressed = 0;
}

void setup() {
  initialise();
  blink(2, LED1);
  blink(2, LED2);
}

void loop() {
  blink(1, LED1);
  blink(1, LED2);
  while (on) {
    run();
  }
  if (digitalRead(BUTTON1) || digitalRead(BUTTON2)) {
    on = true;
    Serial.println("started running");
  }
}
