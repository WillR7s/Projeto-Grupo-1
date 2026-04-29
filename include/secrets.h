#ifndef SECRETS_H
#define SECRETS_H
//Wifi config
const char* WIFI_SSID = "SALA 09";
const char* WIFI_SENHA = "info@134";
//======================================
//DEBUG
//======================================
// 0 = sem mensagens; 1 = apenas erros; 2 = todas as mensagens
#define DEBUG_NIVEL_INICIAL 2
//pino usado para forçar todas as mensagens
#define PINO_HABILITA_DEBUG_COMPLETO 4

//======================================
//MQTT
//======================================
const char* MQTT_BROKER = "broker.hivemq.com";
const char* MQTT_CLIENT_ID = "esp32_Gael_De_Rousset";
const char* MQTT_USUARIO = "";
const char* MQTT_SENHA = "";
const char* MQTT_CERTIFICADO_CA = "";
const char* TOPICOS_PUBLICAR[] = {"senai134/esp32/status", "senai134/esp32/log", "senai134/esp32/resposta"};
const char* TOPICOS_RECEBER[] = {"senai134/esp32/comando", "senai134/esp32/config", "senai134/esp32/coloca_oque_quiser"};

const bool MQTT_TLS = false;

const int MQTT_PORTA = 1883;
const int TOTAL_TOPICOS_PUBLICAR = 3;
const int TOTAL_TOPICOS_RECEBER = 3;


#endif