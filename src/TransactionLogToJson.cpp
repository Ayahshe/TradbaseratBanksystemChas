#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

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
    }
    else {
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
