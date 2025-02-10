#include "BankAccount.h"
#include <iostream>
#include <random>
#include <map>

BankAccount::BankAccount(int accountNumber, int balance) : accountNumber(accountNumber), balance(balance) {};

// Insättning (går ej att sätta in <0 - saldo ökar vi insättning)
void BankAccount::deposit(int amount) {
    std::lock_guard<std::mutex> lock(mtx);

    if (amount < 0) {
        std::cout << "Attempting to deposit less than 0 from " << accountNumber << std::endl;
        return;
    };

    balance += amount;
}

// Uttag (går ej att dra ut mindre än 0 och ej mer än vad man har - saldo minskar vid insättning)
int BankAccount::withdraw(int amount) {
    std::lock_guard<std::mutex> lock(mtx);

    if (amount < 0) {
        return 1;
    };

    if (amount > balance) {
        return 1;
    };

    balance -= amount;

    return 0;
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
