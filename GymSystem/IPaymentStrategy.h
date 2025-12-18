#ifndef IPAYMENTSTRATEGY_H
#define IPAYMENTSTRATEGY_H

#include <iostream>

// Патерн STRATEGY + Принцип OCP (Open/Closed Principle)
// Ми можемо додавати нові методи оплати без зміни існуючого коду.
class IPaymentStrategy {
public:
    virtual ~IPaymentStrategy() = default;
    virtual bool processPayment(double amount) = 0;
};

// Реалізація 1: Оплата карткою
class CardPayment : public IPaymentStrategy {
public:
    bool processPayment(double amount) override {
        std::cout << "[Payment] Processing CARD payment: $" << amount << "... Success.\n";
        return true;
    }
};

// Реалізація 2: Оплата готівкою
class CashPayment : public IPaymentStrategy {
public:
    bool processPayment(double amount) override {
        std::cout << "[Payment] Processing CASH payment: $" << amount << "... Success.\n";
        return true;
    }
};

#endif