#include <Arduino.h>
#include <ArduinoJson.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"

#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "time.h"
LiquidCrystal_I2C lcd(0x27, 20, 4);

const char *Servidor = "a.ntp.br";

const long Offset = -3 * 3600; // UTC-3
const int horarioDeVerao = 0;
int temperaturaC = 25;
struct tm timeinfo;

const char TOPICO_ESCUTANDO[] = "senai134/g1/esp32/display";

void tratarMensagemRecebida(const char *topico, const String &mensagem);
void tratarJsonTopico(const String &mensagem);
void trocartela();
void tratarHora();

void setup()
{
	configurarDebug();
	conectarWifi();
	configurarMQTT();
	registrarCallBackMensagem(tratarMensagemRecebida);
	conectarMQTT();
	lcd.init();
	lcd.backlight();

	// Tela inicial
	lcd.setCursor(4, 0);
	lcd.print("Iniciando");
	delay(1000);
	lcd.setCursor(0, 1);
	lcd.print("Obtendo parametros");
	delay(2000);
	lcd.setCursor(3, 2);
	lcd.print("Condensador 1");
	delay(2000);

	configTime(Offset, horarioDeVerao, Servidor);
	tratarHora();
	trocartela();
}

void loop()
{
	garantirWiFiConectado();
	garantirMQTTconectado();
	loopMQTT();
	tratarHora();
	delay(1000);
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

	if (!temTemperatura && !temLampada)
	{
		debugErro("JSON precisa conter ao menos 'temperatura' ou 'estadoLampada'");
		return;
	}
}

void trocartela()
{
	lcd.clear();
	// Temperatura Celsius
	lcd.setCursor(0, 0);
	lcd.print("Temp: ");
	lcd.print(temperaturaC);
	lcd.write(223);
	lcd.print("C");
	lcd.setCursor(11, 0);
	lcd.setCursor(0, 1);
	lcd.print("Lampada: on/off");
	lcd.setCursor(0, 2);
	lcd.print("Status: Quente");
	lcd.setCursor(0, 3);
    lcd.print("Hora: ");
    lcd.print(&timeinfo, "%H:%M:%S  ");
}

void tratarHora()
{
	// Tenta obter hora
	if (!getLocalTime(&timeinfo))
	{
		debugErro("Falha ao obter hora");
		return;
	}

	// LCD
	lcd.setCursor(0, 3);
	lcd.print("Hora: ");

	lcd.print(&timeinfo, "%H:%M:%S  ");
}

/*if (temTemperatura)
{
	// TODO Lógica para converter e enviar para o LCD
}

if(temLampada)
{
	// TODO aqui enviar para o LCD mostra estado da lampada
}*/
