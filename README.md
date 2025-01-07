# Room Sensor Modul

The ESP32 in this project is an ESP32C3. This is a small module with USB connector.
The Sensors is a BME280 for Temperatur, pressure and humidity.
It's Webinterface and Modbus available. 
The Modbus-server has 6 Register:
- temperatur
- pressure
- humidity
- altitude
- voltage (batterie)
- Set "True" from client: enable Deep-Sleep-Modus for xx min

The 12 Volt is reduced 10-26 Volt to 5 Volt with a DC Step-Down_Converter for connect to wired power. 
Also is a Batterie inserted in the housing and measurement for this voltage.
After configuration with the Webinterface can you with modbus switch to Deepsleep-Modus.

The Website use LittleFS Filesystem. You must use Partition Schemes "Minimal SPIFFS with APPS and OTA".
The HTML Data upload separately.

Modbus is tested with PLC S7-1214 TCP Client and KTP400

# Partlist:

- Espressiv ESP32-C3-DevkitM1 [Link](https://www.reichelt.de/entwicklungsboard-esp32-c3-mini-1-esp32c3devkitm-p311730.html?&nbc=1)
- BME 280 [Link](https://www.reichelt.de/entwicklerboards-temperatur-feuchtigkeits-und-drucksensor--debo-bme280-p253982.html?&nbc=1)
- Traco-Power TSR 1-2450 for 12V / 5V [Link](https://www.reichelt.de/dc-dc-wandler-tsr-1-1-w-5-v-1000-ma-sil-to-220-tsr-1-2450-p116850.html?search=tsr+1-24)
- Housing [Link](https://www.reichelt.de/sensorgehaeuse-74x74x25-5-mm-belueftet-weiss-cb-rs02vwh-p317699.html?&nbc=1)
- Batterieholder [Link](https://www.reichelt.de/batteriehalter-fuer-1-18350-keystone-1095p-p213366.html?&nbc=1)
- Akku [Link](https://www.reichelt.de/li-ion-akku-cr123-3-6-v-850-mah-usb-c-xcell-148203-p366682.html?&nbc=1)

# Wiring diagram

![grafik](https://github.com/user-attachments/assets/d2bd1a8f-84b5-4373-a100-b24541ba1129)

# PCB and Housing assembly

![grafik](https://github.com/user-attachments/assets/29a6c316-a486-42a8-855a-6aeb1b7e20d0)

PCB by Aisler [Link](https://aisler.net/p/JVQKQVUK)

# Webinterface

<img width="1080" alt="image" src="https://github.com/user-attachments/assets/b9d54b22-bd9d-4a3b-9983-1489b16eb7a9" />
<img width="1080" alt="image" src="https://github.com/user-attachments/assets/76e7ed92-e2f1-4aca-aae2-e641b5249f7e" />
<img width="1080" alt="image" src="https://github.com/user-attachments/assets/a76b5d52-b510-472e-954c-a52bf90fcb06" />
<img width="1080" alt="image" src="https://github.com/user-attachments/assets/5fa71013-38e1-4a74-ab89-2a668cdcb6f8" />

# S7 Connection

![grafik](https://github.com/user-attachments/assets/8d18e7bc-5492-497c-9e7f-ebbfbf8c76eb)
![grafik](https://github.com/user-attachments/assets/ee474e9d-3117-4803-ad7e-69706605e85a)



# Versions

- 1.2 add Modbusregister to switch DeepSleepModus on and WiFiScan off
- 1.1 add batterie voltage measurement
- 1.0 working Version

