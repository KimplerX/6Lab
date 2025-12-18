#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <ctime>
#include <iostream>
#include "Enums.h"

class Membership {
    int id;
    MembershipType type;
    bool isActive;
    std::time_t expireDate;

public:
    Membership(int id, MembershipType type) : id(id), type(type), isActive(true) {
        // Термін дії: поточний час + 30 днів
        expireDate = std::time(nullptr) + (30 * 24 * 60 * 60);
    }

    bool isValid() const { 
        return isActive && std::time(nullptr) < expireDate; 
    }
    
    // Метод призупинення (подовжує термін)
    void suspend(int days) {
        if (!isActive) return;
        expireDate += (days * 24 * 60 * 60);
        std::cout << "[System] Membership extended by " << days << " days.\n";
    }

    int getId() const { return id; }
};

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
    
    void bookSpot() { if (hasSpace()) bookedSpots++; }
    
    void cancelSpot() { if (bookedSpots > 0) bookedSpots--; }

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getTrainer() const { return trainerName; }
    int getAvailableSpots() const { return maxSpots - bookedSpots; }
};

#endif