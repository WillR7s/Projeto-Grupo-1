/*Nome: Gael
Projeto: Wifi no ESP
Descrição: Integrar Wifi no ESP32
Data: 16/04
Versão: 1.0
*/
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "WiFiManeger.h"
void conectarWifi();
void garantir_Wifi_Conectado();
void setup() 
{
  WiFi.mode(WIFI_STA);
  Serial.begin(9600);
  conectarWifi();
}

void loop() 
{
 garantir_Wifi_Conectado();
}
