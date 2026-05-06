#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "rgb.h"
#include "DebugManager.h"

//*===constantes===
extern const int pinoLedRGB;
extern const int qntsLEDs;

//*===instancias===
Adafruit_NeoPixel ledRGB(qntsLEDs, pinoLedRGB, NEO_GRB + NEO_KHZ800);

void configurarLedRGB()
{
    ledRGB.begin();
    ledRGB.setBrightness(50);
    ledRGB.clear();
    ledRGB.show();

    debugInfo("LED RGB configurado no pino " + String(pinoLedRGB));
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
    if (celsius > 100)
        alterarCorLedRGB(255, 0, 0);
    else if (celsius > 90)
        alterarCorLedRGB(255, 166, 0);
    else if (celsius > 85)
        alterarCorLedRGB(255, 255, 0);
    else if (celsius > 60)
        alterarCorLedRGB(0, 255, 0);
    else if (celsius > 50)
        alterarCorLedRGB(0, 255, 187);
    else if (celsius > 40)
        alterarCorLedRGB(0, 132, 255);
    else
        alterarCorLedRGB(0, 0, 255);

    debugInfo("Temperatura aplicada ao LED: " + String(celsius) + " C");
}
