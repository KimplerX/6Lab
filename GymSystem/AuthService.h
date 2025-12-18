#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "User.h"
#include "UserFactory.h"

class AuthService {
    std::vector<std::shared_ptr<User>> registeredUsers;

public:
    void registerUser(Role role, int id, std::string name, std::string email, std::string password) {
        auto newUser = UserFactory::createUser(role, id, name, email, password);
        registeredUsers.push_back(newUser);
    }

    std::shared_ptr<User> login(std::string email, std::string password) {
        for (auto& user : registeredUsers) {
            if (user->getEmail() == email && user->checkPassword(password)) {
                return user;
            }
        }
        return nullptr;
    }
};

#endif