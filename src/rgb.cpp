#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "rgb.h"
#include "DebugManager.h"

//*===constantes===
extern const int PINO_LED_RGB;
extern const int QTDS_LEDS;

//*=====estaticas=====
static int corAtualR = 0;
static int corAtualG = 0;
static int corAtualB = 0;

static int corAlvoR = 255;
static int corAlvoG = 255;
static int corAlvoB = 255;

static ulong ultimoUpdate = 0;
const ulong INTERVALO_MS = 20;
const int VALOR_DIFERENCA_RGB = 5;

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

void aplicarCorPorTemperatura(float celsius)
{
    celsius = constrain(celsius, 40.0f, 100.0f);

    corAlvoR = map(celsius, 40, 100, 0, 255);
    corAlvoG = 255 - abs((int)map(celsius, 40, 100, -255, 255));
    corAlvoB = map(celsius, 40, 100, 255, 0);

    debugInfo("Novo alvo do LED: (R: " + String(corAlvoR) + ", G: " + String(corAlvoG) + ", B: " + String(corAlvoB) + ")");
}

void atualizarLedRGB()
{
    if (corAtualR == corAlvoR && corAtualG == corAlvoG && corAtualB == corAlvoB)
        return;

    if (millis() - ultimoUpdate < INTERVALO_MS)
        return;

    ultimoUpdate = millis();

    if (corAtualR < corAlvoR)
        corAtualR = min(corAtualR + VALOR_DIFERENCA_RGB, corAlvoR);
    else if (corAtualR > corAlvoR)
        corAtualR = max(corAtualR - VALOR_DIFERENCA_RGB, corAlvoR);

    if (corAtualG < corAlvoG)
        corAtualG = min(corAtualG + VALOR_DIFERENCA_RGB, corAlvoG);
    else if (corAtualG > corAlvoG)
        corAtualG = max(corAtualG - VALOR_DIFERENCA_RGB, corAlvoG);

    if (corAtualB < corAlvoB)
        corAtualB = min(corAtualB + VALOR_DIFERENCA_RGB, corAlvoB);
    else if (corAtualB > corAlvoB)
        corAtualB = max(corAtualB - VALOR_DIFERENCA_RGB, corAlvoB);

    alterarCorLedRGB(corAtualR, corAtualG, corAtualB);
}
