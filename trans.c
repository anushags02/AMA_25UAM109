// Improved Bank Account Management System using Random Access Files
// Features Added:
// 1. Add account
// 2. Update account
// 3. Delete account
// 4. Display all accounts
// 5. Search account
// 6. Generate accounts.txt
// 7. Input validation
// 8. Better file handling
// 9. Optimized loops and logic fixes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "credit.dat"
#define MAX_ACCOUNTS 100

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
void initializeFile(FILE *fPtr);
unsigned int enterChoice(void);

void addAccount(FILE *fPtr);
void updateAccount(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void listAccounts(FILE *fPtr);
void searchAccount(FILE *fPtr);
void generateTextFile(FILE *fPtr);

int main()
{
    FILE *cfPtr;

    // open binary file
    if ((cfPtr = fopen(FILE_NAME, "rb+")) == NULL)
    {
        // create file if not present
        cfPtr = fopen(FILE_NAME, "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be opened.\n");
            return 1;
        }

        initializeFile(cfPtr);
    }

    unsigned int choice;

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1:
            addAccount(cfPtr);
            break;

        case 2:
            updateAccount(cfPtr);
            break;

        case 3:
            deleteAccount(cfPtr);
            break;

        case 4:
            listAccounts(cfPtr);
            break;

        case 5:
            searchAccount(cfPtr);
            break;

        case 6:
            generateTextFile(cfPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    printf("\nProgram terminated successfully.\n");

    return 0;
}

// initialize file with empty records
void initializeFile(FILE *fPtr)
{
    struct clientData blankClient = {0, "", "", 0.0};

    rewind(fPtr);

    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
    }
}

// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n========= BANK ACCOUNT SYSTEM =========\n");
    printf("1 - Add New Account\n");
    printf("2 - Update Account\n");
    printf("3 - Delete Account\n");
    printf("4 - List All Accounts\n");
    printf("5 - Search Account\n");
    printf("6 - Generate accounts.txt\n");
    printf("7 - Exit\n");
    printf("Enter your choice: ");

    scanf("%u", &choice);

    return choice;
}

// add account
void addAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter Last Name: ");
    scanf("%14s", client.lastName);

    printf("Enter First Name: ");
    scanf("%9s", client.firstName);

    printf("Enter Balance: ");
    scanf("%lf", &client.balance);

    client.acctNum = accountNum;

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    fflush(fPtr);

    printf("Account added successfully.\n");
}

// update account
void updateAccount(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;
    double transaction;

    printf("Enter account number to update: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    printf("\nCurrent Account Details:\n");
    printf("Account No : %u\n", client.acctNum);
    printf("Name       : %s %s\n",
           client.firstName,
           client.lastName);
    printf("Balance    : %.2lf\n", client.balance);

    printf("\nEnter amount (+deposit / -withdraw): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    fflush(fPtr);

    printf("Updated balance: %.2lf\n", client.balance);
}

// delete account
void deleteAccount(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

    fflush(fPtr);

    printf("Account deleted successfully.\n");
}

// display all accounts
void listAccounts(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n================ ACCOUNT LIST ================\n");
    printf("%-10s %-15s %-15s %-10s\n",
           "Acc No",
           "Last Name",
           "First Name",
           "Balance");

    printf("------------------------------------------------------\n");

    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        fread(&client, sizeof(struct clientData), 1, fPtr);

        if (client.acctNum != 0)
        {
            printf("%-10u %-15s %-15s %-10.2lf\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// search specific account
void searchAccount(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;

    printf("Enter account number to search: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("\n===== ACCOUNT DETAILS =====\n");
    printf("Account Number : %u\n", client.acctNum);
    printf("Last Name      : %s\n", client.lastName);
    printf("First Name     : %s\n", client.firstName);
    printf("Balance        : %.2lf\n", client.balance);
}

// generate formatted text file
void generateTextFile(FILE *fPtr)
{
    FILE *writePtr;

    struct clientData client;

    writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL)
    {
        printf("Text file could not be created.\n");
        return;
    }

    rewind(fPtr);

    fprintf(writePtr,
            "%-10s %-15s %-15s %-10s\n",
            "Acc No",
            "Last Name",
            "First Name",
            "Balance");

    fprintf(writePtr,
            "------------------------------------------------------\n");

    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        fread(&client, sizeof(struct clientData), 1, fPtr);

        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-10u %-15s %-15s %-10.2lf\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt generated successfully.\n");
}
