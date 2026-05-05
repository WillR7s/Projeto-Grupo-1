#include "secrets.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "WiFiManager.h"
#include "DebugManager.h"
void conectarWifi()
{
  debug_info_sem_linha("\n");
  debug_info("===============================");
  debug_info("Iniciando uma conexão Wifi.....");
  debug_info("===============================");
  //Configura o ESP32 como station, ou seja ele vai se conectar a um roteador existente
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  debug_info_sem_linha("Conectando");
  int tentativas = 0;
  const int max_tentativas = 30;
  while(WiFi.status() !=WL_CONNECTED && tentativas < max_tentativas)
  {
    debug_info_sem_linha(".");
    delay(500);
    tentativas++;
  }
  debug_info_sem_linha("\n");
  if(WiFi.status() == WL_CONNECTED)
  {
    debug_info("Wifi conectado com sucesso!");
    debug_info_sem_linha("[INFO] Endereço Ip: ");
    debug_info_sem_linha(WiFi.localIP().toString());
    debug_info_sem_linha("\n");
  }
  else
  {
    debug_erro("Falha na conexão");
    debug_erro("Verifique o SSID, a senha e o sinal da rede.");
  }
}
void garantir_Wifi_Conectado()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    debug_info("Wifi desconectado, Tentando reconectar....");
    conectarWifi();
  }
  if(WiFi.status() != WL_CONNECTED)
  {
    debug_erro("Não foi possivel se conectar");
  }
}
bool wifi_esta_conectado()
{
    return WiFi.status() == WL_CONNECTED;
}