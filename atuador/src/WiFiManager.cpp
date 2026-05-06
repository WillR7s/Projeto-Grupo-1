#include <Arduino.h>
#include <WiFi.h>
#include "WiFiManager.h"
#include "secrets.h"
#include "DebugManager.h"

void conectarWifi()
{
	debugInfoSemLinha("===============================\n");
	debugInfo("Iniciando conexão WiFi ...");
	debugInfoSemLinha("===============================\n");

	WiFi.mode(WIFI_STA);

	WiFi.begin(WIFI_SSID, WIFI_SENHA);

	debugInfoSemLinha("Conectando ");

	int tentativas = 0;
	const int maxTentativa = 30;

	while (WiFi.status() != WL_CONNECTED && tentativas < maxTentativa)
	{
		debugInfoSemLinha(". ");
		delay(500);

		if (WiFi.status() == WL_CONNECT_FAILED)
			break;
		tentativas++;
	}

	debugInfoSemLinha("\n");
	
	if (WiFi.status() == WL_CONNECTED)
	{
		debugInfo("Conectado");
		debugInfoSemLinha("[INFO] Endereco IP: ");
		debugInfoSemLinha(String(WiFi.localIP()));
		debugInfoSemLinha("\n");
	}
	else
		debugErro("Falha na conexão com o WiFi");
		debugErro("Verifique SSID, senha e sinal de rede.");
}

void garantirWiFiConectado()
{
	if(WiFi.status() != WL_CONNECTED)
	{
		debugInfo("WiFi desconectado. Tentando reconectar...");
		conectarWifi();
	}

	if(WiFi.status() != WL_CONNECTED)
	{
		debugErro("Não foi possivel reconectar ao WiFi");
	}
}

bool wifiEstaConectado()
{
    return WiFi.status() == WL_CONNECTED;
}