#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include "nlohmann/json.hpp"
#include <fstream>
#include <mutex>
#include <chrono>
#include "Bank.h"


std::mutex coutMutex;

void printFromThread(const std::string& message) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << message << std::endl;
}

void logTransactionToJson(int accountNumber, const std::string& type, int amount, int balance) {
    nlohmann::json transaction = {
        {"accountNumber", accountNumber},
        {"type", type},
        {"amount", amount},
        {"balance", balance},
        {"timestamp", std::time(nullptr)}
    };

    std::ofstream file("transactions.json", std::ios::app);
    if (file.is_open()) {
        file << transaction.dump(4) << std::endl;
    } else {
        std::cerr << "Failed to open transactions.json for logging." << std::endl;
    }

    // Färger och stil för konsolutskrift
    std::string color = (type == "deposit") ? "\033[32m" : "\033[31m"; // Grönt för deposit, rött för withdraw
    std::string bold = "\033[1m";
    std::string reset = "\033[0m";

    // Skriv ut med färger och fetstil
    std::cout << color
              << "Transaction: "
              << bold << "Account " << accountNumber << reset
              << ", Type: " << type
              << ", Amount: " << amount
              << ", Balance: " << balance
              << reset << std::endl;
}

void Client(Bank& bank, int clientId) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> accountDist(0, bank.getAccountNumbers().size() - 1);
    std::uniform_int_distribution<> amountDist(1, 100);
    std::uniform_int_distribution<> actionDist(0, 1);
    std::uniform_int_distribution<> randInterval(10, 50);

    for (int i = 0; i < 5; ++i) {
        auto accountNumbers = bank.getAccountNumbers();
        if (accountNumbers.empty()) {
            continue;
        }

        int accountIndex = accountDist(gen);
        int accountNumber = accountNumbers[accountIndex];
        int amount = amountDist(gen);
        int action = actionDist(gen);

        std::this_thread::sleep_for(std::chrono::milliseconds(randInterval(gen)));

        auto* account = bank.getAccount(accountNumber);
        if (!account) continue;

        if (action == 0) { // Insättning
            account->deposit(amount);
            logTransactionToJson(accountNumber, "deposit", amount, account->getBalance());
        } else { // Uttag
            account->withdraw(amount);
            logTransactionToJson(accountNumber, "withdraw", amount, account->getBalance());
        }
    }
}

int main() {
    Bank bank;

    // Skapa och lägg till 5 konton
    for (int i = 0; i < 5; ++i) {
        int accountNumber = bank.generateAccountNumber();
        BankAccount account(accountNumber, 100 * (i + 1)); // Skapa BankAccount-objekt
        bank.addAccount(account); // Lägg till kontot i banken
    }

    // Starta klienttrådar
    std::vector<std::thread> clients;
    for (int i = 0; i < 5; ++i) {
        clients.emplace_back(Client, std::ref(bank), i);
    }

    // Vänta på att alla trådar avslutas
    for (auto& client : clients) {
        client.join();
    }

    // Skriv ut saldon för alla konton
    bank.getAccountBalances();

    return 0;
}
