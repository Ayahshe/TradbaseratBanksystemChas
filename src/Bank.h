#ifndef BANK_H
#define BANK_H

#include "BankAccount.h"
#include <map>
#include <random>
#include <mutex>
#include <vector>
#include <condition_variable>

class Bank {
private:
    std::map<int, std::shared_ptr<BankAccount>> accounts;
    mutable std::mutex accountsMutex;  // Endast en deklaration
    std::condition_variable reportCondition;  // Endast en deklaration
    bool readyToReport = false;  // Endast en deklaration

public:
    Bank(); 
    void addAccount(const BankAccount& account);
    std::vector<int> getAccountNumbers() const;
    void getAccountBalances() const;
    int getRandAccountNumber() const;
    int generateAccountNumber() const;
    std::shared_ptr<BankAccount> getAccount(int accountNumber);
    void signalReportReady();  // Rättstavad metod
    void generateReport();
};

#endif // BANK_H