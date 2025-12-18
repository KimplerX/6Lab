#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <ctime>
#include <iostream>
#include "Enums.h"

// Клас, що відповідає за абонемент (SRP: тільки дані про підписку)
class Membership {
    int id;
    MembershipType type;
    bool isActive;
    std::time_t expireDate;

public:
    Membership(int id, MembershipType type) : id(id), type(type), isActive(true) {
        // Логіка: термін дії +30 днів від створення
        expireDate = std::time(nullptr) + (30 * 24 * 60 * 60);
    }

    bool isValid() const { 
        return isActive && std::time(nullptr) < expireDate; 
    }
    
    // Призупинення абонементу (UseCase: Призупинення)
    void suspend(int days) {
        if (!isActive) return;
        // Проста імітація: подовжуємо дату, але логічно вважаємо неактивним на час паузи
        // Тут для спрощення просто додаємо дні до кінця терміну
        expireDate += (days * 24 * 60 * 60);
        std::cout << "[System] Membership suspended. Expiration date extended by " << days << " days.\n";
    }

    int getId() const { return id; }
};

// Клас заняття (SRP: дані про конкретне тренування)
class Session {
    int id;
    std::string name;
    std::string trainerName;
    int maxSpots;
    int bookedSpots;

public:
    Session(int id, std::string name, std::string trainer, int spots)
        : id(id), name(name), trainerName(trainer), maxSpots(spots), bookedSpots(0) {}

    bool hasSpace() const { return bookedSpots < maxSpots; }
    
    void bookSpot() {
        if (hasSpace()) bookedSpots++;
    }

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getTrainer() const { return trainerName; }
    int getAvailableSpots() const { return maxSpots - bookedSpots; }
};

#endif