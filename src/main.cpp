#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include "Bank.h"

std::mutex coutMutex;

void printFromThread(const std::string& message) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << message << std::endl;
}

// Simulerar en kund som utför slumpmässiga transaktioner
void Client(Bank& bank, int clientId) {
    // Skapa en slumpgenerator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> accountDist(0, bank.getAccountNumbers().size() - 1); // doesn't work, since getAccountNumbers() just returns an int
    std::uniform_int_distribution<> amountDist(10, 100); // Slumpmässigt belopp
    std::uniform_int_distribution<> actionDist(0, 1); // 0 för insättning, 1 för uttag så att det ser fint ut med slumpmässiga insättningar och uttag

    for (int i = 0; i < 5; ++i) { // Varje kund gör 5 transaktioner
        int accountIndex = accountDist(gen);
        int amount = amountDist(gen);
        int action = actionDist(gen); // Väljer insättning eller uttag

        // Hämta kontonummer från banken
        auto accountNumbers = bank.getAccountNumbers();
        int accountNumber = accountNumbers[accountIndex];

        if (action == 0) { // Insättning
            // std::cout << "Customer " << clientId << " Transaction: Attempt. Depositing " << amount << " into " << accountNumber << std::endl;
            printFromThread("Customer " + std::to_string(clientId) + " Transaction: Attempt. Depositing " + std::to_string(amount) + " into " + std::to_string(accountNumber));

            bank.getAccount(accountNumber)->deposit(amount);
        } else { // Uttag
            // std::cout << "Customer " << clientId << " Transaction: Attempt. Withdrawing " << amount << " from " << accountNumber << std::endl;
            printFromThread("Customer " + std::to_string(clientId) + " Transaction: Attempt. Withdrawing " + std::to_string(amount) + " from " + std::to_string(accountNumber));

            bank.getAccount(accountNumber)->withdraw(amount);
        }
    }
}


int main(int argc, char* argv[])
{

    Bank bank;
    
    // Skapa 5 konton med olika startbalanser
    for (int i = 0; i < 5; ++i) {
        bank.addAccount(bank.generateAccountNumber());
    }

    // Skapa 5 kunder med trådar
    std::vector<std::thread> clients;
    for(int i = 0; i < 10; ++i) {
        clients.emplace_back(Client, std::ref(bank), i);
    }

    // Vänta på att alla trådar ska avslutas
    for(auto& client : clients) {
        client.join();
    }

    // Printa
    bank.getAccountBalances();    
    return 0;
}
