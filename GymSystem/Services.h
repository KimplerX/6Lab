#ifndef SERVICES_H
#define SERVICES_H

#include <vector>
#include <memory>
#include <algorithm>
#include "User.h"
#include "Models.h"

// Сервіс розкладу (Gym Management)
class GymService {
    std::vector<std::shared_ptr<Session>> sessions;

public:
    // UseCase: Додавання заняття (Admin/Trainer)
    void addSession(std::shared_ptr<Session> session) {
        sessions.push_back(session);
    }

    // UseCase: Перегляд розкладу
    void showSchedule() const {
        std::cout << "\n>>> CURRENT GYM SCHEDULE <<<\n";
        for (const auto& s : sessions) {
            std::cout << "[ID: " << s->getId() << "] " << s->getName() 
                      << " | Trainer: " << s->getTrainer() 
                      << " | Spots Left: " << s->getAvailableSpots() << "\n";
        }
        std::cout << "----------------------------\n";
    }
};

// Сервіс бронювання (Booking Management)
class BookingManager {
public:
    // UseCase: Бронювання заняття
    bool bookSession(std::shared_ptr<Client> client, std::shared_ptr<Session> session) {
        if (!client->hasActiveMembership()) {
            std::cout << "[Booking] Failed: Client " << client->getName() << " has no active membership.\n";
            return false;
        }
        if (!session->hasSpace()) {
            std::cout << "[Booking] Failed: Session is full.\n";
            return false;
        }

        session->bookSpot();
        client->addBooking(session->getId());
        std::cout << "[Booking] Success: " << client->getName() << " booked spot in '" << session->getName() << "'.\n";
        return true;
    }
};

// Сервіс звітності (Report Generation)
class ReportGenerator {
public:
    // UseCase: Генерація звіту
    void generateReport() {
        std::cout << "\n[REPORT SYSTEM] Generating monthly activity report...\n";
        std::cout << " - Active Users: Calculating...\n";
        std::cout << " - Revenue: Calculating...\n";
        std::cout << "Report exported to PDF (simulated).\n";
    }
};

#endif