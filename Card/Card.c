#include"Card.h"


/**********************************************************************************************************************************************************
 ************************************************* Function to get the name of the card holder ************************************************************
 **********************************************************************************************************************************************************/

EN_cardError_t getCardHolderName(ST_cardData_t *cardData) //Function to get the name of the card holder.
{
	printf("Please enter the name as written on the card: ");
	gets(cardData->cardHolderName); //Get the cardholder name from the user.

	int nameLength = strlen(cardData->cardHolderName); //Length of the name that the user entered.
	if (nameLength<20 || nameLength >24) //Check if the name violates the rules of the length.
		{
			printf("The entered name is too short or too long, it should be more than 20 and less than 25\n");
			return WRONG_NAME;
		}

	uint8_t spacesCount = 0; //                                       
	for(int i=0; i<nameLength; i++){ //Check if the name has any invalid characters or the number of spaces is more than 3.
		uint8_t c = cardData->cardHolderName[i];
		if(c == ' '){
			spacesCount++;
		}
		if(((!(c>='a' && c<='z')) && (!(c>='A' && c<='Z')) && (c!= ' ')) || spacesCount>3)
			{
				printf("Not a valid card holder name, it has an invalid characters.\nThe only allowed characters are a-z, A-Z and space.!\n");
				return WRONG_NAME;
			}
	}
	//If it survived all the previous cases it would leave in peace.
	printf("Valid card holder name!\n");
	return CARD_OK; 
}

/************************************************************** Its test function *************************************************************************/

void getCardHolderNameTest(void) //Function to test the upper function"getCardHolderName".
{
	ST_cardData_t instance;
	for(int i=0; i<5; i++){
		switch(i){
			case 0:
				//input: Mahmoud Mostafa Tayee
				getCardHolderName(&instance);
				//Expected output: Valid card holder name!
				break;
			case 1:
				//Mahmoud Mostafa Mahmoud Tayee
				getCardHolderName(&instance);
				//Expected output: The entered name is too short or too long, it should be more than 20 and less than 25!
				break;
			case 2:
				//Mahmoud Mostafa Tayee.
				getCardHolderName(&instance);
				//Expected output: Not a valid card holder name, it has an invalid characters.\nThe only allowed characters are a-z, A-Z and space.!
				break;
			case 3:
				//M                  M
				getCardHolderName(&instance);
				//Expected output: Not a valid card holder name, it has an invalid characters.\nThe only allowed characters are a-z, A-Z and space.!
				break;
			case 4:
				//123456789123456789125
				getCardHolderName(&instance);
				//Expected output: Not a valid card holder name, it has an invalid characters.\nThe only allowed characters are a-z, A-Z and space.
				break;
		}
	}
}

/**********************************************************************************************************************************************************
 ************************************************* Function to get the expiration of the ATM card *********************************************************
 **********************************************************************************************************************************************************/

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) //Function to get the expiration of the ATM card.
{
	printf("Please enter the Expiry date as written on the card in the format \"MM/YY\", e.g \"05/25\": ");
	gets(cardData->cardExpirationDate); //Get the Expiry date from the user.

	int ExDateLength = strlen(cardData->cardExpirationDate); //Length of the Expiry date that the user entered.
	if (ExDateLength!=5) //Check if the name violates the rules of the length.
		{
			printf("The entered Expiry date must be in the format \"MM/YY\", e.g \"05/25\"\n");
			return WRONG_EXP_DATE;
		}

	if(cardData->cardExpirationDate[2] != '/')
	{
		printf("The entered Expiry date must be in the format \"MM/YY\", e.g \"05/25\"\n");
		return WRONG_EXP_DATE;
	}

	for(int i=0; i<ExDateLength; i++){ //Check if the Expiry date has any invalid characters.
		uint8_t c = cardData->cardExpirationDate[i];
		if(i==2)
			continue;
		
		if(!(c>='0' && c<='9'))
			{
				printf("The entered Expiry date must be in the format \"MM/YY\", e.g \"05/25\"\n");
				return WRONG_EXP_DATE;
			}
	}

	//Getting the values of the month and the day to check if they are vioating the rules.
	uint8_t month = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
	// uint8_t day = (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4] - '0');

	if (!(month>=1 && month <=12)){
		printf("The entered expiry date is not vlaid\n");
		return WRONG_EXP_DATE;
	}
	//If it survived all the previous cases it would leave in peace.
	// printf("Valid card Expiry date!\n");
	return CARD_OK; 

}

/************************************************************** Its test function *************************************************************************/

void getCardExpiryDateTest(void) //Function to test the upper function"getCardExpiryDate".
{
	ST_cardData_t instance;
	for(int i=0; i<5; i++){
		switch(i){
			case 0:
				//input: 06/29
				getCardExpiryDate(&instance);
				//Expected output: Valid card Expiry date!
				break;
			case 1:
				//6/29
				getCardExpiryDate(&instance);
				//Expected output: The entered name is too short or too long, it should be more than 20 and less than 25!
				break;
			case 2:
				//06-29
				getCardExpiryDate(&instance);
				//Expected output: Not a valid card holder name, it has an invalid characters.\nThe only allowed characters are a-z, A-Z and space.!
				break;
			case 3:
				//13/29
				getCardExpiryDate(&instance);
				//Expected output: Not a valid card holder name, it has an invalid characters.\nThe only allowed characters are a-z, A-Z and space.!
				break;
			case 4:
				//06/50
				getCardExpiryDate(&instance);
				//Expected output: Not a valid card holder name, it has an invalid characters.\nThe only allowed characters are a-z, A-Z and space.
				break;
		}
	}
}

/**********************************************************************************************************************************************************
 *********************************** Function to get the primary Account Number(PAN) of the card holder ***************************************************
 **********************************************************************************************************************************************************/

EN_cardError_t getCardPAN(ST_cardData_t *cardData) //Function to get the primary Account Number(PAN) of the card holder.
{
	printf("Please enter the PAN number as written on the card: ");
	gets(cardData->primaryAccountNumber); //Get the PAN from the user.

	int nameLength = strlen(cardData->primaryAccountNumber); //Length of the PAN that the user entered.
	if (nameLength<16 || nameLength >19) //Check if the PAN violates the rules of the length.
		{
			printf("The entered PAN is too short or too long, it should be more than 16 and less than 19\n");
			return WRONG_PAN;
		}

	for(int i=0; i<nameLength; i++){ //Check if the PAN invalid characters.
		uint8_t c = cardData->primaryAccountNumber[i];
		
		if(!(c>='0' && c<='9'))
			{
				printf("Not a valid primary Account Number(PAN), it has an invalid characters.\nOnly numbers are allowed.!\n");
				return WRONG_PAN;
			}
	}
	//If it survived all the previous cases it would leave in peace.
	printf("Valid primary Account Number(PAN)!\n");
	return CARD_OK; 
}

/************************************************************** Its test function *************************************************************************/

void getCardPANTest(void) //Function to test the upper function"getCardPAN".
{
	ST_cardData_t instance;
	for(int i=0; i<4; i++){
		switch(i){
			case 0:
				//input: 123456789123456789
				getCardPAN(&instance);
				//Expected output: Valid primary Account Number(PAN)!
				break;
			case 1:
				//123456789123456789123
				getCardPAN(&instance);
				//Expected output: The entered PAN is too short or too long, it should be more than 16 and less than 19!
				break;
			case 2:
				//1234567891z23456789
				getCardPAN(&instance);
				//Expected output: Not a valid primary Account Number(PAN), it has an invalid characters.\nOnly numbers are allowed!
				break;
			case 3:
				//123456
				getCardPAN(&instance);
				//Expected output: The entered PAN is too short or too long, it should be more than 16 and less than 19!
				break;
		}
	}
}


// int main(){

// 	//Test functions
// 	// getCardHolderNameTest();
// 	// getCardExpiryDateTest();
// 	getCardPANTest();


// }



