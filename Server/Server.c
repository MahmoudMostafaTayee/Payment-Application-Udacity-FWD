#include"Server.h"

ST_accountsDB_t accountsDB[255];
ST_transaction_t transactionsDB[255];
uint32_t tranSequence;
uint8_t ref;

/*********************************************************** Addtional function ***************************************************************************
 ******************************************* Function to get the database of accounts from the file *******************************************************
 **********************************************************************************************************************************************************/

EN_serverError_t getAccountsDB(ST_transaction_t *transData)
{
	// Used mainly this video to implement this function, https://www.youtube.com/watch?v=rbVt5v8NNe8

	//File pointer variable for accessing the file.
	FILE *file;

	// Attempt to open the file accountsDB.txt in reading mode to read its contents.
	file = fopen("../Server/accountsDB.txt", "r");

	///////// Read the transactions sequence.
	uint8_t read = fscanf(file, "%d\n", &(transData->transactionSequenceNumber));

	if (file == NULL)
	{
		printf("\tError opening file!\n");
		return ACCOUNT_NOT_FOUND; // I wanted to add another type of error but though it would be more conveninent to use one from the defined ones, the one that is close.
	}

	if(read != 1)
		{
			printf("\tFile format is not correct!\n");
			return ACCOUNT_NOT_FOUND; // I wanted to add another type of error but though it would be more conveninent to use one from the defined ones, the one that is close.

		}



	// Check if the file opened scuccessfully of a mistake heppened.
	
	// read is to ensure that each line/record is read correctly and records to keep track of no. of records.
	uint8_t records = 0;

	//An array of characters to read the state of the account and then map it to the enum EN_accountState_t for the variable "state".
	char str[8];

	//Read all records.

	do{


		read = fscanf(file, 
							"%f, %7[^,], %19[^\n]\n", 
							&accountsDB[records].balance, 
							str, 
							accountsDB[records].primaryAccountNumber);


		accountsDB[records].state = RUNNING;
		if(!strncmp(str, "BLOCKED", 7))
			accountsDB[records].state = BLOCKED;

		//If fscanf has already read all the values from the file then we have already read another record successfully.
		if(read == 3) records++;


		if(read != 3 && !(feof(file)))
		{
			printf("\tFile format is not correct!\n");
			return ACCOUNT_NOT_FOUND; // I wanted to add another type of error but though it would be more conveninent to use one from the defined ones, the one that is close.
		}

		// Check if there is an error reading from the file.
		if(ferror(file))
		{
			printf("\t\tError reading from the file!\n");
			return ACCOUNT_NOT_FOUND; // I wanted to add another type of error but though it would be more conveninent to use one from the defined ones, the one that is close.
		}

	}while(!(feof(file)));


	// Close the file after finish reading from it.
	fclose(file);

	// //Print the number of records read from the file.
	// printf("No. of records read is: %d records.\n", records);

	// // Print all the records that have been read from the file.
	// for(int i=0; i<records; i++){
	// 	printf("The account with the PAN: %s has balance: %f and the account state is %d\n",
	// 																						accountsDB[i].primaryAccountNumber,
	// 																						accountsDB[i].balance,
	// 																						accountsDB[i].state);
	// }

}

/**********************************************************************************************************************************************************
 ************************************** Function to take all transaction data and validate its data *******************************************************
 **********************************************************************************************************************************************************/

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
	
	ST_accountsDB_t *accountRefrence = (ST_accountsDB_t*)malloc(sizeof(ST_accountsDB_t));

	if( isValidAccount(&(transData->cardHolderData), accountRefrence) == ACCOUNT_NOT_FOUND){
			free(accountRefrence);
			return FRAUD_CARD;
	}
	if(isBlockedAccount(accountRefrence) == BLOCKED_ACCOUNT){
			free(accountRefrence);
			return DECLINED_STOLEN_CARD;
	}
	if(isAmountAvailable(&(transData->terminalData), accountRefrence) == LOW_BALANCE){
			free(accountRefrence);
			return DECLINED_INSUFFECIENT_FUND;
	}

	free(accountRefrence);
	return APPROVED;
}

/************************************************************** Its test function *************************************************************************/
void recieveTransactionDataTest(void){
	ST_transaction_t transData;

	for(int i=0; i<5; i++){
		switch(i){
			case 0:
				//input: 031349698688737606(Valid account number), 500.0(Available amount) and it's in RUNNING state.
				strcpy(transData.cardHolderData.primaryAccountNumber, "031349698688737606");
				transData.terminalData.transAmount = 500;
				recieveTransactionData(&transData);
				printf("****************************\n");
				break;
			case 1:
				//input: 445814431785914540(Valid account number), 1000.0(Available amount) and it's in BLOCKED state.
				strcpy(transData.cardHolderData.primaryAccountNumber, "445814431785914540");
				transData.terminalData.transAmount = 1000;
				recieveTransactionData(&transData);
				printf("****************************\n");
				break;
			case 2:
				//input: 520175778165282978(Valid account number), 1000.0(Not Available amount) and it's in RUNNING state.
				strcpy(transData.cardHolderData.primaryAccountNumber, "520175778165282978");
				transData.terminalData.transAmount = 1000;
				recieveTransactionData(&transData);
				printf("****************************\n");
				break;
			case 3:
				//input: 445815555785914540(Not valid account number).
				strcpy(transData.cardHolderData.primaryAccountNumber, "445815555785914540");
				transData.terminalData.transAmount = 1000;
				recieveTransactionData(&transData);
				printf("****************************\n");
				break;
		}
	}

}

/**********************************************************************************************************************************************************
 ************************************************ Function to validate if the account exists or not *******************************************************
 **********************************************************************************************************************************************************/

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence){
	int i=0;
	while(accountsDB[i].primaryAccountNumber[0] != '\0'){
		if(!(strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber))){
			ref = i;
			// printf("The account was found at record #%d.\n", i+1);
			accountRefrence->balance = accountsDB[i].balance;
			accountRefrence->state = accountsDB[i].state;
			strcpy(accountRefrence->primaryAccountNumber, accountsDB[i].primaryAccountNumber);
			// printf("%s\n", accountRefrence->primaryAccountNumber);
			return SERVER_OK;
		}
		i++;
	}

	printf("\tThe account was not found.\n");
	accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;
}

/************************************************************** Its test function *************************************************************************/

void isValidAccountTest(void){
	ST_cardData_t cardData;
	ST_accountsDB_t *accountRefrence;

	for(int i=0; i<2; i++){
		switch(i){
			case 0:
				//input: 944886204475764987(Valid account number).
				strcpy(cardData.primaryAccountNumber, "944886204475764987");
				// printf("%x\n", (accountRefrence));
				isValidAccount(&cardData, accountRefrence);
				// printf("%s\n", accountRefrence->primaryAccountNumber);
				break;
			case 1:
				//input: 944886205555764987(Not a valid account number).
				strcpy(cardData.primaryAccountNumber, "944886205555764987");
				// printf("%x\n", (accountRefrence));
				isValidAccount(&cardData, accountRefrence);
				// printf("%s\n", accountRefrence->primaryAccountNumber);
				break;
		}
	}
}

/**********************************************************************************************************************************************************
 ************************************************ Function to verify if the account is BLOCKED or not *****************************************************
 **********************************************************************************************************************************************************/

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence){
	if(accountRefrence->state == RUNNING){
		// printf("The account is in RUNNING state.\n");
		return SERVER_OK;
	}

	printf("\tThe account is in BLOCKED state.\n");
	return BLOCKED_ACCOUNT;
}

/************************************************************** Its test function *************************************************************************/

void isBlockedAccountTest(void){
	ST_cardData_t cardData;
	ST_accountsDB_t accountRefrence;

	for(int i=0; i<2; i++){
		switch(i){
			case 0:
				//input: 944886204475764987(Valid account number).
				strcpy(cardData.primaryAccountNumber, "031349698688737606");
				isValidAccount(&cardData, &accountRefrence);
				isBlockedAccount(&accountRefrence);
				break;
			case 1:
				//input: 944886205555764987(Not a valid account number).
				strcpy(cardData.primaryAccountNumber, "604684011915906746");
				isValidAccount(&cardData, &accountRefrence);
				isBlockedAccount(&accountRefrence);
				break;
		}
	}
}

/**********************************************************************************************************************************************************
 ************************** Function to check if the account has a sufficient amount to withdraw or not. **************************************************
 **********************************************************************************************************************************************************/

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence){
	if(termData->transAmount > accountRefrence->balance){
		printf("\tThe required amount is not available to withdraw.\n");
		return LOW_BALANCE;
	}

	// printf("The required amount is available to withdraw.\n");
	return SERVER_OK;
}

/************************************************************** Its test function *************************************************************************/

void isAmountAvailableTest(void){
	ST_cardData_t cardData;
	ST_accountsDB_t accountRefrence;

	ST_terminalData_t termData;

	for(int i=0; i<2; i++){
		switch(i){
			case 0:
				//input: 500(Available balance).
				strcpy(cardData.primaryAccountNumber, "031349698688737606");
				isValidAccount(&cardData, &accountRefrence);
				termData.transAmount = 500;
				isAmountAvailable(&termData, &accountRefrence);
				break;
			case 1:
				//input: 10000(Not available balance).
				strcpy(cardData.primaryAccountNumber, "8989374615436851");
				isValidAccount(&cardData, &accountRefrence);
				termData.transAmount = 10000;
				isAmountAvailable(&termData, &accountRefrence);
				break;
		}
	}
}

/**********************************************************************************************************************************************************
 *********************************************** Function to save the transaction data to the database ****************************************************
 **********************************************************************************************************************************************************/

EN_serverError_t saveTransaction(ST_transaction_t *transData){
	FILE *file;
	file = fopen("../Server/transactionsDB.txt", "a");

	if(file == NULL){
		printf("\tUnable to opne the file.\n");
		return INTERNAL_SERVER_ERROR;
	}
	
	accountsDB[ref].balance -= transData->terminalData.transAmount;
	tranSequence = (transData->transactionSequenceNumber)+1;
	fprintf(file, "%d", transData->transactionSequenceNumber);
	fputs(",", file);
	fputs(transData->terminalData.transactionDate, file);
	fputs(",", file);
	fprintf(file, "%.2f", transData->terminalData.transAmount);
	fputs(",", file);
	fprintf(file, "%d", recieveTransactionData(transData));
	fputs(",", file);
	fprintf(file, "%.2f", transData->terminalData.maxTransAmount);
	fputs(",", file);
	fputs(transData->cardHolderData.cardHolderName, file);
	fputs(",", file);
	fputs(transData->cardHolderData.primaryAccountNumber, file);
	fputs(",", file);
	fputs(transData->cardHolderData.cardExpirationDate, file);
	fputs("\n", file);

	fclose(file);

	return SERVER_OK;

}

/************************************************************** Its test function *************************************************************************/

void saveTransactionTest(void){

	ST_transaction_t transData; 
	transData.transactionSequenceNumber = 55;
	strcpy(transData.terminalData.transactionDate, "29/06/1994");
	transData.terminalData.transAmount = 55.0;
	transData.terminalData.maxTransAmount = 100;
	strcpy(transData.cardHolderData.cardHolderName, "Mahmoud Mostafa Tayee");
	strcpy(transData.cardHolderData.primaryAccountNumber, "8989374615436851");
	strcpy(transData.cardHolderData.cardExpirationDate, "06/26");


	saveTransaction(&transData);



	}

/**********************************************************************************************************************************************************
 ************************** Function to list the data in the transaction database on the console screen ***************************************************
 **********************************************************************************************************************************************************/

void listSavedTransactions(void)
{
	// Used mainly this video to implement this function, https://www.youtube.com/watch?v=rbVt5v8NNe8


	//File pointer variable for accessing the file.
	FILE *file;

	// Attempt to open the file accountsDB.txt in reading mode to read its contents.
	file = fopen("../Server/transactionsDB.txt", "r");

	
	// read is to ensure that each line/record is read correctly and records to keep track of no. of records.
	uint8_t read = 0, records = 0;

	//An array of characters to read the state of the account and then map it to the enum EN_accountState_t for the variable "state".
	uint32_t transactionSequenceNumber;
	uint8_t transactionDate[11];
	uint8_t transState;
	float transAmount, maxTransAmount;
	uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[6];

	//Read all records.
	do{

		read = fscanf(file, 
							"%d, %10[^,], %f, %d, %f, %24[^,], %19[^,], %5[^\n]\n", 
							&transactionSequenceNumber, 
							transactionDate, 
							&transAmount,
							&transState,
							&maxTransAmount,
							cardHolderName,
							primaryAccountNumber,
							cardExpirationDate);

		//If fscanf has already read all the values from the file then we have already read another record successfully.
		if(read == 8) 
			{
				records++;
				// Printing the vaules to the console
				printf("\t#########################\n");
				printf("\tTransaction Sequence Number: %d\n", transactionSequenceNumber);
				printf("\tTransaction Date: %s\n", transactionDate);
				printf("\tTransaction Amount: %f\n", transAmount);
				printf("\tTransaction State: %d\n", transState);
				printf("\tTerminal Max Amount: %f\n", maxTransAmount);
				printf("\tCardholder Name: %s\n", cardHolderName);
				printf("\tPAN: %s\n", primaryAccountNumber);
				printf("\tCard Expiration Date: %s\n", cardExpirationDate);
				printf("\t#########################\n");}


	}while(!(feof(file)));

	if(records == 0){
		printf("\t############################################################\n");
		printf("\t#########    There are no transactions to list    ##########\n");
		printf("\t############################################################\n");
	}

	// Close the file after finish reading from it.
	fclose(file);

	// //Print the number of records read from the file.
	// printf("No. of records read is: %d records.\n", records);

	// // Print all the records that have been read from the file.
	// for(int i=0; i<records; i++){
	// 	printf("The account with the PAN: %s owned by: %s, its expiry date is: %s\n",
	// 																						transactionsDB[i].cardHolderData.primaryAccountNumber,
	// 																						transactionsDB[i].cardHolderData.cardHolderName,
	// 																						transactionsDB[i].cardHolderData.cardExpirationDate);
	// }

}

/************************************************************** Its test function *************************************************************************/

void listSavedTransactionsTest(void){
	listSavedTransactions();
}

/**********************************************************************************************************************************************************
 ***************************************************** Function to Update the accounts database ***********************************************************
 **********************************************************************************************************************************************************/

EN_transState_t updateAccountsDB(void){
	FILE *file;
	file = fopen("../Server/accountsDB.txt", "w");

	if(file == NULL){
		printf("\tUnable to open the file.\n");
		return INTERNAL_SERVER_ERROR;
	}
	
	fprintf(file, "%d", tranSequence);
	fputs("\n", file);

	int i = 0;
	while(accountsDB[i].primaryAccountNumber[0] != '\0')
	{
		fprintf(file, "%.1f", accountsDB[i].balance);
		fputs(", ", file);
		if(accountsDB[i].state == RUNNING)
			fputs("RUNNING", file);
		else
			fputs("BLOCKED", file);
		fputs(", ", file);
		fputs(accountsDB[i].primaryAccountNumber, file);
		fputs("\n", file);
		i++;
	}

	fclose(file);

	return SERVER_OK;

}


// int main(){
// 	// The following call is to read the database of the accounts from the file.
// 	getAccountsDB();

// 	// recieveTransactionDataTest();
// 	// isValidAccountTest();
// 	// isBlockedAccountTest();
// 	// isAmountAvailableTest();
// 	// saveTransactionTest();
// 	// listSavedTransactions();


// 	// ST_transaction_t transData;
// 	// strcpy(transData.cardHolderData.primaryAccountNumber, "031349698688737606");
// 	// transData.terminalData.transAmount = 500;
// 	// recieveTransactionData(&transData);

// 	// ST_transaction_t transData; 
// 	// transData.transactionSequenceNumber = 55;
// 	// strcpy(transData.terminalData.transactionDate, "29/06/1994");
// 	// transData.terminalData.transAmount = 55.0;
// 	// transData.terminalData.maxTransAmount = 100;
// 	// strcpy(transData.cardHolderData.cardHolderName, "Mahmoud Mostafa Tayee");
// 	// strcpy(transData.cardHolderData.primaryAccountNumber, "8989374615436851");
// 	// strcpy(transData.cardHolderData.cardExpirationDate, "06/26");
// 	// saveTransaction(&transData);


// }
