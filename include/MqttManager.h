#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H
#include <Arduino.h>

typedef void (*Call_back_Mensagem_MQTT)(const char* topico, const  String& mensagem);
void configurar_MQTT();
void conectar_MQTT();
void garantir_MQTT_conectado();
void loop_MQTT();
void publicar_mensagem(const char * topico, const char * mensagem);
void registrar_Call_back_Mensagem(Call_back_Mensagem_MQTT callback);

bool MQTT_esta_conectado();

const char * obter_topico_publicacao();
const char * obter_topico_recebimento();




#endif