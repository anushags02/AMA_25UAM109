// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.
// ADDED: Input validation & error handling, Transaction history log
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define LOG_FILE "transactions.log"

// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
};                        // end structure clientData

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void logTransaction(unsigned int acctNum, const char *firstName,
                    const char *lastName, double amount, double newBalance);
int  isValidAccountNum(unsigned int acctNum);
void clearInputBuffer(void);

int main(int argc, char *argv[])
{
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(-1);
    }

    while ((choice = enterChoice()) != 5)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        default:
            puts("Incorrect choice. Please enter a number between 1 and 5.");
            break;
        }
    }

    fclose(cfPtr);
    puts("Program ended. Goodbye!");
    return 0;
}

// ADDED: Validate account number is within 1-100
int isValidAccountNum(unsigned int acctNum)
{
    return (acctNum >= 1 && acctNum <= MAX_ACCOUNTS);
}

// ADDED: Clear leftover characters in input buffer
void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ADDED: Log every transaction to transactions.log with timestamp
void logTransaction(unsigned int acctNum, const char *firstName,
                    const char *lastName, double amount, double newBalance)
{
    FILE *logPtr = fopen(LOG_FILE, "a");
    if (logPtr == NULL)
    {
        puts("Warning: Could not open transaction log.");
        return;
    }

    time_t now = time(NULL);
    char timestamp[26];
    strncpy(timestamp, ctime(&now), 25);
    timestamp[24] = '\0';

    fprintf(logPtr,
            "[%s] Acct#%-4u | %-10s %-15s | "
            "Transaction: %+.2f | New Balance: %.2f\n",
            timestamp, acctNum, firstName, lastName, amount, newBalance);

    fclose(logPtr);
}

void textFile(FILE *readPtr)
{
    FILE *writePtr;
    int result;
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        rewind(readPtr);
        fprintf(writePtr, "%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

        while (!feof(readPtr))
        {
            result = fread(&client, sizeof(struct clientData), 1, readPtr);
            if (result != 0 && client.acctNum != 0)
            {
                fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                        client.acctNum, client.lastName,
                        client.firstName, client.balance);
            }
        }

        fclose(writePtr);
        puts("accounts.txt has been successfully created.");
    }
}

void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    int scanResult;
    struct clientData client = {0, "", "", 0.0};

    printf("%s", "Enter account to update ( 1 - 100 ): ");
    scanResult = scanf("%u", &account);
    clearInputBuffer();

    // ADDED: Validate numeric input
    if (scanResult != 1)
    {
        puts("Invalid input. Please enter a numeric account number.");
        return;
    }

    // ADDED: Validate account number range
    if (!isValidAccountNum(account))
    {
        printf("Error: Account number must be between 1 and %d.\n", MAX_ACCOUNTS);
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account #%u has no information.\n", account);
    }
    else
    {
        printf("%-6d%-16s%-11s%10.2f\n\n",
               client.acctNum, client.lastName,
               client.firstName, client.balance);

        printf("%s", "Enter charge ( + ) or payment ( - ): ");
        scanResult = scanf("%lf", &transaction);
        clearInputBuffer();

        // ADDED: Validate transaction is numeric
        if (scanResult != 1)
        {
            puts("Invalid input. Transaction amount must be a number.");
            return;
        }

        // ADDED: Balance overflow guard
        if (client.balance + transaction < -1000000.0)
        {
            puts("Error: Transaction would result in an invalid balance. Aborted.");
            return;
        }

        client.balance += transaction;

        printf("%-6d%-16s%-11s%10.2f\n",
               client.acctNum, client.lastName,
               client.firstName, client.balance);

        fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        // ADDED: Log the transaction
        logTransaction(client.acctNum, client.firstName,
                       client.lastName, transaction, client.balance);
        puts("Transaction logged successfully.");
    }
}

void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0};
    unsigned int accountNum;
    int scanResult;
    char confirm; // ADDED: confirmation variable

    printf("%s", "Enter account number to delete ( 1 - 100 ): ");
    scanResult = scanf("%u", &accountNum);
    clearInputBuffer();

    // ADDED: Validate numeric input
    if (scanResult != 1)
    {
        puts("Invalid input. Please enter a numeric account number.");
        return;
    }

    // ADDED: Validate account number range
    if (!isValidAccountNum(accountNum))
    {
        printf("Error: Account number must be between 1 and %d.\n", MAX_ACCOUNTS);
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account %u does not exist.\n", accountNum);
    }
    else
    {
        // ADDED: Show record and ask for confirmation before deleting
        printf("Account to delete:\n");
        printf("%-6d%-16s%-11s%10.2f\n\n",
               client.acctNum, client.lastName,
               client.firstName, client.balance);
        printf("Are you sure you want to delete this account? (y/n): ");

        scanResult = scanf(" %c", &confirm);
        clearInputBuffer();

        if (scanResult != 1 || (confirm != 'y' && confirm != 'Y'))
        {
            puts("Deletion cancelled.");
            return;
        }

        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

        // ADDED: Log the deletion
        logTransaction(client.acctNum, client.firstName,
                       client.lastName, 0.0, 0.0);
        printf("Account #%u has been deleted and the action was logged.\n", accountNum);
    }
}

void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;
    int scanResult;

    printf("%s", "Enter new account number ( 1 - 100 ): ");
    scanResult = scanf("%u", &accountNum);
    clearInputBuffer();

    // ADDED: Validate numeric input
    if (scanResult != 1)
    {
        puts("Invalid input. Please enter a numeric account number.");
        return;
    }

    // ADDED: Validate account number range
    if (!isValidAccountNum(accountNum))
    {
        printf("Error: Account number must be between 1 and %d.\n", MAX_ACCOUNTS);
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account #%d already contains information.\n", client.acctNum);
    }
    else
    {
        printf("%s", "Enter lastname, firstname, balance\n? ");
        scanResult = scanf("%14s%9s%lf",
                           client.lastName, client.firstName, &client.balance);
        clearInputBuffer();

        // ADDED: Validate all three fields were entered
        if (scanResult != 3)
        {
            puts("Invalid input. Please enter last name, first name, and balance.");
            return;
        }

        // ADDED: Reject negative initial balance
        if (client.balance < 0.0)
        {
            puts("Error: Initial balance cannot be negative. Record not created.");
            return;
        }

        client.acctNum = accountNum;
        fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        // ADDED: Log new account creation
        logTransaction(client.acctNum, client.firstName,
                       client.lastName, client.balance, client.balance);
        printf("Account #%u created successfully and logged.\n", accountNum);
    }
}

unsigned int enterChoice(void)
{
    unsigned int menuChoice;
    int scanResult;

    printf("%s", "\nEnter your choice\n"
                 "1 - store a formatted text file of accounts called\n"
                 "    \"accounts.txt\" for printing\n"
                 "2 - update an account\n"
                 "3 - add a new account\n"
                 "4 - delete an account\n"
                 "5 - end program\n? ");

    // ADDED: Validate menu input is numeric
    scanResult = scanf("%u", &menuChoice);
    clearInputBuffer();

    if (scanResult != 1)
    {
        puts("Invalid input. Please enter a number between 1 and 5.");
        return 0;
    }

    return menuChoice;
}