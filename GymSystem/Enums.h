#ifndef ENUMS_H
#define ENUMS_H

// Перелік ролей користувачів
enum class Role {
    CLIENT,
    TRAINER,
    ADMIN
};

// Типи абонементів
enum class MembershipType {
    BASIC,
    PREMIUM,
    VIP
};

#endif