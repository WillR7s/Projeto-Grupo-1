/*Nome: Gael
Projeto: Wifi no ESP
Descrição: Integrar Wifi no ESP32
Data: 04/05
Versão: 3.0
*/
#include <Arduino.h>
#include <arduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"

// Constantes
const int Pino_led = 48;
const int Quantidade_leds = 1;
const char TOPICO_LED_RGB[] = "senai_gael_testar_led";
const int Pino_lampada = 2;

// Instancias
Adafruit_NeoPixel led_RGB(Quantidade_leds, Pino_led, NEO_GBR + NEO_KHZ800);
// Prototipagens
void lampada();
void configurar_led_rgb();
void tratar_mensagem_recebida(const char *topico, const String &mensagem);
void alterar_cor_do_led_rgb(int vermelho, int verde, int azul);
void tratar_JSON_Comando(const String &mensagem);
void setup()
{
  pinMode(Pino_lampada, OUTPUT);
  configurar_debug();
  conectarWifi();
  configurar_MQTT();
  registrar_Call_back_Mensagem(tratar_mensagem_recebida);
  conectar_MQTT();
  configurar_led_rgb();
}

void loop()
{
 
  garantir_Wifi_Conectado();
  garantir_MQTT_conectado();
  loop_MQTT();
}
void tratar_mensagem_recebida(const char *topico, const String &mensagem)
{
  debug_info("======================================");
  debug_info("Mensagem recebida na aplicação");
  debug_info("======================================");

  if (topico == nullptr)
  {
    debug_erro("Topico MQTT invalido");
    return;
  }

  debug_info("Topico: " + String(topico));
  debug_info("Mensagem: " + mensagem);

  if (strcmp(topico, TOPICO_LED_RGB) == 0)
  {
    tratar_JSON_Comando(mensagem);
    return;
  }

  debug_erro("Tópicos não tratados: " + String(topico));
}

void configurar_led_rgb()
{

  led_RGB.begin();
  led_RGB.setBrightness(80);
  led_RGB.clear();
  led_RGB.show();

  debug_info("Led RGB configurado no pino" + String(Pino_led));
}

void alterar_cor_do_led_rgb(int vermelho, int verde, int azul)
{
  vermelho = constrain(vermelho, 0, 255);
  verde = constrain(verde, 0, 255);
  azul = constrain(azul, 0, 255);

  led_RGB.setPixelColor(0, led_RGB.Color(vermelho, verde, azul));
  led_RGB.show();

  debug_info("Cor aplicada no led RGB");
  debug_info("R:" + String(vermelho));
  debug_info("G:" + String(verde));
  debug_info("B:" + String(azul));
}

void tratar_JSON_Comando(const String &mensagem)
{
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    debug_erro("Erro ao interpretar json");
    debug_erro(erro.c_str());
    return;
  }

  if (!(doc["led"].is<JsonObject>() || doc["lampada"].is<JsonObject>()))
  {

    debug_info("Não encontrado o comando para o LED RGB ou para a lampada");

    return;
  }
  else
  {
    if (!(doc["led"]["R"].is<int>() || !doc["led"]["G"].is<int>() || !doc["led"]["B"].is<int>() ||!doc["lampada"].is<bool>() ))
    {
      debug_erro("JSON invalido. Use led.R, led.G e led.B");
      return;
    }
    else
    {
      int vermelho = doc["led"]["R"].as<int>();
      int verde = doc["led"]["G"].as<int>();
      int azul = doc["led"]["B"].as<int>();
      alterar_cor_do_led_rgb(vermelho, verde, azul);
      bool trocar_estado = doc["lampada"].as<bool>();
      digitalWrite(Pino_lampada, trocar_estado);
    }
  }
}
