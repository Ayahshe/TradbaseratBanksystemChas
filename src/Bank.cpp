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
    std::cout << "Welcome to the Bank!\n";
}

void Bank::signalReportReady() {
    {
    std::lock_guard<std::mutex> lock(accountsMutex);
    readyToReport = true;
}
    reportCondition.notify_one();
}


void Bank::addAccount(const BankAccount& account)
{
std::lock_guard<std::mutex> lock(accountsMutex);

    // Create a dynamically allocated BankAccount object
    BankAccount* newAccount = new BankAccount(account.getAccountNumber(), 0);

    // Insert the account pointer into the map
    auto result = accounts.insert({account.getAccountNumber(), newAccount});
}

std::vector<int> Bank::getAccountNumbers() const {
    std::vector<int> accountNumbers;

    for(const auto& account : accounts) {
        accountNumbers.push_back(account.first);
    }

    return accountNumbers;
}

// Displays all accounst and their balances
void Bank::getAccountBalances() const
{
std::lock_guard<std::mutex> lock(accountsMutex);
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
