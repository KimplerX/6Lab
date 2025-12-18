#ifndef USERFACTORY_H
#define USERFACTORY_H

#include "User.h"
#include <memory>
#include <stdexcept>

class UserFactory {
public:
    static std::shared_ptr<User> createUser(Role role, int id, std::string name, std::string email, std::string password) {
        switch (role) {
            case Role::CLIENT:
                return std::make_shared<Client>(id, name, email, password);
            case Role::TRAINER:
                return std::make_shared<Trainer>(id, name, email, password);
            case Role::ADMIN:
                return std::make_shared<Admin>(id, name, email, password);
            default:
                throw std::invalid_argument("Unknown role");
        }
    }
};

#endif