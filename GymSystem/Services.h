#ifndef SERVICES_H
#define SERVICES_H

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "User.h"
#include "Models.h"
#include "IPaymentStrategy.h"

// Service 1: Розклад
class GymService {
    std::vector<std::shared_ptr<Session>> sessions;
public:
    void addSession(std::shared_ptr<Session> session) { sessions.push_back(session); }
    
    std::shared_ptr<Session> getSession(int id) {
        for (auto& s : sessions) if (s->getId() == id) return s;
        return nullptr;
    }

    const std::vector<std::shared_ptr<Session>>& getAllSessions() const { return sessions; }

    void showSchedule() const {
        std::cout << "\n>>> CURRENT GYM SCHEDULE <<<\n";
        for (const auto& s : sessions) {
            std::cout << "[ID: " << s->getId() << "] " << s->getName() 
                      << " | Trainer: " << s->getTrainer() 
                      << " | Spots Left: " << s->getAvailableSpots() << "\n";
        }
    }
};

// Service 2: Продажі
class SalesManager {
public:
    void buyMembership(std::shared_ptr<Client> client, MembershipType type, IPaymentStrategy& paymentStrategy) {
        double price = (type == MembershipType::BASIC) ? 30.0 : ((type == MembershipType::PREMIUM) ? 50.0 : 100.0);
        std::cout << "[Sales] Processing transaction ($" << price << ")...\n";
        if (paymentStrategy.processPayment(price)) {
            int newId = rand() % 9000 + 1000;
            client->setMembership(std::make_shared<Membership>(newId, type));
            std::cout << "[Sales] SUCCESS! Membership assigned.\n";
        } else {
            std::cout << "[Sales] FAILED.\n";
        }
    }
};

// Service 3: Бронювання
class BookingManager {
public:
    bool bookSession(std::shared_ptr<Client> client, std::shared_ptr<Session> session) {
        if (!client->hasActiveMembership()) {
            std::cout << "[Booking] Error: No active membership.\n";
            return false;
        }
        if (!session->hasSpace()) {
            std::cout << "[Booking] Error: Class full.\n";
            return false;
        }
        session->bookSpot();
        client->addBooking(session->getId());
        std::cout << "[Booking] Success!\n";
        return true;
    }

    bool cancelSession(std::shared_ptr<Client> client, std::shared_ptr<Session> session) {
        auto& bookings = client->getBookedSessionIds();
        auto it = std::find(bookings.begin(), bookings.end(), session->getId());
        if (it != bookings.end()) {
            bookings.erase(it);
            session->cancelSpot();
            std::cout << "[Booking] Cancelled.\n";
            return true;
        }
        std::cout << "[Booking] Error: Not booked.\n";
        return false;
    }
};

// --- СТРУКТУРА ТА МЕНЕДЖЕР ДЛЯ ЗАЯВОК (ADMIN APPROVAL) ---
struct SuspensionRequest {
    int id;
    std::shared_ptr<Client> client;
    int days;
    bool isProcessed;
};

class RequestManager {
    std::vector<SuspensionRequest> requests;
    int nextId = 1;

public:
    // Клієнт створює заявку
    void submitRequest(std::shared_ptr<Client> client, int days) {
        requests.push_back({nextId++, client, days, false});
        std::cout << "[System] Request submitted! Waiting for Admin approval.\n";
    }

    // Адмін обробляє заявки
    void processRequests() {
        std::cout << "\n--- PENDING REQUESTS ---\n";
        bool found = false;
        for (auto& req : requests) {
            if (!req.isProcessed) {
                found = true;
                std::cout << "Request #" << req.id << " from " << req.client->getName() 
                          << " (" << req.days << " days suspension). Approve? (y/n): ";
                char c; std::cin >> c;
                if (c == 'y' || c == 'Y') {
                    req.client->getMembership()->suspend(req.days);
                    req.isProcessed = true;
                    std::cout << "-> Approved.\n";
                } else {
                    req.isProcessed = true;
                    std::cout << "-> Rejected.\n";
                }
            }
        }
        if (!found) std::cout << "No pending requests.\n";
    }
};

// Service 5: Звіти
class ReportGenerator {
public:
    void generateReport(const GymService& gymService) {
        std::cout << "\n[ADMIN REPORT]\n";
        std::cout << "Date: " << __DATE__ << "\n";
        int totalSpots = 0;
        const auto& sessions = gymService.getAllSessions();
        std::cout << "Total Classes: " << sessions.size() << "\n";
        for(const auto& s : sessions) totalSpots += s->getAvailableSpots();
        std::cout << "Total Open Spots: " << totalSpots << "\n";
        std::cout << "Report End.\n";
    }
};

#endif