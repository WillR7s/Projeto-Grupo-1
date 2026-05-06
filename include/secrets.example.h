#ifndef SECRETS_H
#define SECRETS_H

// Wifi
extern const char* WIFI_SSID;
extern const char* WIFI_SENHA;

// Debug
// 0 = sem mensagens; 1 = apenas erros; 2 = todas as mensagens
extern const int DEBUG_NIVEL_INICIAL;
extern const int PINO_HABILITA_DEBUG_COMPLETO;

// MQTT
extern const char* MQTT_BROKER;
extern const char* MQTT_CLIENT_ID;
extern const char* MQTT_USUARIO;
extern const char* MQTT_SENHA;
extern const char  MQTT_CERTIFICADO_CA[];
extern const char* TOPICOS_PUBLICAR[];
extern const char* TOPICOS_RECEBER[];

extern const bool MQTT_TLS;
extern const bool USAR_AWS_IOT;

extern const int MQTT_PORTA;
extern const int TOTAL_TOPICOS_PUBLICAR;
extern const int TOTAL_TOPICOS_RECEBER;
extern const int PINO_DEBUG;

#endif
