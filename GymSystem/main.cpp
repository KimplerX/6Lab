#include <iostream>
#include <memory>
#include <string>
#include <ctime>     // Для srand
#include <cstdlib>   // Для rand
#include "UserFactory.h"
#include "Services.h"
#include "IPaymentStrategy.h"
#include "AuthService.h" 

int main() {
    srand(time(0)); // Ініціалізація генератора випадкових чисел

    // Ініціалізація сервісів
    GymService gymService;
    AuthService authService;
    BookingManager bookingManager;
    SalesManager salesManager;
    RequestManager requestManager;

    // --- SEED DATA (Тестові дані) ---
    gymService.addSession(std::make_shared<Session>(101, "Yoga", "Elena", 10));
    gymService.addSession(std::make_shared<Session>(102, "Boxing", "Tyson", 2));

    // Додаємо адміна та тренера (звичайні користувачі можуть реєструватися самі)
    authService.registerUser(Role::TRAINER, 2, "Coach John", "coach", "123");
    authService.registerUser(Role::ADMIN, 3, "Admin User", "admin", "123");

    // Змінна для генерації ID нових користувачів
    int nextUserId = 100;

    bool systemRunning = true;

    while (systemRunning) {
        std::cout << "\n=== GYM SYSTEM WELCOME ===\n";
        std::shared_ptr<User> currentUser = nullptr;

        while (!currentUser) {
            std::cout << "1. Login\n";
            std::cout << "2. Register (New Client)\n";
            std::cout << "3. Exit System\n> ";
            
            int startChoice;
            std::cin >> startChoice;

            if (startChoice == 1) {
                // --- ЛОГІН ---
                std::string email, pass;
                std::cout << "Email: "; std::cin >> email;
                std::cout << "Pass: "; std::cin >> pass;
                
                currentUser = authService.login(email, pass);
                if (!currentUser) std::cout << "Invalid credentials.\n";

            } else if (startChoice == 2) {
                // --- РЕЄСТРАЦІЯ ---
                std::string name, email, pass;
                std::cout << "Enter Name: "; std::cin >> name;
                std::cout << "Enter Email: "; std::cin >> email;
                std::cout << "Create Password: "; std::cin >> pass;

                // Реєструємо як Клієнта (Role::CLIENT)
                authService.registerUser(Role::CLIENT, nextUserId++, name, email, pass);
                std::cout << "Registration Successful! Please login now.\n";
                
            } else if (startChoice == 3) {
                systemRunning = false;
                break;
            }
        }

        if (!systemRunning) break;

        std::cout << "\nWelcome, " << currentUser->getName() << "!\n";

        // --- ДАЛІ ЙДЕ ВАШЕ СТАНДАРТНЕ МЕНЮ (без змін) ---
        
        // --- МЕНЮ КЛІЄНТА ---
        if (currentUser->getRole() == Role::CLIENT) {
            auto client = std::dynamic_pointer_cast<Client>(currentUser);
            int choice = 0;
            while (choice != 7) {
                std::cout << "\n--- CLIENT MENU ---\n";
                std::cout << "1. Dashboard & History\n2. Buy Membership\n3. Schedule\n";
                std::cout << "4. Book Class\n5. Cancel Booking\n6. Request Suspension\n7. Logout\n> ";
                std::cin >> choice;

                if (choice == 1) { client->displayDashboard(); client->showHistory(); }
                else if (choice == 2) {
                    std::cout << "Type (1.Basic, 2.Premium): "; int t; std::cin >> t;
                    MembershipType mt = (t==1)? MembershipType::BASIC : MembershipType::PREMIUM;
                    std::cout << "Pay (1.Card, 2.Cash): "; int p; std::cin >> p;
                    if(p==1) { auto s = std::make_unique<CardPayment>(); salesManager.buyMembership(client, mt, *s); }
                    else { auto s = std::make_unique<CashPayment>(); salesManager.buyMembership(client, mt, *s); }
                }
                else if (choice == 3) gymService.showSchedule();
                else if (choice == 4) {
                    int sid; std::cout << "Session ID: "; std::cin >> sid;
                    auto s = gymService.getSession(sid);
                    if(s) bookingManager.bookSession(client, s); else std::cout << "Not found.\n";
                }
                else if (choice == 5) {
                    int sid; std::cout << "Session ID to cancel: "; std::cin >> sid;
                    auto s = gymService.getSession(sid);
                    if(s) bookingManager.cancelSession(client, s); else std::cout << "Not found.\n";
                }
                else if (choice == 6) {
                    if(client->hasActiveMembership()) {
                        int d; std::cout << "Days to suspend: "; std::cin >> d;
                        requestManager.submitRequest(client, d);
                    } else std::cout << "No active membership.\n";
                }
            }
        }
        // --- МЕНЮ ПЕРСОНАЛУ ---
        else {
            int choice = 0;
            while (choice != 4) {
                std::string role = (currentUser->getRole() == Role::ADMIN) ? "ADMIN" : "TRAINER";
                std::cout << "\n--- STAFF MENU (" << role << ") ---\n";
                std::cout << "1. Add Session\n2. Show Schedule\n";
                if (currentUser->getRole() == Role::ADMIN) std::cout << "3. Admin Panel (Reports & Requests)\n";
                else std::cout << "3. (Admin Only)\n";
                std::cout << "4. Logout\n> ";
                std::cin >> choice;

                if (choice == 1) {
                    int id, spots; std::string n, t;
                    std::cout << "ID: "; std::cin >> id; std::cout << "Name: "; std::cin >> n;
                    std::cout << "Trainer: "; std::cin >> t; std::cout << "Spots: "; std::cin >> spots;
                    gymService.addSession(std::make_shared<Session>(id, n, t, spots));
                }
                else if (choice == 2) gymService.showSchedule();
                else if (choice == 3) {
                    if(currentUser->getRole() == Role::ADMIN) {
                        std::cout << "a. Generate Report\nb. Process Requests\n> ";
                        char sub; std::cin >> sub;
                        if(sub == 'a') { ReportGenerator rg; rg.generateReport(gymService); }
                        else if(sub == 'b') { requestManager.processRequests(); }
                    } else std::cout << "Access Denied.\n";
                }
            }
        }
    }
    return 0;
}