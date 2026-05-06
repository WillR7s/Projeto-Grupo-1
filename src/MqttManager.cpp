#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Arduino.h>

#include "MqttManager.h"
#include "secrets.h"
#include "WiFiManager.h"
#include "DebugManager.h"

WiFiClient wifiClient;
WiFiClientSecure wifiClientSecure;

PubSubClient mqttClient;

CallbackMensagemMQTT callbackDaAplicacao = nullptr;

void registrarCallBackMensagem(CallbackMensagemMQTT callback)
{
    callbackDaAplicacao = callback;

    if (callbackDaAplicacao != nullptr)
        debugInfo("Callback da aplicação registrado com sucesso");
    else
        debugErro("Callback continua nulo.");
}

const char *obterTopicoPublicaco(int indiceTopico)
{
    if (indiceTopico < 0 || indiceTopico >= TOTAL_TOPICOS_PUBLICAR)
    {
        debugErro("Indice inválido para tópico de publicação: " + String(indiceTopico));
        return "";
    }
    return TOPICOS_PUBLICAR[indiceTopico];
}

const char *obterTopicoRecebimento(int indiceTopico)
{
    if (indiceTopico < 0 || indiceTopico >= TOTAL_TOPICOS_RECEBER)
    {
        debugErro("Indice inválido para tópico de recebimento: " + String(indiceTopico));
        return "";
    }
    return TOPICOS_RECEBER[indiceTopico];
}

void callbackInternoMQTT(char *topico, byte *payload, uint tamanho)
{
    String mensagem = "";

    for (uint i = 0; i < tamanho; i++)
        mensagem += (char)payload[i];

    debugInfoSemLinha("======================\n");
    debugInfo(" Mensagem MQTT recebida ");
    debugInfoSemLinha("======================\n");
    debugInfo("Tópico: " + String(topico));
    debugInfo("Tópico: " + mensagem);

    if (callbackDaAplicacao != nullptr)
        callbackDaAplicacao(topico, mensagem);
    else
        debugErro ("Mensagem recebida, mas nenhum callback da aplicação foi registrado.");
}

void configurarMQTT()
{
    debugInfoSemLinha("=========================\n");
    debugInfoSemLinha(" Mensagem MQTT recebida \n");
    debugInfoSemLinha("=========================\n");

    if (USAR_AWS_IOT)
    {
        //TODO: Implementaro código para usar broker IOT com Core da AWS
    }
    else if (MQTT_TLS)
    {    
        debugInfo("Modo selecionado: MQTT com TLS.");

        if (strlen(MQTT_CERTIFICADO_CA) > 100)
        {
            debugInfo("Certificado CA do broker MQTT configurado.");
            wifiClientSecure.setCACert(MQTT_CERTIFICADO_CA);
        }
        else
        {
            debugErro("Certificado não configurado. Usando Insecuret");
            wifiClientSecure.setInsecure();
        }

        mqttClient.setClient(wifiClientSecure);
        mqttClient.setServer(MQTT_BROKER, MQTT_PORTA);

        debugInfo("Broker MQTT: " + String(MQTT_BROKER));
        debugInfo("Porta MQTT: " + String(MQTT_PORTA));
    }
    else
    {
        debugInfo("Modo selecionado: MQTT sem TLS");

        mqttClient.setClient(wifiClient);
        mqttClient.setServer(MQTT_BROKER, MQTT_PORTA);

        debugInfo("Broker MQTT: " + String(MQTT_BROKER));
        debugInfo("Porta MQTT: " + String(MQTT_PORTA));
    }

    mqttClient.setCallback(callbackInternoMQTT);
    debugInfo("Callback interno no MQTT configurado");
}

void conectarMQTT()
{
    if (!wifiEstaConectado())
    {
        debugErro("MQTT não pode ser conectar porque o WiFi está desconectado");
        return;
    }

    debugInfoSemLinha("=============================\n");
    debugInfoSemLinha(" Iniciando conexão MQTT ... \n");
    debugInfoSemLinha("=============================\n\n");

    int tentativasMQTT = 0;
    const int maxTentativas = 5;

    while(!mqttEstaConectado() && tentativasMQTT < maxTentativas)
    {
        debugInfo("Tentando conectar ao broker MQTT. Tentativa: " + String(tentativasMQTT));

        bool conectado = false;

        if (USAR_AWS_IOT)
        {
            // TODO: Implementar conexão AWS
        }
        else
        {
            if (strlen(MQTT_USUARIO) > 0)
            {
                debugInfo("Concetando MQTT com usuario e senha");

                conectado = mqttClient.connect(
                    MQTT_CLIENT_ID, 
                    MQTT_USUARIO, 
                    MQTT_SENHA
                );
            }
            else
            {
                debugInfo("Conectando MQTT sem usuário e senha");
                conectado = mqttClient.connect(MQTT_CLIENT_ID);
            }
        }
        if (conectado)
        {
            debugInfo("MQTT conectado com sucesso");
            int totalTopicos = obterTotalTopicosRecebimento();
            debugInfo("Total de tópicos para inscrição: " + String(totalTopicos));

            for (int i = 0; i < totalTopicos; i++)
            {
                const char* topico = obterTopicoRecebimento(i);

                bool inscrito = mqttClient.subscribe(topico);

                if (inscrito)
                {
                    debugInfo("Inscrito no tópico: " + String(topico));
                }
                else
                {
                    debugErro("Falha ao se inscrever no topico: " + String(topico));
                }
            }

            publicarMensagemNoTopico(0, "ESP32 conectado ao MQTT");
        }
        else
        {
            debugErro("Falha ao conectar ao MQTT. Código de erro: " + String(mqttClient.state()));
            tentativasMQTT++;
            delay(2000);
        }
    }

    if (!mqttEstaConectado())
    {
        debugErro("Não foi possível conectar ao broker MQTT após " + String(maxTentativas) + " tentativas");
    }
}

int obterTotalTopicosRecebimento()
{
    return TOTAL_TOPICOS_RECEBER;
}

void garantirMQTTconectado()
{
    if(!wifiEstaConectado())
    {
        debugErro("MQTT não reconetado porque o WiFi está desconectado");
        return;
    }

    if(!mqttEstaConectado())
    {
        debugErro("MQTT desconectado. Tentando reconectar...");
        conectarMQTT();
    }
}

void loopMQTT()
{
    mqttClient.loop();
}

void publicarMensagem(const char *topico, const char *mensagem)
{
    if (!mqttEstaConectado())
    {
        debugErro("Não foi possível publicar. MQTT desconectado");
        return;
    }

    bool publicado = mqttClient.publish(topico, mensagem);

    if (publicado)
    {
        debugInfo("Mensagem publicada vida MQTT.");
        debugInfo("Topico: " + String(topico));
        debugInfo("Mensagem: " + String(mensagem));
    }
    else
    {
        debugErro("Falha ao publicar mensagem no tópico: " + String(topico));
    }
}

void publicarMensagemNoTopico(int indiceTopico, const char* mensagem)
{
    const char* topico = obterTopicoPublicaco(indiceTopico);

    if (strlen(topico) == 0)
    {
        debugErro("Não foi possível publicar. Índice de tópico inválido: " + String(indiceTopico));
        return;
    }

    publicarMensagem(topico, mensagem);
}

bool mqttEstaConectado()
{
    return mqttClient.connected();
}