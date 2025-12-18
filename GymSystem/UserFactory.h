#ifndef USERFACTORY_H
#define USERFACTORY_H

#include "User.h"
#include <memory>
#include <stdexcept>

// Патерн FACTORY METHOD
// SRP: Цей клас відповідає лише за створення (інкапсулює "new")
class UserFactory {
public:
    static std::shared_ptr<User> createUser(Role role, int id, std::string name, std::string email) {
        switch (role) {
            case Role::CLIENT:
                return std::make_shared<Client>(id, name, email);
            case Role::TRAINER:
                return std::make_shared<Trainer>(id, name, email);
            case Role::ADMIN:
                return std::make_shared<Admin>(id, name, email);
            default:
                throw std::invalid_argument("Unknown role");
        }
    }
};

#endif