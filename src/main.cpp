/*Nome: Gael
Projeto: Wifi no ESP
Descrição: Integrar Wifi no ESP32
Data: 30/04
Versão: 2.0
*/
#include <Arduino.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"

void tratar_mensagem_recebida(const char * topico, const String& mensagem);
void setup() 
{
  configurar_debug();
  Serial.print("aaaaaa");
  conectarWifi();
  configurar_MQTT();
  registrar_Call_back_Mensagem(tratar_mensagem_recebida);
  conectar_MQTT();
}

void loop() 
{
 garantir_Wifi_Conectado();
 garantir_MQTT_conectado();
 loop_MQTT();
}
void tratar_mensagem_recebida(const char * topico, const String& mensagem)
{

}