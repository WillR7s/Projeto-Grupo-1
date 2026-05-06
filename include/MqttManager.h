#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>

void configurarMQTT();
void conectarMQTT();
void garantirMQTTconectado();
void loopMQTT();

void publicarMensagem(const char* topico, const char* mensagem);

bool mqttEstaConectado();

const char* obterTopicoPublicaco();
const char* obterTopicoRecebimento(int indiceTopico);
int obterTotalTopicosRecebimento();
void publicarMensagemNoTopico(int indiceTopico, const char* mensagem);

typedef void (*CallbackMensagemMQTT)(const char* topico, const String& mensagem);
void registrarCallBackMensagem(CallbackMensagemMQTT callback);

#endif