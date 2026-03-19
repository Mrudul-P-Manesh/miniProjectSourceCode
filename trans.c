
// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    char phone[15];       // account phone number
    double balance;       // account balance
};                        // end structure clientData

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchByName(FILE *fPtr);
void calculateTotals(FILE *fPtr);
void wipeDatabase(FILE *fPtr);

int main(int argc, char *argv[])
{
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice

    // fopen opens the file; exits if file cannot be opened
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        // if file doesn't exist, create it
        if ((cfPtr = fopen("credit.dat", "wb+")) == NULL)
        {
            printf("%s: File could not be opened.\n", argv[0]);
            exit(-1);
        }
        else
        {
            // properly initialize 100 blank records so there's no random memory garbage
            struct clientData blankClient = {0, "", "", "", 0.0};
            int i;
            for (i = 1; i <= 100; ++i)
            {
                fwrite(&blankClient, sizeof(struct clientData), 1, cfPtr);
            }
            rewind(cfPtr); // set pointer back to start
        }
    }

    // enable user to specify action
    while ((choice = enterChoice()) != 8)
    {
        switch (choice)
        {
        // create text file from record file
        case 1:
            textFile(cfPtr);
            break;
        // update record
        case 2:
            updateRecord(cfPtr);
            break;
        // create record
        case 3:
            newRecord(cfPtr);
            break;
        // delete existing record
        case 4:
            deleteRecord(cfPtr);
            break;
        // search by name
        case 5:
            searchByName(cfPtr);
            break;
        // calculate bank totals
        case 6:
            calculateTotals(cfPtr);
            break;
        // wipe entire database
        case 7:
            wipeDatabase(cfPtr);
            break;
        // display if user does not select valid choice
        default:
            puts("Incorrect choice");
            break;
        } // end switch

        printf("\n");
        system("pause"); // wait for user before clearing screen again
    }     // end while

    fclose(cfPtr); // fclose closes the file
} // end main

// create formatted text file for printing
void textFile(FILE *readPtr)
{
    FILE *writePtr; // accounts.txt file pointer
    int result;     // used to test whether fread read any bytes
    // create clientData with default information
    struct clientData client = {0, "", "", "", 0.0};

    // fopen opens the file; exits if file cannot be opened
    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    } // end if
    else
    {
        rewind(readPtr); // sets pointer to beginning of file
        fprintf(writePtr, "%-6s%-16s%-11s%-16s%10s\n", "Acct", "Last Name", "First Name", "Phone", "Balance");

        // copy all records from random-access file into text file
        while (!feof(readPtr))
        {
            result = fread(&client, sizeof(struct clientData), 1, readPtr);

            // write single record to text file
            if (result != 0 && client.acctNum != 0)
            {
                fprintf(writePtr, "%-6d%-16s%-11s%-16s%10.2f\n", client.acctNum, client.lastName, client.firstName,
                        client.phone, client.balance);
            } // end if
        }     // end while

        fclose(writePtr); // fclose closes the file
    }                     // end else
} // end function textFile

// update balance in record
void updateRecord(FILE *fPtr)
{
    unsigned int account; // account number
    double transaction;   // transaction amount
    // create clientData with no information
    struct clientData client = {0, "", "", "", 0.0};

    // obtain number of account to update
    printf("%s", "Enter account to update ( 1 - 100 ): ");
    while (scanf("%d", &account) != 1)
    {
        while (getchar() != '\n'); // clear buffer
        printf("Invalid input. Enter account to update ( 1 - 100 ): ");
    }

    // move file pointer to correct record in file
    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    // read record from file
    fread(&client, sizeof(struct clientData), 1, fPtr);
    // display error if account does not exist
    if (client.acctNum == 0)
    {
        printf("Account #%d has no information.\n", account);
    }
    else
    { // update record
        printf("%-6d%-16s%-11s%-16s%10.2f\n\n", client.acctNum, client.lastName, client.firstName, client.phone, client.balance);

        // request transaction amount from user
        printf("%s", "Enter charge ( + ) or payment ( - ): ");
        while (scanf("%lf", &transaction) != 1)
        {
            while (getchar() != '\n'); // clear buffer
            printf("Invalid input. Enter charge ( + ) or payment ( - ): ");
        }
        client.balance += transaction; // update record balance

        printf("%-6d%-16s%-11s%-16s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.phone, client.balance);

        // move file pointer to correct record in file
        // move back by 1 record length
        fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
        // write updated record over old record in file
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
    } // end else
} // end function updateRecord

// delete an existing record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;                       // stores record read from file
    struct clientData blankClient = {0, "", "", "", 0}; // blank client
    unsigned int accountNum;                        // account number

    // obtain number of account to delete
    printf("%s", "Enter account number to delete ( 1 - 100 ): ");
    while (scanf("%d", &accountNum) != 1)
    {
        while (getchar() != '\n'); // clear buffer
        printf("Invalid input. Enter account number to delete ( 1 - 100 ): ");
    }

    // move file pointer to correct record in file
    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    // read record from file
    fread(&client, sizeof(struct clientData), 1, fPtr);
    // display error if record does not exist
    if (client.acctNum == 0)
    {
        printf("Account %d does not exist.\n", accountNum);
    } // end if
    else
    { // delete record
        // move file pointer to correct record in file
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        // replace existing record with blank record
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
    } // end else
} // end function deleteRecord

// create and insert record
void newRecord(FILE *fPtr)
{
    // create clientData with default information
    struct clientData client = {0, "", "", "", 0.0};
    unsigned int accountNum; // account number

    // obtain number of account to create
    printf("%s", "Enter new account number ( 1 - 100 ): ");
    while (scanf("%d", &accountNum) != 1)
    {
        while (getchar() != '\n'); // clear buffer
        printf("Invalid input. Enter new account number ( 1 - 100 ): ");
    }

    // move file pointer to correct record in file
    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    // read record from file
    fread(&client, sizeof(struct clientData), 1, fPtr);
    // display error if account already exists
    if (client.acctNum != 0)
    {
        printf("Account #%d already contains information.\n", client.acctNum);
    } // end if
    else
    { // create record
        // user enters last name, first name, phone and balance
        printf("%s", "Enter lastname, firstname, phone, balance\n? ");
        while (scanf("%14s%9s%14s%lf", client.lastName, client.firstName, client.phone, &client.balance) != 4)
        {
            while (getchar() != '\n'); // clear buffer
            printf("Invalid input. Enter lastname, firstname, phone, balance\n? ");
        }

        client.acctNum = accountNum;
        // move file pointer to correct record in file
        fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
        // insert record in file
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
    } // end else
} // end function newRecord

// search for an account by last name
void searchByName(FILE *fPtr)
{
    char searchName[15];
    struct clientData client = {0, "", "", "", 0.0};
    int found = 0;
    int result;

    printf("%s", "\nEnter last name to search: ");
    while (scanf("%14s", searchName) != 1)
    {
        while (getchar() != '\n'); // clear buffer
        printf("Invalid input. Enter last name to search: ");
    }

    // display headers
    printf("\n%-6s%-16s%-11s%-16s%10s\n", "Acct", "Last Name", "First Name", "Phone", "Balance");

    rewind(fPtr); // start at beginning of file
    while (!feof(fPtr))
    {
        result = fread(&client, sizeof(struct clientData), 1, fPtr);

        // check if record contains information
        if (result != 0 && client.acctNum != 0)
        {
            // compare last names (case-sensitive)
            if (strcmp(client.lastName, searchName) == 0)
            {
                printf("%-6d%-16s%-11s%-16s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.phone, client.balance);
                found = 1;
            }
        }
    } // end while

    if (!found)
    {
        printf("\nNo accounts found with last name \"%s\".\n", searchName);
    }
} // end function searchByName

// calculate bank totals and active accounts
void calculateTotals(FILE *fPtr)
{
    struct clientData client = {0, "", "", "", 0.0};
    int activeAccounts = 0;
    double totalBalance = 0.0;
    int result;

    rewind(fPtr); // start at beginning of file
    while (!feof(fPtr))
    {
        result = fread(&client, sizeof(struct clientData), 1, fPtr);

        // check if record contains information
        if (result != 0 && client.acctNum != 0)
        {
            activeAccounts++;
            totalBalance += client.balance;
        }
    } // end while

    printf("\n--- Bank Summary ---\n");
    printf("Total Active Accounts: %d\n", activeAccounts);
    printf("Total Bank Balance: $%.2f\n", totalBalance);
    printf("--------------------\n\n");
} // end function calculateTotals

// completely wipe the database and create 100 blank records
void wipeDatabase(FILE *fPtr)
{
    struct clientData blankClient = {0, "", "", "", 0.0};
    int i;
    char confirm[5];

    printf("\n%s", "!!! WARNING: THIS WILL DELETE ALL ACCOUNTS FOREVER !!!\n");
    printf("%s", "Type 'Y' and hit Enter to confirm, or any other key to cancel: ");
    
    // correctly read string confirmation
    while (scanf("%4s", confirm) != 1)
    {
        while (getchar() != '\n'); // clear buffer
    }

    if (confirm[0] == 'Y' || confirm[0] == 'y')
    {
        rewind(fPtr); // jump to beginning of credit.dat
        for (i = 1; i <= 100; ++i)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        }
        printf("\nDATABASE WIPED SUCCESSFULLY. 100 blank records created.\n");

        // wipe accounts.txt too
        FILE *txt = fopen("accounts.txt", "w");
        if (txt) {
            fclose(txt);
            printf("accounts.txt file has been cleared.\n");
        }
    }
    else
    {
        printf("\nWipe cancelled. Your accounts are safe.\n");
    }
} // end function wipeDatabase

// enable user to input menu choice
unsigned int enterChoice(void)
{
    unsigned int menuChoice; // variable to store user's choice

    system("cls"); // clear the console screen for dashboard feel

    // display available options
    printf("%s", "\n========================================\n"
                 "        BANK MANAGEMENT SYSTEM\n"
                 "========================================\n"
                 "1 - Store a formatted text file of accounts\n"
                 "    (\"accounts.txt\") for printing\n"
                 "2 - Update an account\n"
                 "3 - Add a new account\n"
                 "4 - Delete an account\n"
                 "5 - Search by name\n"
                 "6 - Calculate bank totals\n"
                 "7 - WIPE ENTIRE DATABASE\n"
                 "8 - End program\n"
                 "========================================\n"
                 "Enter your choice: ");

    if (scanf("%u", &menuChoice) != 1)
    {
        while (getchar() != '\n'); // clear buffer
        menuChoice = 0;            // force default (invalid) choice
    }
    return menuChoice;
} // end function enterChoice
