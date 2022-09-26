#ifndef TERMINAL_H
#define TERMINAL_H


#include<stdio.h>
#include<string.h>
#include <time.h>
#include"../Card/Card.h"
#include<math.h>
#include <stdbool.h>

typedef unsigned char uint8_t;

typedef struct ST_terminalData_t
{
     float transAmount;
     float maxTransAmount;
     uint8_t transactionDate[11];
}ST_terminalData_t;


typedef enum EN_terminalError_t
{
     TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t ;


EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData); // Optional
void getTransactionDateTest(void); //Fucntion to check the fuction abovegetTransactionDate(ST_terminalData_t *)
void isCardExpriedTest(void); //Fucntion to check the fuction above(isCardExpired(ST_cardData_t *, ST_terminalData_t *)).
void getTransactionAmountTest(void); //Fucntion to check the fuction above(getTransactionAmount(ST_terminalData_t *))
void setMaxAmountTest(void); //Fucntion to check the fuction above(setMaxAmount(ST_terminalData_t *, float))

#endif