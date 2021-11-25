#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>

SFEMP3Shield MP3player;
SdFat sd;
SdFile file;

uint8_t cntPULSE;
uint8_t pinDIAL  = 5;
uint8_t pinPULSE = 4;

byte result;

void setup() {
  pinMode(pinDIAL, INPUT_PULLUP);
  pinMode(pinPULSE, INPUT_PULLUP);
  digitalWrite(pinDIAL,  HIGH);                       // Подтягиваем вывод pinDIAL к Vcc.
  digitalWrite(pinPULSE, HIGH);                       // Подтягиваем вывод pinPULSE к Vcc.

  Serial.begin(115200);

  result = sd.begin(SD_SEL, SPI_HALF_SPEED);
  if (result != 0) {
    Serial.print("Error code: ");
    Serial.print(result);
    Serial.println(" when trying to start SD");
  }

  result = MP3player.begin();
  if (result != 0) {
    Serial.print("Error code: ");
    Serial.print(result);
    Serial.println(" when trying to start MP3 player");
  }
}

void loop() {
  if (!digitalRead(pinDIAL)) {
    Serial.println("start reading");
    delay(20);
    cntPULSE = 0;
    while (!digitalRead(pinDIAL)) {
      if (digitalRead(pinPULSE)) {
        delay(5);
        while (digitalRead(pinPULSE)) {
          delay(5);
        }
        delay(5);
        cntPULSE++;
        
        if (cntPULSE > 100) {
          Serial.println("over 9000");
        }
      }
    }
    if (cntPULSE) {
      if (cntPULSE >= 10) {
        cntPULSE = 10; //dial 0
      }
    }

    Serial.println(cntPULSE);

    if (cntPULSE == 10) {
      Serial.println("stop track");
      MP3player.stopTrack();
      Serial.println("stop track2");
    } else {
      result = MP3player.playTrack(cntPULSE);

      if (result != 0) {
        Serial.print("Error code: ");
        Serial.print(result);
        Serial.println(" when trying to play track");
      }

      Serial.println("Playing:" + (String)cntPULSE);
    }
  }
}
