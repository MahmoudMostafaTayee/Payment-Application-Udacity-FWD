#include"Terminal.h"

/**********************************************************************************************************************************************************
 ****************************************** Function to get the date of the Transaction *******************************************************************
 **********************************************************************************************************************************************************/

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData) //Function to get the transation date from the system in the format DD/MM/YYYY, e.g 25/06/2022.
{
	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);

  	uint8_t c[10];
	c[2] = c[5] = '/';
	c[0] = (tm.tm_mday / 10) + '0';
	c[1] = (tm.tm_mday % 10) + '0';

	c[3] = ((tm.tm_mon + 1)/10) + '0';
	c[4] = ((tm.tm_mon + 1)%10) + '0';

	c[6] = ((tm.tm_year + 1900)/1000) + '0';
	c[7] = ((tm.tm_year + 1900)/100)%10 + '0';
	c[8] = ((tm.tm_year + 1900)/10)%10 + '0';
	c[9] = (tm.tm_year + 1900)%10 + '0';
	c[10] = '\0';

	strncpy(termData->transactionDate, c, 11);
	printf("%s\n", termData->transactionDate);
	
	return TERMINAL_OK; 
}
/************************************************************** Its test function *************************************************************************/

void getTransactionDateTest(void) //Fucntion to check the fuction abovegetTransactionDate(ST_terminalData_t *)
{
	ST_terminalData_t termInstance;
	getTransactionDate(&termInstance);
}

/**********************************************************************************************************************************************************
 **************************************** Function to compare the transaction date with the expiration date ***********************************************
 **********************************************************************************************************************************************************/

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)// This function is to compare the transaction date with the expiration date.
{


	// Putting the transaction date in the same format as the card expiry date.
	char c[8];
	strncpy(c, &(termData->transactionDate[3]), 8);
	strncpy(&(c[3]), &(c[5]), 3);


	//Comparing the 2 dates.
	uint8_t compResult;
	compResult = strncmp(c, cardData->cardExpirationDate, 5);
	if (compResult == 1){
		printf("%s", "Card is expired!\n");
		return EXPIRED_CARD;
	}


	// If it survived the check on the expiration date so, it's valid card.
	printf("Card is valid!\n");
	return TERMINAL_OK;
}

/************************************************************** Its test function *************************************************************************/

void isCardExpriedTest(void) //Fucntion to check the fuction above(isCardExpired(ST_cardData_t *, ST_terminalData_t *)).
{
	ST_cardData_t cardInstanse;
	

	ST_terminalData_t termInstance;
	getTransactionDate(&termInstance);
	

	for(int i=0; i<3; i++){
		
		switch(i){
			case 0:
					//input: 09/21
					getCardExpiryDate(&cardInstanse);
					isCardExpired(&cardInstanse, &termInstance);
					//Expected output: Card is expired!
					break;
			case 1:
					//input: 09/22
					getCardExpiryDate(&cardInstanse);
					isCardExpired(&cardInstanse, &termInstance);
					//Expected output: Card is valid!
					break;
			case 2:
					//input: 09/23
					getCardExpiryDate(&cardInstanse);
					isCardExpired(&cardInstanse, &termInstance);
					//Expected output: Card is valid!
					break;
		}	
	}
}

/**********************************************************************************************************************************************************
 ********************************************** Function to get the transaction amount from the user ******************************************************
 **********************************************************************************************************************************************************/


EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData) // This function is to get the transaction amount from the user.
{
	printf("Please enter the amount of transaction: ");
	scanf("%f", &(termData->transAmount));

	if((termData->transAmount<=0)) // Check if the entered amount is greater than 0.
	{
		printf("Invalid transaction amount!\n");
		return INVALID_AMOUNT;
	}

	printf("The transcation amount in valid format!\n");
	return TERMINAL_OK;
}

/************************************************************** Its test function *************************************************************************/

void getTransactionAmountTest(void) //Fucntion to check the fuction above(getTransactionAmount(ST_terminalData_t *))
{
	ST_terminalData_t termInstance;
	for(int i=0; i<3; i++){
		
		switch(i){
			case 0:
					//input: 15.25
					getTransactionAmount(&termInstance);
					//Expected output: The transcation amount in valid amount!
					break;
			case 1:
					//input: 0
					getTransactionAmount(&termInstance);
					//Expected output: Invalid transaction amount!
					break;
			case 2:
					//input: -3
					getTransactionAmount(&termInstance);
					//Expected output: Invalid transaction amount!
					break;
		}	
	}
}

/**********************************************************************************************************************************************************
 **************************************** Function to compare between the max and amount of transcation ***************************************************
 **********************************************************************************************************************************************************/


EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount) //Function to get the max available amount.
{
	
	termData->maxTransAmount = maxAmount;

	if((termData->maxTransAmount)<=0){
		printf("This is not a valid format for the max amount!\n");
		return INVALID_MAX_AMOUNT;
	}

	printf("The max amount, %.2f, is in a valid format!\n", maxAmount);
	return TERMINAL_OK;
}

/************************************************************** Its test function *************************************************************************/

void setMaxAmountTest(void) //Fucntion to check the fuction above(setMaxAmount(ST_terminalData_t *, float))
{
	ST_terminalData_t termInstance;
	for(int i=0; i<3; i++){
		
		switch(i){
			case 0:
					//input: 5.35
					setMaxAmount(&termInstance, 5.35);
					//Expected output: The max amount, 5.35, is in a valid format!
					break;
			case 1:
					//input: 0
					setMaxAmount(&termInstance, 0);
					//Expected output: This is not a valid format for the max amount!
					break;
			case 2:
					//input: -5.35
					setMaxAmount(&termInstance, -5.35);
					//Expected output: This is not a valid format for the max amount!
					break;
		}	
	}
}

/**********************************************************************************************************************************************************
 **************************************** Function to compare between the max and amount of transcation ***************************************************
 **********************************************************************************************************************************************************/

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData) //Function to compare between the max and amount of transcation.
{
	if((termData->transAmount)>(termData->maxTransAmount))
	{
		printf("Required amount exceeds the max!\n");
		return EXCEED_MAX_AMOUNT;
	}

	printf("The required amount is below the maximum allowed amount!\n");
	return TERMINAL_OK;
}

/************************************************************** Its test function *************************************************************************/

void isBelowMaxAmountTest(void) //Fucntion to check the fuction above(isBelowMaxAmount(ST_terminalData_t *))
{
	ST_terminalData_t termInstance;
	setMaxAmount(&termInstance, 100);

	for(int i=0; i<3; i++){
		switch(i){
			case 0:
				//input: 50
				getTransactionAmount(&termInstance);
				isBelowMaxAmount(&termInstance);
				//Expected ouput: The required amount is below the maximum allowed amount!
				break;
			case 1: 
				//input: 100
				getTransactionAmount(&termInstance);
				isBelowMaxAmount(&termInstance);
				//Expected ouput: The required amount is below the maximum allowed amount!
				break;
			case 2: 
				//input: 100
				getTransactionAmount(&termInstance);
				isBelowMaxAmount(&termInstance);
				//Expected ouput: Required amount exceeds the max!
				break;

		}
	}
}

/**********************************************************************************************************************************************************
 *************************************************** Function to check if the PAN number is correct *******************************************************
 **********************************************************************************************************************************************************/
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData){

	/*
		Used the reference from https://www.geeksforgeeks.org/luhn-algorithm/ to understand the algorithm
		also liked the flag check so added it from their code.

	*/
	uint8_t PAN[20];

	strncpy(PAN, cardData->primaryAccountNumber, 20);

	uint8_t accumulator = 0, temp, len = strlen(PAN);
	bool flag = false;
	for(int i =0; i<len; i++){
		temp = PAN[len - i -1] - '0';
		if (flag)
			temp = 2*temp;
		if(temp>=10){
			temp = (temp%10) + (temp/10);
		}
		accumulator += temp;
		flag = !flag;
		printf("%d\n", temp);
	}

	printf("%d", accumulator);

	if(accumulator%10 ){
		printf("Not a valid PAN!\n");
		return INVALID_CARD;
	}

	printf("This is a valid PAN!\n");
	return TERMINAL_OK;
}

// int main(){

// 	// getTransactionDateTest();
// 	// isCardExpriedTest();
// 	// getTransactionAmountTest();
// 	// isBelowMaxAmountTest();
// 	// setMaxAmountTest();


// 	// ST_terminalData_t termInstance;

// 	ST_cardData_t CardInstance;
// 	getCardPAN(&CardInstance);
// 	isValidCardPAN(&CardInstance);
// }