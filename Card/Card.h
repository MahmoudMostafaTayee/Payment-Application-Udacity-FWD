#ifndef Card_H
#define Card_H


#include<stdio.h>
#include<string.h>

typedef unsigned char uint8_t;

//Defining all the error types.
typedef enum EN_cardError_t
{
     CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

//Defining a structure to represent the data of the ATM card
typedef struct ST_cardData_t
{
     uint8_t cardHolderName[25];
     uint8_t primaryAccountNumber[20];
     uint8_t cardExpirationDate[6];
}ST_cardData_t;


EN_cardError_t getCardHolderName(ST_cardData_t *cardData); //Function to get the name of the card holder.
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData); //Function to get the expiration of the ATM card.
EN_cardError_t getCardPAN(ST_cardData_t *cardData); //Function to get the PAN card of the ATM card.
void getCardHolderNameTest(void); //Function to test the getCardHolderName.
void getCardExpiryDateTest(void); //Function to test the getCardExpiryDate.
void getCardPANTest(void); //Function to test the getCardPAN.

#endif