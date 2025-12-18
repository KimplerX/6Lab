#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Enums.h"
#include "Models.h"

// Базовий абстрактний клас (LSP: підкласи замінюють User)
class User {
protected:
    int id;
    std::string name;
    std::string email;
    Role role;

public:
    User(int id, std::string name, std::string email, Role role)
        : id(id), name(name), email(email), role(role) {}

    virtual ~User() = default;

    std::string getName() const { return name; }
    Role getRole() const { return role; }
    
    // Абстрактний метод для відображення панелі керування
    virtual void displayDashboard() const = 0;
};

// Клієнт
class Client : public User {
    std::shared_ptr<Membership> membership;
    std::vector<int> bookedSessionIds;

public:
    Client(int id, std::string name, std::string email)
        : User(id, name, email, Role::CLIENT) {}

    void setMembership(std::shared_ptr<Membership> m) { membership = m; }
    
    bool hasActiveMembership() const { 
        return membership && membership->isValid(); 
    }

    void addBooking(int sessionId) { bookedSessionIds.push_back(sessionId); }

    void displayDashboard() const override {
        std::cout << "\n--- CLIENT DASHBOARD: " << name << " ---\n";
        if (hasActiveMembership()) 
            std::cout << "Status: Active Membership (ID: " << membership->getId() << ")\n";
        else 
            std::cout << "Status: No Active Membership\n";
        std::cout << "Booked Sessions: " << bookedSessionIds.size() << "\n";
    }
    
    // Getter для абонементу, щоб сервіси могли з ним працювати
    std::shared_ptr<Membership> getMembership() { return membership; }
};

// Тренер
class Trainer : public User {
public:
    Trainer(int id, std::string name, std::string email)
        : User(id, name, email, Role::TRAINER) {}

    void displayDashboard() const override {
        std::cout << "\n--- TRAINER DASHBOARD: " << name << " ---\n";
        std::cout << "Action: Check your schedule.\n";
    }
};

// Адміністратор
class Admin : public User {
public:
    Admin(int id, std::string name, std::string email)
        : User(id, name, email, Role::ADMIN) {}

    void displayDashboard() const override {
        std::cout << "\n--- ADMIN DASHBOARD: " << name << " ---\n";
        std::cout << "Action: Generate reports, Manage users.\n";
    }
};

#endif