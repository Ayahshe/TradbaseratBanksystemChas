#ifndef BANK_H
#define BANK_H
#include "BankAccount.h"
#include <map>
#include <random>

class Bank
{
    private:
    std::map<int, BankAccount*> accounts;

    public:
    Bank(); 
    void addAccount(const BankAccount& account);
    std::vector<int> getAccountNumbers() const;
    void getAccountBalances() const;
    int getRandAccountNumber() const;
    int generateAccountNumber() const;
    BankAccount* getAccount(int accountNumber); // Hämta ett konto

};

#endif //BANK_H
