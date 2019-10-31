#include <Arduino.h>
#include <TM1637Display.h>

/*
CLK is yellow
DIO is green
*/


#define SWITCH_PIN_1 8
#define FOCUS_PIN 13
#define SHUTTER_PIN 12
#define CLK 11
#define DIO 10
#define BRIGHTNESS 0x0f
#define TEST_DELAY 1000
#define DELAY_TIME 15000
#define INT_TIME 10000
#define NUM_PICS 4
#define SERIAL_ENABLE 0

TM1637Display display(CLK, DIO);

//initialize variables
int toggle=0, picsLeft = 0, time_left=0;
long target=0;
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 }, colon[] = { 0x00, 0x80, 0x00, 0x00 };

void setup() {
  //start serial connection
  if (SERIAL_ENABLE) Serial.begin(9600);
  //configure switch as an input and enable the internal pull-up resistor
  pinMode(SWITCH_PIN_1, INPUT_PULLUP);
  //configure outputs
  pinMode(FOCUS_PIN, OUTPUT);
  pinMode(SHUTTER_PIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DIO, OUTPUT);
  display.setBrightness(0x0f);
  
  display.setColon(true);
  display.showNumberDec(8888, false);
  delay(TEST_DELAY);
  display.setSegments(blank);
  
}

void loop() {
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(SWITCH_PIN_1);
  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (sensorVal == LOW && picsLeft == 0) {
    target = millis() + DELAY_TIME + 999;
    picsLeft = NUM_PICS;
    //colon[0] = display.encodeDigit(NUM_PICS - picsLeft + 1);
    display.setSegments(colon);
  }
  if (picsLeft > 0) {
    long countdown = target - millis();
    if (countdown < 3000) {
      digitalWrite(FOCUS_PIN, HIGH);
    }
    if (countdown < 0) {
      digitalWrite(SHUTTER_PIN, HIGH);
    }
    if (countdown < -1000) {
      picsLeft -= 1;
      digitalWrite(SHUTTER_PIN, LOW);
      digitalWrite(FOCUS_PIN, LOW);
      if (picsLeft > 0) {
        //colon[0] = display.encodeDigit(NUM_PICS - picsLeft + 1);
        countdown = INT_TIME + 999;
        target = millis() + countdown;
        display.setSegments(colon);
      }
    }
    
    if (countdown > 0) {
      time_left = int(countdown / 1000);
      if (SERIAL_ENABLE) Serial.println(time_left);
      display.showNumberDec(time_left, true, 2, 2);
    } else {
      if (SERIAL_ENABLE) Serial.println("");
      display.setSegments(blank);
    }
  }
}



