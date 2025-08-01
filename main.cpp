#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/esp32/fs/sdmmc.h>
#include "DFRobot_ENS160.h"

#include "Wire.h"
 
#define SDA_PIN 12
#define SCL_PIN 13

using namespace eloq;
DFRobot_ENS160_I2C ens160(&Wire, 0x53);

void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.println("USELESS CAMMMM.........");
    Wire.begin(SDA_PIN, SCL_PIN); 

    while (!ens160.begin()) {
    Serial.println("ENS160 not detected. Check wiring.");
    delay(500);
   }
    Serial.println("ENS160 Initialized");

 //   ens160.setPowerMode(ENS160_STANDARD_MODE); // Activate standard air quality mode



    // camera settings
    // replace with your own model!
    camera.pinout.aithinker();
    camera.brownout.disable();
    camera.resolution.vga();
    camera.quality.high();

    // you can configure each pin of SDMMC (if needed)
    // (delete these lines if you're not sure)
    sdmmc.pinout.clk(39);
    sdmmc.pinout.cmd(38);
    sdmmc.pinout.d0(40);

    // init camera
    while (!camera.begin().isOk())
        Serial.println(camera.exception.toString());

    // init SD
    while (!sdmmc.begin().isOk())
        Serial.println(sdmmc.exception.toString());

    Serial.println("Camera OK");
    Serial.println("SD card OK");
    Serial.println("Enter 'capture' to capture a new picture");
}
int check ( ){
  
  
  uint16_t eco2 = ens160.getECO2();
  uint8_t aqi = ens160.getAQI();

 // Serial.print(" ppb | AQI: "); Serial.print(aqi); 
  
   if(eco2<800&&aqi==1){
    return 1;
  }
  else {
    return 0;
  }
}

void loop() {
    // await for "capture" from the Serial Monitor
  if(check()){
    // capture picture
    if (!camera.capture().isOk()) {
        Serial.println(camera.exception.toString());
        return;
    }

    // save under root folder
    if (sdmmc.save(camera.frame).to("", "jpg").isOk()) {
      Serial.print("File written to "); 
      Serial.println(sdmmc.session.lastFilename);
    }
    else Serial.println(sdmmc.session.exception.toString());

    // save under nested folder
    if (sdmmc.save(camera.frame).inside("myfolder").to("", "jpg").isOk()) {
      Serial.print("File written to ");
      Serial.println(sdmmc.session.lastFilename);
    }
    else Serial.println(sdmmc.session.exception.toString());

    // restart the loop
   // Serial.println("Enter 'capture' to capture a new picture");

}
else {
    Serial.println("air quality not met \n ");
    Serial.println("cannot take picture");


}
delay(2000);

}
