#ifndef RGB_H
#define RGB_H

#include <Arduino.h>

void configurarLedRGB();
void alterarCorLedRGB(int red, int green, int blue);
void aplicarCorPorTemperatura(float celsius);
void atualizarLedRGB();

#endif