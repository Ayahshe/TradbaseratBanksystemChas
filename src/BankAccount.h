#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H
#include <mutex>

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
    int withdraw(int amount);
    int getBalance();
    int getAccountNumber() const;

};

#endif //BANKACCOUNT_H
