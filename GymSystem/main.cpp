#include <iostream>
#include <memory>
#include "UserFactory.h"
#include "Services.h"
#include "IPaymentStrategy.h"

int main() {
    // 1. Налаштування системи
    GymService gymService;
    BookingManager bookingManager;
    ReportGenerator reportGenerator;

    std::cout << "=== GYM INFORMATION SYSTEM ===\n";

    // 2. Створення користувачів (Factory Pattern)
    auto u1 = UserFactory::createUser(Role::CLIENT, 1, "Alex Mindy", "alex@mail.com");
    auto u2 = UserFactory::createUser(Role::TRAINER, 2, "John Coach", "john@gym.com");
    
    // Кастинг, бо нам потрібні специфічні методи клієнта (setMembership)
    auto client = std::dynamic_pointer_cast<Client>(u1);

    // 3. Створення розкладу
    auto session1 = std::make_shared<Session>(101, "Crossfit", "John Coach", 2); // Лише 2 місця
    auto session2 = std::make_shared<Session>(102, "Yoga", "Elena V.", 10);
    
    gymService.addSession(session1);
    gymService.addSession(session2);
    gymService.showSchedule();

    // 4. Сценарій: Спроба бронювання БЕЗ абонементу
    bookingManager.bookSession(client, session1);

    // 5. Сценарій: Купівля абонементу (Strategy Pattern)
    std::cout << "\n--- Purchasing Membership ---\n";
    // Вибираємо стратегію (можна легко замінити на CashPayment)
    std::unique_ptr<IPaymentStrategy> payment = std::make_unique<CardPayment>();
    
    if (payment->processPayment(100.0)) {
        auto mem = std::make_shared<Membership>(999, MembershipType::VIP);
        client->setMembership(mem);
    }

    // 6. Сценарій: Успішне бронювання
    std::cout << "\n--- Retry Booking ---\n";
    bookingManager.bookSession(client, session1);
    bookingManager.bookSession(client, session1); // Бронюємо ще раз (заповнюємо групу)
    
    // 7. Сценарій: Група переповнена
    auto u3 = UserFactory::createUser(Role::CLIENT, 3, "New User", "new@mail.com");
    auto client2 = std::dynamic_pointer_cast<Client>(u3);
    // Даємо йому абонемент "читом"
    client2->setMembership(std::make_shared<Membership>(888, MembershipType::BASIC));
    
    bookingManager.bookSession(client2, session1); // Має бути помилка (Session full)

    // 8. Звітність та Дашборди (Polymorphism)
    std::cout << "\n--- System Checks ---\n";
    client->displayDashboard();
    u2->displayDashboard(); // Тренер
    reportGenerator.generateReport();

    return 0;
}