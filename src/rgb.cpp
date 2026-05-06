#include <Arduino.h>
#include <ArduinoJson.h>
#include <AdaFruit_NeoPixel.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"

//*===constantes===
const int pinoLamp = 0;
const int pinoLedRGB = 0;
const int qntsLEDs = 0;
const char TOPICO_COMANDO[] = "";



//*===instancias===
Adafruit_NeoPixel ledRGB(qntsLEDs, pinoLedRGB, NEO_GRB + NEO_KHZ800);

void configurarLedRGB(){
  ledRGB.begin();
  ledRGB.setBrightness(80);
  ledRGB.clear();
  ledRGB.show();

  debugInfo("LED RGB configurado no pino " + String(pinoLedRGB));
}

void alterarCorLedRGB(int vermelho, int verde, int azul){

  vermelho = constrain(vermelho, 0, 255);
  verde = constrain(verde, 0, 255);
  azul = constrain(azul, 0, 255);

  ledRGB.setPixelColor(0, ledRGB.Color(vermelho, verde, azul));
  ledRGB.show();
  ledRGB.begin();

  debugInfo("Cor aplicada.");
  debugInfo("R: " + String(vermelho));
  debugInfo("G: " + String(verde));
  debugInfo("B: " + String(azul));

  ledRGB.Color(255, 0, 0);
  
  if (valorTemperaturaCelsius > 100 )
  {
      ledRGB.Color(255, 0, 0); //Alerta EXTREMO, super perigo
      //?tem q colocar o print pro lcd 
  }
  else if (valorTemperaturaCelsius > 90)
  {
      ledRGB.Color(255, 166, 0); //Ficando quente, começa a ficar em alerta
      //?tem q colocar o print pro lcd 
  }
  else if (valorTemperaturaCelsius > 85) 
  {
      ledRGB.Color(255, 255, 0);//tranquilo, mas ficando quente
      //?tem q colocar o print pro lcd 
  }
  
  else if (valorTemperaturaCelsius > 60)
  {
      ledRGB.Color(0, 255, 0); //super tranquilo, ideal
      //?tem q colocar o print pro lcd 
  }
  else if (valorTemperaturaCelsius > 50)
  {
      ledRGB.Color(0, 255, 187); // bom, mas ficando frio
      //?tem q colocar o print pro lcd 
  }
  else if (valorTemperaturaCelsius > 40)
  {
      ledRGB.Color(0, 132, 255);
      //?tem q colocar o print pro lcd 
  }
  else
  {
      ledRGB.Color(0, 0, 255); //Alerta EXTREMO, frio de mais, gelando
      //?tem q colocar o print pro lcd 
  }
}



