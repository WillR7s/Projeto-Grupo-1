#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "rgb.h"
#include "DebugManager.h"

//*===constantes===
const int PINO_LED_RGB = 48;
const int QTDS_LEDS = 1;

//*=====estaticas=====
static int corAtualR = 0;
static int corAtualG = 0;
static int corAtualB = 0;



static ulong ultimoUpdate = 0;
const ulong INTERVALO_MS = 20;
const int VALOR_DIFERENCA_RGB = 5;
float sensor = 0.0;

//*===instancias===
Adafruit_NeoPixel ledRGB(QTDS_LEDS, PINO_LED_RGB, NEO_GRB + NEO_KHZ800);

void configurarLedRGB()
{
    ledRGB.begin();
    ledRGB.setBrightness(50);
    ledRGB.clear();
    ledRGB.show();

    debugInfo("LED RGB configurado no pino " + String(PINO_LED_RGB));
}

void alterarCorLedRGB(int red, int green, int blue)
{

    red = constrain(red, 0, 255);
    green = constrain(green, 0, 255);
    blue = constrain(blue, 0, 255);

    ledRGB.setPixelColor(0, ledRGB.Color(red, green, blue));
    ledRGB.show();

    debugInfo("Cor aplicada: (R: " + String(red) + ", G: " + String(green) + ", B: " + String(blue) + ")");
}

void atualizarLedRGB(float celsius)
{
    celsius = constrain(celsius, 40.0f, 100.0f);

  if(sensor > 40.0 && sensor < 52.0)
  {
  corAtualB = 255;
  corAtualG = 0;
  corAtualR = 0;
   debugInfo("Novo alvo do LED: (R: " + String(corAtualR) + ", G: " + String(corAtualG) + ", B: " + String(corAtualB) + ")");
  }
  else if(sensor > 52 && sensor < 64)
  {
    corAtualB = 255;
    corAtualG = 255;
    corAtualR = 0;
     debugInfo("Novo alvo do LED: (R: " + String(corAtualR) + ", G: " + String(corAtualG) + ", B: " + String(corAtualB) + ")");
  }
  else if(sensor > 64 && sensor < 72)
  {
    corAtualB = 0;
    corAtualG = 255;
    corAtualR = 0;
     debugInfo("Novo alvo do LED: (R: " + String(corAtualR) + ", G: " + String(corAtualG) + ", B: " + String(corAtualB) + ")");
  }
  else if(sensor > 72 && sensor < 84)
  {
    corAtualB = 0;
    corAtualG = 255;
    corAtualR = 255;
     debugInfo("Novo alvo do LED: (R: " + String(corAtualR) + ", G: " + String(corAtualG) + ", B: " + String(corAtualB) + ")");
  }
  else if(sensor > 84 && sensor <= 100)
  {
    corAtualB = 0;
    corAtualG = 0;
    corAtualR = 255;
    debugInfo("Novo alvo do LED: (R: " + String(corAtualR) + ", G: " + String(corAtualG) + ", B: " + String(corAtualB) + ")");
  }

    alterarCorLedRGB(corAtualR, corAtualG, corAtualB);
}
