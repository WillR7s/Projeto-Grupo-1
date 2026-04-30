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

void conectar_MQTT()
{
    if(!wifi_esta_conectado)
    {
        debug_erro("MQTT não pode conectar porque o wifi está desconectado");
        return;
    }
        debug_info("======================================");
        debug_info("Iniciando conexão MQTT...");
        debug_info("======================================");
        int tentativas_MQTT = 0;
        const int maximo_de_tentativas = 5;

        while(!mqtt_Client.connected() && tentativas_MQTT < maximo_de_tentativas)
        {
            debug_info("tentando conectar ao Broker MQTT. Tentativas: " + String(tentativas_MQTT));
            bool conectado = false;
            if(USAR_AWS_IOT)
            {

            }

            else
            {
                if(strlen(MQTT_USUARIO) > 0)
                {
                    debug_info("Conectando MQTT com usuario e senha");
                    conectado = mqtt_Client.connect(MQTT_CLIENT_ID, MQTT_USUARIO, MQTT_SENHA);
                }

                else//conexão no modo anonimo
                {
                    debug_info("Conectando MQTT sem usuario e senha.");
                    conectado = mqtt_Client.connect(MQTT_CLIENT_ID); //Linha importante
                }
            }
                if(conectado)
                {
                    debug_info("MQTT conectado com sucesso");
                    int total_topicos = obter_Total_Topicos_Recebimento();
                    debug_info("Total de tópicos para inscrição: " + String(total_topicos));
                    for(int i = 0; i < total_topicos; i++)
                    {
                        const char* topico = obter_topico_recebimento(i);
                        bool inscrito = mqtt_Client.subscribe(topico); //Linha importante
                        if(inscrito) debug_info("Inscrito no topico" + String(topico));
                        else debug_erro("Falha ao se inscrever no topico: " + String(topico));
                    }
                    publicar_mensagem_no_topico(0, "ESP32 conectado ao MQTT");
                }
               else 
               {
                debug_erro("Falha ao conectar no MQTT. Codigo de erro: " + String(mqtt_Client.state()));
                tentativas_MQTT++;
                delay(2000);
               }
        } //Fim do while
        if(!mqtt_Client.connected()) debug_erro("Não foi possivel conectar ao broker MQTT após " + String(maximo_de_tentativas) + " tentativas");
}

int obter_Total_Topicos_Recebimento(){return TOTAL_TOPICOS_RECEBER;}

void garantir_MQTT_conectado()
{
    if(!wifi_esta_conectado()) debug_erro("MQTT não reconectado, porque o wifi esta desconectado");
    if(!mqtt_Client.connected())
    {
        debug_erro("MQTT desconectado. Tentando reconectar....");
        conectar_MQTT();
    }
}

bool MQTT_esta_conectado(){return mqtt_Client.connected();}

void loop_MQTT() {mqtt_Client.loop();}

void publicar_mensagem(const char * topico, const char * mensagem)
{
    if(!mqtt_Client.connected()) 
    {
        debug_erro("Não foi possivel publicar. MQTT Desconectado");
        return;
    }

    bool publicado = mqtt_Client.publish(topico, mensagem);

    if(publicado) 
    {
    debug_info("Mensagem publicada via MQTT.");
    debug_info("Topico: " + String(topico));
    debug_info("Mensagem: " + String(mensagem));
    }
    else  debug_erro("Falha ao publicar mensagem no topico: " + String(topico));
    
      
    
}

void publicar_mensagem_no_topico(int indice_topico, const char* mensagem)
{
    const char* topico = obter_topico_publicacao(indice_topico);
    if(strlen(topico) == 0) 
    {
    debug_erro("Não foi possivel publicar. Indice de topico invalido: " + String(indice_topico));
    return;
    }
    publicar_mensagem(topico, mensagem);
}