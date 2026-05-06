#include <Arduino.h>
#include <ArduinoJson.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"

const char TOPICO_ESCUTANDO[] = "senai134/g1/esp32/display";

void tratarMensagemRecebida(const char *topico, const String &mensagem);
void tratarJsonTopico(const String &mensagem);

void setup()
{
	configurarDebug();
	conectarWifi();
	configurarMQTT();
	registrarCallBackMensagem(tratarMensagemRecebida);
	conectarMQTT();
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

	if (strcmp(topico, TOPICO_ESCUTANDO) == 0)
	{
		tratarJsonTopico(mensagem);
		return;
	}

	debugErro("Tópico não tratado: " + String(topico));
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
	bool temCorRGB = doc["ledRGB"].is<JsonObject>();
	bool temUnidade = doc["unidade"].is<String>();

	if (!temTemperatura && !temLampada && !temCorRGB && !temUnidade)
	{
		debugErro("JSON precisa conter ao menos 'temperatura' ou 'estadoLampada' ou 'ledRGB' ou 'unidade'");
		return;
	}

	if (temTemperatura)
	{
		// TODO Lógica para converter e enviar para o LCD
	}

	if(temLampada)
	{
		// TODO aqui enviar para o LCD mostra estado da lampada
	}

	if(temCorRGB)
	{
		// TODO aqui enviar para o LCD mostra estado da lampada
	}
}