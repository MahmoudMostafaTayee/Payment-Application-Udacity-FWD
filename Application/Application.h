#ifndef Application_H
#define Application_H

#include"../Card/Card.h"
#include"../Server/Server.h"
#include"../Terminal/Terminal.h"

typedef unsigned uint32_t;
typedef unsigned char uint8_t;

typedef enum {
	START_MODULE, CARD_MODULE, TERMINAL_MODULE, SERVER_MODULE, ANOTHER_TRANSACTION, END_MODULE
} MODULE_IDENTIFIER;

void appStart(void);

#endif

