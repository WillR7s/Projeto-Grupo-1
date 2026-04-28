#include "secrets.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "WiFiManager.h"
void conectarWifi()
{
  Serial.println();
  Serial.println("===============================");
  Serial.println("Iniciando uma conexão Wifi.....");
  Serial.println("===============================");
  //Configura o ESP32 como station, ou seja ele vai se conectar a um roteador existente
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  Serial.print("Conectando");
  int tentativas = 0;
  const int max_tentativas = 30;
  while(WiFi.status() !=WL_CONNECTED && tentativas < max_tentativas)
  {
    Serial.print(".");
    delay(500);
    tentativas++;
  }
  Serial.println();
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Wifi conectado com sucesso!");
    Serial.print("Endereço Ip: ");
    Serial.print(WiFi.localIP());
  }
  else
  {
    Serial.println("Falha na conexão");
    Serial.println("Verifique o SSID, a senha e o sinal da rede.");
  }
}
void garantir_Wifi_Conectado()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi desconectado, Tentando reconectar....");
    conectarWifi();
  }
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Não foi possivel se conectar");
  }
}
bool wifi_esta_conectado()
{
    return WiFi.status() == WL_CONNECTED;
}