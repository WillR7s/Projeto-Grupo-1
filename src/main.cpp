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

const int PINO_LED_RGB = 48;
const int pinoLampada = 2;
const int QTDS_LEDS = 1;
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
	atualizarLedRGB();
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

	bool temTemperatura = doc["temperatura"].is<JsonObject>();
	bool temLampada = doc["estadoLampada"].is<bool>();

	if (!temTemperatura && !temLampada)
	{
		debugErro("JSON precisa conter ao menos 'temperatura' ou 'estadoLampada'");
		return;
	}

	if (temTemperatura)
	{
		if (!doc["temperatura"]["valor"].is<float>())
		{
			debugErro("Temperatura precisa do campo 'valor' (float, em Celsius)");
			return;
		}

		float valorCelsius = doc["temperatura"]["valor"].as<float>();

		bool exibirEmCelsius = doc["temperatura"]["exibirEmCelsius"].is<bool>() ? doc["temperatura"]["exibirEmCelsius"].as<bool>() : true;

		debugInfo("Temperatura: " + String(valorCelsius) + " C");

		aplicarCorPorTemperatura(valorCelsius);

		// TODO Informar exibirEmCelsius para o LCD.
	}

	if (temLampada)
	{
		bool estadoLampada = doc["estadoLampada"].as<bool>();
		alterarEstadoLampada(estadoLampada);
	}
}