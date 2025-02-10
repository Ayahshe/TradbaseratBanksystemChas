#include "Bank.h"
#include "TransactionLogToJson.cpp"

void Client(Bank& bank, int clientId) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> accountDistribution(0, bank.getAccountNumbers().size() - 1);
    std::uniform_int_distribution<> amountDistribution(1, 100);
    std::uniform_int_distribution<> transactionDistribution(0, 1);
    std::uniform_int_distribution<> randInterval(10, 50);

    for (int i = 0; i < 5; ++i) {
        auto accountNumbers = bank.getAccountNumbers();
        if (accountNumbers.empty()) {
            continue;
        }

        int accountIndex = accountDistribution(gen);
        int accountNumber = accountNumbers[accountIndex];
        int amount = amountDistribution(gen);
        int transaction = transactionDistribution(gen);

        std::this_thread::sleep_for(std::chrono::milliseconds(randInterval(gen)));

        std::shared_ptr<BankAccount> account = bank.getAccount(accountNumber);
        if (!account) continue;

        if (transaction == 0) { // Insättning
            account->deposit(amount);
            logTransactionToJson(accountNumber, "deposit", amount, account->getBalance());
        }
        else { // Uttag
            account->withdraw(amount);
            logTransactionToJson(accountNumber, "withdraw", amount, account->getBalance());
        }
    }
}
