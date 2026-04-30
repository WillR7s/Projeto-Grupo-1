#ifndef DEBUG_MANAGER_H
#define DEBUG_MANAGER_H
#include <Arduino.h>
#define DEBUG_NENHUM 0
#define DEBUG_ERRO 1
#define DEBUG_TUDO 2

void configurar_debug();
void debug_erro(const String& mensagem);
void debug_info(const String& mensagem);

void debug_erro_sem_linha(const String& mensagem);
void debug_info_sem_linha(const String& mensagem);

int obter_nivel_de_debug_atual();



#endif