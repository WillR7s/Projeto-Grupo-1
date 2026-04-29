#include "DebugManager.h"
#include <Arduino.h>
#include "secrets.h"

int nivel_debug_atual = DEBUG_NIVEL_INICIAL;

void debug_erro(const String& mensagem)
{
 if(nivel_debug_atual >= DEBUG_ERRO)
 {
    Serial.print("[Erro] ");
    Serial.println(mensagem);
 }
}

void debug_info(const String& mensagem)
{
 if(nivel_debug_atual >= DEBUG_TUDO)
 {
    Serial.print("[Info] ");
    Serial.println(mensagem);
 }
}

void debug_erro_sem_linha(const String& mensagem){if(nivel_debug_atual >= DEBUG_ERRO) Serial.println(mensagem);}

void debug_info_sem_linha(const String& mensagem){if(nivel_debug_atual >= DEBUG_TUDO) Serial.println(mensagem);}

int obter_nivel_de_debug_atual(){ return nivel_debug_atual;}

void configurar_debug()
{
    Serial.begin(9600);
    delay(1000);
    pinMode(PINO_HABILITA_DEBUG_COMPLETO, INPUT_PULLUP);
    digitalRead(PINO_HABILITA_DEBUG_COMPLETO) == LOW? nivel_debug_atual = DEBUG_TUDO : nivel_debug_atual = DEBUG_NIVEL_INICIAL;

    debug_info_sem_linha("\n\n\n\n\n\r");
    
    debug_info("==========================");
    debug_info(" ESP32 Iniciado");
    debug_info(" Sistema de debug ativo");
    if(nivel_debug_atual == DEBUG_TUDO) Serial.println("[INFO] Debug iniciado em modo completo");
    else if(nivel_debug_atual == DEBUG_ERRO) Serial.println("[INFO] Debug iniciado em apenas erros");
    debug_info("==========================");
}

