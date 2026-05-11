# Bank Account Management System in C

A simple **Bank Account Management System** developed in the C programming language using **Random Access Files** (`credit.dat`).

This project allows users to:
- Add new bank accounts
- Update account balances
- Delete accounts
- Display all accounts
- Search for accounts
- Generate a formatted text report

---

# Features

## 1. Add New Account
Create a new account by entering:
- Account Number
- Last Name
- First Name
- Balance

---

## 2. Update Existing Account
Modify account balance using:
- Positive value → Deposit
- Negative value → Withdrawal

---

## 3. Delete Account
Remove an account permanently from the file.

---

## 4. Display All Accounts
Shows all available account records in a formatted table.

---

## 5. Search Account
Search and display details of a specific account using account number.

---

## 6. Generate Text File
Creates a readable report file named:

`accounts.txt`

---

## 7. Input Validation
The program checks:
- Valid account numbers (1–100)
- Existing/non-existing accounts

---

## 8. Random Access File Handling
Uses binary file handling:
- `fopen()`
- `fseek()`
- `fread()`
- `fwrite()`

This improves performance and direct record access.

---

# Technologies Used

- C Programming
- File Handling
- Structures
- Random Access Files

---

# File Structure

## Main Files

| File Name | Description |
|------------|-------------|
| `main.c` | Source code |
| `credit.dat` | Binary database file |
| `accounts.txt` | Generated report file |

---

# Structure Used

```c
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};