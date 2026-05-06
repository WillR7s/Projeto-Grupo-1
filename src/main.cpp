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
#include "rgb.h"

const int pinoLedRGB = 48;
const int pinoLampada = 2;
const int qntsLEDs = 1;
const char TOPICO_COMANDO[] = "senai134-g1/esp32/comando";

void tratarMensagemRecebida(const char *topico, const String &mensagem);
void configurarLampada();
void tratarJsonTopico(const String &mensagem);
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

void configurarLampada()
{
	pinMode(pinoLampada, OUTPUT);
	
	debugInfo("Lampada configurado no pino " + String(pinoLampada));
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

	if (!doc["led"].is<JsonObject>() || doc["lampada"].is<JsonObject>())
	{
		debugErro("Não encontrado o comando para o LED RGB ou o comando para Lampada");
		return;
	}
	else
	{
		if (!(doc["led"]["color"]["r"].is<int>() || doc["led"]["color"]["g"].is<int>() || doc["led"]["color"]["b"].is<int>() || doc["lampada"].is<bool>()))
		{
			debugErro("Valores RGB não encontrados");
			return;
		}
		else
		{
			int red = doc["led"]["color"]["r"].as<int>();
			int green = doc["led"]["color"]["g"].as<int>();
			int blue = doc["led"]["color"]["b"].as<int>();

			int brightness = doc["led"]["brightness"].is<int>() ? doc["led"]["brightness"].as<int>() : 30;

			int estado = doc["lampada"].as<bool>();

			alterarCorLedRGB(red, green, blue);
			alterarEstadoLampada(estado);
		}
	}
}