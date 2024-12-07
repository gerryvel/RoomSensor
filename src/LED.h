#include "arduino.h"
#include "task.h"
#include <FastLED.h>

//Configuration LED
//const int LEDBoard = 2;  //DevModule
//const int LEDBoard = 13;   //Adafruit Huzzah32

/*
enum LED {Red = 25, Green = 26, Blue = 33};


void LEDblink(int PIN = LEDboard()){
 taskBegin();
   while(1)   // blockiert dank der TaskPause nicht 
   {
      digitalWrite(PIN,HIGH);  // LED ein
      taskPause(5);   // gibt Rechenzeit ab         
      digitalWrite(PIN,LOW);   // LED aus
      taskPause(1000);   // gibt Rechenzeit ab         
   }
   taskEnd();   
}

void LEDflash(int PIN = LED()){
   taskBegin();
   while(1)   // blockiert dank der TaskPause nicht 
   {
      digitalWrite(PIN,HIGH);  // LED ein
      taskPause(5);   // gibt Rechenzeit ab    
      digitalWrite(PIN,LOW);   // LED aus
      taskPause(250);   // gibt Rechenzeit ab    
   }
   taskEnd();   
}

// Flash LED for x ms
void flashLED(int PIN = LED(), int duration = 0){
 digitalWrite(PIN, HIGH); 
 delay(duration);
 digitalWrite(PIN, LOW);
}

void LEDInit() {                        // Start Initialisierung
  pinMode(LED(Red),   OUTPUT);
  pinMode(LED(Blue),  OUTPUT);
  pinMode(LED(Green), OUTPUT);
  digitalWrite(LED(Red), HIGH);
  delay(250);
  digitalWrite(LED(Red), LOW);
  digitalWrite(LED(Blue), HIGH);
  delay(250);
  digitalWrite(LED(Blue), LOW);
  digitalWrite(LED(Green), HIGH);
  delay(250);
  digitalWrite(LED(Green), LOW);
}

void LEDon(int PIN = LED()) {
  digitalWrite(PIN, HIGH);    
}

void LEDoff(int PIN = LED()) {
  digitalWrite(PIN, LOW);
}

void LEDoff_RGB() {
  digitalWrite(LED(Blue), LOW);
  digitalWrite(LED(Green),LOW);
  digitalWrite(LED(Red), LOW);
}
*/

CRGB leds[1];

enum LEDcolor {
  Red = 1, 
  Green = 2, 
  Blue = 3,
  White = 4,
  Black = 5
  };

void LEDboard(int color = LEDcolor())
{
  if (color = 1){
    leds[0] = CRGB::Red;    
    FastLED.show();
  }
  if (color = 2){
    leds[0] = CRGB::Green;
    FastLED.show();
  }
  if (color = 3){
    leds[0] = CRGB::Blue;
    FastLED.show();
  }
  if (color = 4){
    leds[0] = CRGB::Black;
    FastLED.show();
  }
if (color = 5){
    leds[0] = CRGB::White;
    FastLED.show();
  }
}

void LEDinitBoard()
{
  LEDboard(Red);
  delay(250);
  LEDboard(Blue);
  delay(250);
  LEDboard(Green);
  delay(250);
  LEDboard(White);
  delay(250);
  LEDboard(Black);
  delay(250);
}



