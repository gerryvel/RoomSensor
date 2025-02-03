#ifndef _SLEEP_H_
#define _SLEEP_H_

#include "driver/adc.h"
#include <WiFi.h>
#include "configuration.h"
#include <esp_bt.h>
#include <esp_wifi.h>

void ESPgoToSleep()
{
  Serial.println("Going to sleep...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  adc_power_off();
  esp_wifi_stop();
  esp_bt_controller_disable();

  // Configure the timer to wake us up!
  Serial.println("\nGo to Deep-Sleep-Mode for " + String(Sleeptime/1000000) + " seconds\n");
  esp_sleep_enable_timer_wakeup(Sleeptime); 
  
  // Go to sleep! Zzzz
  esp_deep_sleep_start();
}

#endif