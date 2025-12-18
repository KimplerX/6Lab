#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Enums.h"
#include "Models.h"

class User {
protected:
    int id;
    std::string name;
    std::string email;
    std::string password;
    Role role;

public:
    User(int id, std::string name, std::string email, std::string password, Role role)
        : id(id), name(name), email(email), password(password), role(role) {}

    virtual ~User() = default;

    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    Role getRole() const { return role; }
    
    bool checkPassword(const std::string& inputPass) const {
        return this->password == inputPass;
    }
    
    virtual void displayDashboard() const = 0;
};

class Client : public User {
    std::shared_ptr<Membership> membership;
    std::vector<int> bookedSessionIds;

public:
    Client(int id, std::string name, std::string email, std::string password)
        : User(id, name, email, password, Role::CLIENT) {}

    void setMembership(std::shared_ptr<Membership> m) { membership = m; }
    
    bool hasActiveMembership() const { 
        return membership && membership->isValid(); 
    }

    std::shared_ptr<Membership> getMembership() { return membership; }

    void addBooking(int sessionId) { bookedSessionIds.push_back(sessionId); }
    
    // Посилання на вектор (щоб можна було видаляти елементи ззовні)
    std::vector<int>& getBookedSessionIds() { return bookedSessionIds; }

    void showHistory() const {
        std::cout << "\n--- Your Booking History (Session IDs) ---\n";
        if (bookedSessionIds.empty()) std::cout << "No bookings yet.\n";
        else {
            for (int id : bookedSessionIds) std::cout << "- Session ID: " << id << "\n";
        }
    }

    void displayDashboard() const override {
        std::cout << "\n--- CLIENT DASHBOARD: " << name << " ---\n";
        if (hasActiveMembership()) 
            std::cout << "Status: Active Membership (ID: " << membership->getId() << ")\n";
        else 
            std::cout << "Status: No Active Membership\n";
    }
};

class Trainer : public User {
public:
    Trainer(int id, std::string name, std::string email, std::string password)
        : User(id, name, email, password, Role::TRAINER) {}

    void displayDashboard() const override {
        std::cout << "\n--- TRAINER DASHBOARD: " << name << " ---\n";
    }
};

class Admin : public User {
public:
    Admin(int id, std::string name, std::string email, std::string password)
        : User(id, name, email, password, Role::ADMIN) {}

    void displayDashboard() const override {
        std::cout << "\n--- ADMIN DASHBOARD: " << name << " ---\n";
    }
};

#endif