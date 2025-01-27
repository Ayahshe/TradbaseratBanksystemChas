#include "BankAccount.h"
#include <iostream>
#include <random>
#include <map>
/*#include <fstream>
#include "nlohmann/json.hpp"
#include <ctime>*/

//using json = nlohmann::json;

/* void BankAccount::logTransaction(const std::string& type, int amount) {
    std::lock_guard<std::mutex> lock(mtx); // Trådsäker

    json transaction = {
        {"accountNumber", accountNumber},
        {"type", type}, // deposit eller withdraw
        {"amount", amount},
        {"balance", balance}, // Nya saldot
        {"timestamp", std::time(nullptr)}
    };

    std::ofstream file("transactions.json", std::ios::app);
    if (file.is_open()) {
        file << transaction.dump() << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to open transactions.json for logging." << std::endl;
    }
} */

/*

Attribut: balance (int), accountNumber (int)

Metoder:
deposit(int amount)
withdraw(int amount)
getBalance()
*/

BankAccount::BankAccount(int accountNumber, int balance) : accountNumber(accountNumber), balance(balance) {};

// Insättning (går ej att sätta in <0 - saldo ökar vi insättning)
void BankAccount::deposit(int amount)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (amount < 0) {
        std::cout << "Attempting to deposit less than 0 from " << accountNumber << std::endl;
        return;
    };

    balance += amount;
    /*logTransaction("deposit", amount);
    std::cout << "Logging transaction: " << " Amount: " << amount << " Balance: " << balance << std::endl;*/
}

// Uttag (går ej att dra ut mindre än 0 och ej mer än vad man har - saldo minskar vid insättning)
int BankAccount::withdraw(int amount)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (amount < 0) {
        // std::cout << "Attempting to withdraw less than 0 from " << accountNumber << std::endl;
        return 1;
    };

    if (amount > balance) {
        // std::cout << "Insufficient funds, trying to withdraw " << amount << " from " << accountNumber << " but only " << balance << " is available!" << std::endl;
        return 1;
    };

    balance -= amount;
   /* logTransaction("withdraw", amount);
    std::cout << "Logging transaction: " << " Amount: " << amount << " Balance: " << balance << std::endl;*/

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

