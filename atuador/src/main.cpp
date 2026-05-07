#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"
#include "rgb.h"

const int pinoLampada = 2;
const char TOPICO_RECEBER[] = "senai134/g1/esp32/comando";

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

	if (strcmp(topico, TOPICO_RECEBER) == 0)
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

	bool temTemperatura = doc["temperatura"].is<float>();
	bool temLampada = doc["estadoLampada"].is<bool>();

	if (!temTemperatura && !temLampada)
	{
		debugErro("JSON precisa conter ao menos 'temperatura' ou 'estadoLampada'");
		return;
	}

	JsonDocument respostaDoc;

	if (temTemperatura)
	{
		float valorCelsius = doc["temperatura"].as<float>();

		debugInfo("Temperatura: " + String(valorCelsius) + " C");

		atualizarLedRGB(valorCelsius);
		
		respostaDoc["temperatura"] = valorCelsius;
	}

	if (temLampada)
	{
		bool estadoLampada = doc["estadoLampada"].as<bool>();
		respostaDoc["estadoLampada"] = estadoLampada;
		alterarEstadoLampada(estadoLampada);
	}

	if (!respostaDoc.isNull())
	{ 
		String mensagemJson;
		serializeJson(respostaDoc, mensagemJson);

		publicarMensagemNoTopico(TOPICO_DISPLAY, mensagemJson.c_str());
	}

	respostaDoc.clear();
	doc.clear();
}