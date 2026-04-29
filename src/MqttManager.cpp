#include <arduino.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include "DebugManager.h"
#include "secrets.h"
#include "WiFiManager.h"
#include "MQttManager.h"

WiFiClient wifi_Cliente;
WiFiClientSecure wifi_client_secure;

PubSubClient mqtt_Client;

Call_back_Mensagem_MQTT callback_Da_Aplicacao = nullptr;

void registrar_Call_back_Mensagem(Call_back_Mensagem_MQTT callback)
{
    callback_Da_Aplicacao = callback;
    callback_Da_Aplicacao != nullptr? debug_info("Callback da aplicação registrado com sucesso") : debug_erro("Callback da aplicação não foi registrado");
}

const char* obter_topico_publicacao(int indice_Topico)
{
    if(indice_Topico < 0 || indice_Topico >= TOTAL_TOPICOS_PUBLICAR)
    {
        debug_erro("Indice invalido para topico de publicação: " + String(indice_Topico));
        return "";
    }
    return TOPICOS_PUBLICAR[indice_Topico];
}

const char* obter_topico_recebimento(int indice_Topico)
{
    if(indice_Topico < 0 || indice_Topico >= TOTAL_TOPICOS_RECEBER)
    {
        debug_erro("Indice invalido para topico de recebimento: " + String(indice_Topico));
        return "";
    }
    return TOPICOS_RECEBER[indice_Topico];
}

void callback_Interno_MQTT(char* topico, byte* payload, unsigned int tamanho)
{
    String mensagem = "";
    for(uint16_t i = 0; i < tamanho; i++) mensagem += (char)payload[i];
    
    debug_info("===============================");
    debug_info("Mensagem MQTT recebida");
    debug_info("===============================");
    debug_info("Tópico: " + String(topico));
    debug_info("Mensagem: " + String(mensagem));
    debug_info("===============================");
    callback_Da_Aplicacao != nullptr?  
    callback_Da_Aplicacao (topico, mensagem) : debug_erro("Mensagem foi recebida mas nenhum callback registrado");
   
       
    
}

void configurar_MQTT()
{
     debug_info("===============================");
     debug_info("Configurando MQTT");
     debug_info("===============================");

     if(USAR_AWS_IOT)
     {
        //TODO: Implementar código para usar broker IOT Core da AWS
     }
     else if(MQTT_TLS)
     {
        //TODO: Implementar codigo para conectar ao broker com certificado TLS
     }
     else //Conectar ao broker publico sem certificado
     {
        debug_info("Modo selecionado: MQTT sem TLS.");
        mqtt_Client.setClient(wifi_Cliente);
        mqtt_Client.setServer(MQTT_BROKER, MQTT_PORTA);
        debug_info("Broker MQTT: " + String(MQTT_BROKER));
        debug_info("Porta MQTT: " + String(MQTT_PORTA));
     }
     mqtt_Client.setCallback(callback_Interno_MQTT);
     debug_info("Callback interno no MQTT configurado");
}