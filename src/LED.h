#include "arduino.h"
#include "task.h"

enum LEDcolor {
  Red = 1, 
  Green = 2, 
  Blue = 3,
  White = 4,
  Clear = 5
  };

void LEDboard(int color = LEDcolor())
{
  switch (color){
  case 1:
    neopixelWrite(LED_BUILTIN,20,0,0);
    break;
  case 2:
    neopixelWrite(LED_BUILTIN,0,20,0);
    break;
  case 3:  
    neopixelWrite(LED_BUILTIN,0,0,20);
    break;
  case 4:
    neopixelWrite(LED_BUILTIN,20,20,20);
    break;
  case 5:
    neopixelWrite(LED_BUILTIN,0,0,0);
    break; 
  default:
    neopixelWrite(LED_BUILTIN,0,0,0);
    break;
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
  LEDboard(Clear);
}

// Flash LED for x ms
void flashLED(int color = LEDcolor(), int duration = 0){
 LEDboard(color); 
 delay(duration);
 LEDboard(Clear);
}

void LEDflash(int color = LEDcolor()){
   taskBegin();
   while(1)   // blockiert dank der TaskPause nicht 
   {
      LEDboard(color);  // LED ein
      taskPause(5);   // gibt Rechenzeit ab    
      LEDboard(Clear);   // LED aus
      taskPause(3000);   // gibt Rechenzeit ab    
   }
   taskEnd(); 
}
