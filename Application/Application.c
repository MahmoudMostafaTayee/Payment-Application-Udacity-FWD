#include"Application.h"

/**********************************************************************************************************************************************************
 ******************* Function acts as the interface between all other functions to accomplish the task of the application *********************************
 **********************************************************************************************************************************************************/

void printModule(MODULE_IDENTIFIER moduleIdentifier){
         printf("\n\n\n\n\n\n\t*******************************************************************************************************\n");
         printf("\t*******************************                                         *******************************\n");
         printf("\t*******************************           Payment Application           *******************************\n");
   switch(moduleIdentifier){
      case(START_MODULE):
         printf("\t*******************************                                         *******************************\n");
         printf("\t*******************************************************************************************************\n\n");
         break;
      case(ANOTHER_TRANSACTION):
         printf("\t*******************************                                         *******************************\n");
         printf("\t*******************************************************************************************************\n\n");
         printf("\tType q or Q to quit then click enter.\n");
         printf("\tOr if you want to make another transaction type any other letter.\n\n\t");
         break;
      case(CARD_MODULE):
         printf("\t*******************************              Card Module                *******************************\n");
         printf("\t*******************************                                         *******************************\n");
         printf("\t*******************************************************************************************************\n\n");
         break;
      case(TERMINAL_MODULE):
         printf("\t*******************************           Terminal Module               *******************************\n");
         printf("\t*******************************                                         *******************************\n");
         printf("\t*******************************************************************************************************\n");
         break;
      case(SERVER_MODULE):
         printf("\t*******************************             Server Module               *******************************\n");
         printf("\t*******************************                                         *******************************\n");
         printf("\t*******************************************************************************************************\n\n");
         break;
      case(END_MODULE):
         printf("\t*******************************      Thanks for using our service       *******************************\n");
         printf("\t*******************************                                         *******************************\n");
         printf("\t*******************************************************************************************************\n\n");
         break;
         
   }
}


void appStart(void){
   MODULE_IDENTIFIER moduleIdentifier;

   /********************************************************** The Card module part ***********************************************************************/
   moduleIdentifier = CARD_MODULE;
   ST_cardData_t cardData;
   system("cls");
   printModule(moduleIdentifier);
   if(getCardHolderName(&cardData) == WRONG_NAME) // Get the card holder name.
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }
   
   system("cls");
   printModule(moduleIdentifier);
   if(getCardExpiryDate(&cardData) == WRONG_EXP_DATE) // Get the expiry date.
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }
   
   system("cls");
   printModule(moduleIdentifier);
   if(getCardPAN(&cardData) == WRONG_PAN) // Get the card PAN.
        {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }

   /********************************************************** The Terminal module part ********************************************************************/
   moduleIdentifier = TERMINAL_MODULE;
   ST_terminalData_t termData;

   system("cls");
   printModule(moduleIdentifier);
   if(isValidCardPAN(&cardData) == INVALID_CARD)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }

   // system("cls");
   // printModule(moduleIdentifier);
   if(getTransactionDate(&termData) == WRONG_DATE)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }

   // system("cls");
   // printModule(moduleIdentifier);
   if(isCardExpired(&cardData, &termData) == EXPIRED_CARD)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }
   
   // system("cls");
   // printModule(moduleIdentifier);
   if(setMaxAmount(&termData, 8000) == INVALID_MAX_AMOUNT)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }

   // system("cls");
   // printModule(moduleIdentifier);
   if(getTransactionAmount(&termData) == INVALID_AMOUNT)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }

   // system("cls");
   // printModule(moduleIdentifier);
   if(isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }

   /************************************************************ The Server module part ********************************************************************/
    moduleIdentifier = SERVER_MODULE;

   // The following call is to read the database of the accounts from the file.

   // Get the data from the terminal.
   ST_transaction_t transData;
   strcpy(transData.cardHolderData.cardHolderName, cardData.cardHolderName);
   strcpy(transData.cardHolderData.primaryAccountNumber, cardData.primaryAccountNumber);
   strcpy(transData.cardHolderData.cardExpirationDate, cardData.cardExpirationDate);

   transData.terminalData.transAmount = termData.transAmount;
   transData.terminalData.maxTransAmount = termData.maxTransAmount;
   strcpy(transData.terminalData.transactionDate, termData.transactionDate);




   system("cls");
   printModule(moduleIdentifier);
   if(getAccountsDB(&transData) == ACCOUNT_NOT_FOUND)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }

   transData.transState = recieveTransactionData(&transData);

   // system("cls");
   // printModule(moduleIdentifier);
   if(transData.transState != APPROVED)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }

   // system("cls");
   // printModule(moduleIdentifier);
   if(saveTransaction(&transData) == INTERNAL_SERVER_ERROR)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }
   

   if(updateAccountsDB() == INTERNAL_SERVER_ERROR)
      {
         printf("\tPlease click enter to continue!\n");
         getchar();
         return;
      }

   printf("\tYour transaction has been accomplished successfully.\n");
   printf("\tClick enter to continue!\n");
   getchar();

}

void multiModules(){
   MODULE_IDENTIFIER moduleIdentifier;
   uint8_t choice;

   moduleIdentifier = START_MODULE;
   system("cls");
   printModule(moduleIdentifier);
   printf("\tPlease choose one of the following options: \n");
   printf("\t1- Make a transaction.\n");
   printf("\t2- List all transactions.\n");
   printf("\t3- quit.\n");
   printf("\tWrite 1, 2 or 3, according to your choosed option, then click enter: ");
   scanf(" %d", &choice);
   getchar();

   uint8_t flag[5]; //Intialized with any other letter than 'q' or 'Q'
   if(choice == 1)
      {

         do{
               appStart();
               moduleIdentifier = ANOTHER_TRANSACTION;
               system("cls");
               printModule(moduleIdentifier);
               scanf(" %s", flag);
               getchar();
            }while(strcmp(flag, "q") && strcmp(flag, "Q"));
            moduleIdentifier = END_MODULE;
            system("cls");
            printModule(moduleIdentifier);
      }

   else if(choice == 2)
      listSavedTransactions();

   else if(choice == 3)
   {
      moduleIdentifier = END_MODULE;
      system("cls");
      printModule(moduleIdentifier);
      return;
   }

   else
      printf("You entered a non valid option, please choose 1, 2 or 3.\n");
}
 
int main(){
   multiModules();
   // appStart();

}