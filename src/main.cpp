/*Nome: Gael
Projeto: Wifi no ESP
Descrição: Integrar Wifi no ESP32
Data: 04/05
Versão: 3.0
*/
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"

const int PinoLedRGB = 48;
const int pinoLampada = 2;
const int QntLeds = 1;
const char TOPICO_COMANDO[] = "senai134-g1/esp32/comando";

Adafruit_NeoPixel ledRGB(QntLeds, PinoLedRGB, NEO_GRB + NEO_KHZ800);

void tratarMensagemRecebida(const char *topico, const String &mensagem);
void alterarLedRGB(int red, int green, int blue, int brightness = 30);
void configurarLedRGB();
void configurarLampada();
void tratarJsonTopico(const String &mensagem);
void modosLedRGB(int ledMode);
void alterarEstadoLampada(bool estado);

void setup()
{
	configurarDebug();
	conectarWifi();
	configurarMQTT();
	registrarCallBackMensagem(tratarMensagemRecebida);
	conectarMQTT();
	configurarLedRGB();
	configurarLampada();
}

void loop()
{
	garantirWiFiConectado();
	garantirMQTTconectado();
	loopMQTT();
}

void tratarMensagemRecebida(const char *topico, const String &mensagem)
{
	debugInfoSemLinha("==============================\n");
	debugInfoSemLinha("Mensagem recebida na aplicação\n");
	debugInfoSemLinha("==============================\n");

	if (topico == nullptr)
	{
		debugErro("Tópico MQTT inválido.");
		return;
	}

	debugInfo("Tópico: " + String(topico));
	debugInfo("Mensagem: " + mensagem);

	if (strcmp(topico, TOPICO_COMANDO) == 0)
	{
		tratarJsonTopico(mensagem);
		return;
	}

	debugErro("Tópico não tratado: " + String(topico));
}

void configurarLedRGB()
{
	ledRGB.begin();
	ledRGB.setBrightness(20);
	ledRGB.clear();
	ledRGB.show();

	debugInfo("LED RGB configurado no pino " + String(PinoLedRGB));
}

void configurarLampada()
{
	pinMode(pinoLampada, OUTPUT);
	
	debugInfo("Lampada configurado no pino " + String(pinoLampada));
}

void alterarLedRGB(int red, int green, int blue, int brightness)
{
	ledRGB.clear();

	ledRGB.setBrightness(brightness);

	red = constrain(red, 0, 255);
	green = constrain(green, 0, 255);
	blue = constrain(blue, 0, 255);

	ledRGB.setPixelColor(0, ledRGB.Color(red, green, blue));
	ledRGB.show();

	debugInfo("Cor aplicada: (R: " + String(red) + ", G: " + String(green) + ", B: " + String(blue) + ")");
}

void alterarEstadoLampada(bool estado)
{
	digitalWrite(pinoLampada, estado);
}

void tratarJsonTopico(const String &mensagem)
{
	JsonDocument doc;
	DeserializationError erro = deserializeJson(doc, mensagem);

	if (erro)
	{
		debugErro("Erro ao interpretar o JSON");
		debugErro(erro.c_str());
		return;
	}

	if (!doc["temperatura"].is<JsonObject>() || doc["estadoLampada"].is<JsonObject>())
	{
		debugErro("Não encontrado o comando para a temperatura ou o comando para o estado da Lampada");
		return;
	}
	else
	{
		if (!(doc["temperatura"]["valor"].is<float>() || doc["temperatura"]["exibirEmCelsius"].is<bool>() || doc["estadoLampada"].is<bool>()))
		{
			debugErro("Valores de temperatura ou do estado da lampada não encontrados");
			return;
		}
		else
		{
			float valor = doc["temperatura"]["valor"].as<float>();
			float exibirEmCelsius = doc["temperatura"]["exibirEmCelsius"].as<float>();
			bool estadoLampada = doc["estadoLampada"].as<bool>();
      int valorTemperaturaCelsius = 0;
       String unidadeDeMedida = "";

      digitalWrite(pinoLampada, estadoLampada);
      if(exibirEmCelsius)
      {
        valorTemperaturaCelsius = valor;
        unidadeDeMedida = "C";
      }
      else
      {
        valorTemperaturaCelsius = (valor - 32) * (5/9);
        unidadeDeMedida = "F";
      }
		}
	}
}