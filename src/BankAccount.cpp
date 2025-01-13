#include "BankAccount.h"
#include <iostream>
#include <random>
#include <map>

/*

Attribut: balance (int), accountNumber (int)

Metoder:
deposit(int amount)
withdraw(int amount)
getBalance()
*/

BankAccount::BankAccount(int accountNumber, int balance = 0) : accountNumber(accountNumber), balance(balance) {};

// Insättning (går ej att sätta in <0 - saldo ökar vi insättning)
void BankAccount::deposit(int amount)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (amount < 0) {
        std::cout << "Attempting to deposit less than 0 from " << accountNumber << std::endl;
        return;
    };

    amount += amount;
}

// Uttag (går ej att dra ut mindre än 0 och ej mer än vad man har - saldo minskar vid insättning)
void BankAccount::withdraw(int amount)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (amount < 0) {
        std::cout << "Attempting to withdraw less than 0 from " << accountNumber << std::endl;
        return;
    };

    if (amount > balance) {
        std::cout << "Insufficient funds, trying to withdraw " << amount << " from " << accountNumber << " but only " << balance << " is available!" << std::endl;
        return;
    };

    amount -= balance;
}

// Saldo
int BankAccount::getBalance() {
    std::lock_guard<std::mutex> lock(mtx);
    return balance;
}

// Hämta kontonummer
int BankAccount::getAccountNumber() const {
    return accountNumber;
}

