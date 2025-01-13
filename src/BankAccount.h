#include <mutex>

#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

class BankAccount {
private:
    int accountNumber;
    int balance; // Eventuellt float?
    std::mutex mtx;
    int generateAccountNumber() const;
    
public:
    // Konstruktor
    BankAccount(int accountNumber, int balance = 0);
    
    // Metoder
    void deposit(int amount);
    void withdraw(int amount);
    int getBalance();
    int getAccountNumber() const;
};

#endif //BANKACCOUNT_H
