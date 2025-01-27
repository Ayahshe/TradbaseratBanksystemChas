#ifndef BANK_H
#define BANK_H
#include "BankAccount.h"
#include <map>
#include <random>
#include <mutex>

class Bank
{
    private:
    std::map<int, BankAccount*> accounts;
    mutable std::mutex accountsMutex;
    std::condition_variable reportCondition;
    bool readyToReport = false;

    public:
    Bank(); 
    void addAccount(const BankAccount& account);
    std::vector<int> getAccountNumbers() const;
    void getAccountBalances() const;
    int getRandAccountNumber() const;
    int generateAccountNumber() const;
    BankAccount* getAccount(int accountNumber); // Hämta ett konto
    void singalReportReady();
    void generateReport();

};

#endif //BANK_H
