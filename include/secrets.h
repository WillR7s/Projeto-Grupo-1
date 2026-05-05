#ifndef SECRETS_H
#define SECRETS_H
//Wifi config
extern const char* WIFI_SSID;
extern const char* WIFI_SENHA;
//======================================
//DEBUG extern const int DEBUG_NIVEL_INICIAL; 2
//======================================
// 0 = sem mensagens; 1 = apenas erros; 2 = todas as mensagens
extern const int DEBUG_NIVEL_INICIAL; 
//pino usado para forçar todas as mensagens
extern const int PINO_HABILITA_DEBUG_COMPLETO;

//======================================
//MQTT
//======================================
 extern const char* MQTT_BROKER; // = "broker.hivemq.com";
extern const char* MQTT_CLIENT_ID; //= "esp32_Gael_De_Rousset";
extern const char* MQTT_USUARIO; //= "";
extern const char* MQTT_SENHA; //= "";
extern const char MQTT_CERTIFICADO_CA[]; //= "";
extern const char* TOPICOS_PUBLICAR[]; //= {"senai134/esp32/status", "senai134/esp32/log", "senai134/esp32/resposta"};
extern const char* TOPICOS_RECEBER[]; //= {"senai134/esp32/comando", "senai134/esp32/config", "senai134/esp32/coloca_oque_quiser"};

extern const bool MQTT_TLS; //= false;
extern const bool USAR_AWS_IOT; //= false;

extern const int MQTT_PORTA; //= 1883;
extern const int TOTAL_TOPICOS_PUBLICAR; //= 3;
extern const int TOTAL_TOPICOS_RECEBER; //= 3;


#endif