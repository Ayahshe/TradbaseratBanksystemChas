#include "Bank.h"
#include <iostream>
#include <random>
#include <map>
#include <cstdlib>
#include <ctime>
#include <functional>

Bank::Bank() {
    std::cout << "Welcome to the Bank!\n";
}

void Bank::signalReportReady() {
    {
        std::lock_guard<std::mutex> lock(accountsMutex); // Synkronisera åtkomst till flaggan
        readyToReport = true;
    }
    reportCondition.notify_one();
}

void Bank::generateReport() {
    std::unique_lock<std::mutex> lock(accountsMutex);

    // Vänta på att flaggan sätts till true
    reportCondition.wait(lock, [this] { return readyToReport; });

    // När signalen tas emot, skriv ut rapporten
    std::cout << "Generating report:" << std::endl;
    for (const auto& [accountNumber, account] : accounts) {
        if (account) {
            std::cout << "Account " << accountNumber 
                      << ", Balance: " << account->getBalance() 
                      << std::endl;
        }
    }

    readyToReport = false; // Återställ flaggan
}

void Bank::addAccount(const BankAccount& account) {
    std::lock_guard<std::mutex> lock(accountsMutex);

    // Skapa ett dynamiskt allokerat BankAccount-objekt
    BankAccount* newAccount = new BankAccount(account.getAccountNumber(), 0);

    // Sätt in kontopointern i map:en
    auto result = accounts.insert({account.getAccountNumber(), newAccount});
}

std::vector<int> Bank::getAccountNumbers() const {
    std::vector<int> accountNumbers;

    for(const auto& account : accounts) {
        accountNumbers.push_back(account.first);
    }

    return accountNumbers;
}

void Bank::getAccountBalances() const {
    std::lock_guard<std::mutex> lock(accountsMutex);
    for (const auto& [accountNumber, accPtr] : accounts) {
        if (accPtr) {
            std::cout << "\033[1;36m[" << accountNumber << "]\033[0m \033[1;32m= " << accPtr->getBalance() << "\033[0m" << std::endl;
        } else {
            std::cout << "\033[1;31mINVALID ACCOUNT POINTER FOR " << accountNumber << "\033[0m" << std::endl;
        }
    }
}

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

    int accountNumber;

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