#ifndef IPAYMENTSTRATEGY_H
#define IPAYMENTSTRATEGY_H

#include <iostream>

class IPaymentStrategy {
public:
    virtual ~IPaymentStrategy() = default;
    virtual bool processPayment(double amount) = 0;
};

class CardPayment : public IPaymentStrategy {
public:
    bool processPayment(double amount) override {
        std::cout << "[Payment] Processing CARD payment: $" << amount << "... Success.\n";
        return true;
    }
};

class CashPayment : public IPaymentStrategy {
public:
    bool processPayment(double amount) override {
        std::cout << "[Payment] Processing CASH payment: $" << amount << "... Success.\n";
        return true;
    }
};

#endif