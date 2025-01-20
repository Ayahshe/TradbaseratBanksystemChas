#include "Bank.h"
#include <iostream>
#include <random>
#include <map>
#include <cstdlib>
#include <ctime>
#include <functional>

/*

Hantera flera konton i en std::map<int, BankAccount>
Implementera metoder för att lägga till konton och hämta kontouppgifter

*/

Bank::Bank() {
    std::cout << "Välkommen till banken!/n";
    std::cout << "Konto nummer: " << getAccountNumbers();
} 

void Bank::addAccount(const BankAccount& account)
{
    std::cout << "Creating new bank account\nEnter account number and balance";
    // inserts new element & forwards => forwards arguments to constructor of element
    // accounts.emplace(account);
    accounts.emplace(account(generateAccountNumber()));
}

// Returns first account number found?
int Bank::getAccountNumbers() const 
{
    for(const auto& account : accounts)
    {
        if(account.first)
        {
            return account.first;
        } else {
           std:: cout << "\033[1;31mINVALID ACCOUNT POINTER FOR " << account.first << "\033[0m\n";
        }

    }

}

// Displays all accounst and their balances
void Bank::getAccountBalances() const
{
    for (const auto& [accountNumber, accPtr] : accounts) {
        if (accPtr) {
            std::cout << "\033[1;36m[" << accountNumber << "]\033[0m \033[1;32m= " << accPtr->getBalance() << "\033[0m" << std::endl;
        } else {
            std::cout << "\033[1;31mINVALID ACCOUNT POINTER FOR " << accountNumber << "\033[0m" << std::endl;
        }
    }
}

// Returns a random account number in the account map
int Bank::getRandAccountNumber() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, accounts.size() - 1);

    int randIdx = dist(gen);
    auto it = accounts.begin();
    std::advance(it, randIdx);

    return it->first;
}

int Bank::generateAccountNumber() const {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000000, 99999999);

    int accountNumber = dis(gen); // Tiosiffrig kontonummer

    // Generera ett unikt nummer
    do {
        accountNumber = dis(gen);
    } while (accounts.find(accountNumber) != accounts.end()); // Kontrollera om det redan finns

    return accountNumber;

}

BankAccount* Bank::getAccount(int accountNumber) {
    auto it = accounts.find(accountNumber);

    if (it != accounts.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}