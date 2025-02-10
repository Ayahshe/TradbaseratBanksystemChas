#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <chrono>
#include "Bank.h"
#include "Client.cpp"

std::mutex coutMutex;

void printFromThread(const std::string& message) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << message << std::endl;
}

int main() {
    Bank bank;

    int totalAccounts(0);
    std::cout << "How many bank accounts do you want?" << std::endl;
    std::cin >> totalAccounts;
    if (!totalAccounts || totalAccounts < 1 || totalAccounts > 500) {
        std::cout << "Invalid account amount. Choose between 1-500." << std::endl;

        return 1;
    }

    int totalClients(0);
    std::cout << "How many clients do you want?" << std::endl;
    std::cin >> totalClients;
    if (!totalClients || totalClients < 1 || totalClients > 500) {
        std::cout << "Invalid client amount. Choose between 1-500." << std::endl;

        return 1;
    }

    // Skapa och lägg till 5 konton
    for (int i = 0; i < totalAccounts; ++i) {
        int accountNumber = bank.generateAccountNumber();
        BankAccount account(accountNumber, 100 * (i + 1)); // Skapa BankAccount-objekt
        bank.addAccount(account); // Lägg till kontot i banken
    }

    // Starta klienttrådar
    std::vector<std::thread> clients;
    for (int i = 0; i < totalClients; ++i) {
        clients.emplace_back(Client, std::ref(bank), i);
    }

    // Vänta på att alla trådar avslutas
    for (auto& client : clients) {
        client.join();
    }

    // Signalera att rapporten är redo
    bank.signalReportReady();

    std::thread reporter([&bank]() {
        bank.generateReport();
        });

    reporter.join(); // Vänta på att rapporterings-tråden avslutas

    // Skriv ut saldon för alla konton
    bank.getAccountBalances();

    return 0;
}
