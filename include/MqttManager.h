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
void publicar_mensagem_no_topico(int indice_topico, const char* mensagem);
bool MQTT_esta_conectado();

const char* obter_topico_publicacao(int indice_Topico);
const char* obter_topico_recebimento(int indice_Topico);

int obter_Total_Topicos_Recebimento();



#endif