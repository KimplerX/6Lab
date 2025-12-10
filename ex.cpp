#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory> // Для smart pointers (керування пам'яттю)

using namespace std;

// --- Абстракції та Інтерфейси (DIP - Dependency Inversion Principle) ---

/**
 * @brief Інтерфейс для отримання сповіщень (Pattern: Observer)
 * Дозволяє будь-якому об'єкту отримувати повідомлення від системи.
 */
class INotifiable {
public:
    virtual ~INotifiable() = default;
    virtual void update(const string& message) = 0;
};

/**
 * @brief Абстрактний базовий клас Користувача.
 * Відповідає тільки за базові дані та ідентифікацію (SRP).
 */
class User : public INotifiable {
protected:
    int id;
    string name;
    string email;
    string password;

public:
    User(int id, string name, string email, string password)
        : id(id), name(name), email(email), password(password) {}

    virtual ~User() = default;

    int getId() const { return id; }
    string getEmail() const { return email; }
    string getName() const { return name; }
    
    bool verifyPassword(const string& inputPass) const {
        return password == inputPass;
    }

    // Реалізація методу Observer
    void update(const string& message) override {
        cout << "[Notification for " << name << "]: " << message << endl;
    }

    // Абстрактний метод для отримання ролі (поліморфізм)
    virtual string getRoleName() const = 0;
};

// --- Конкретні реалізації (LSP - Liskov Substitution Principle) ---

class Client : public User {
public:
    Client(int id, string n, string e, string p) : User(id, n, e, p) {}
    
    string getRoleName() const override { return "Client"; }
    
    // Специфічний метод клієнта
    void buyMembership(const string& type) {
        // Логіка покупки...
        update("You have successfully purchased " + type + " membership.");
    }
};

class Trainer : public User {
public:
    Trainer(int id, string n, string e, string p) : User(id, n, e, p) {}
    
    string getRoleName() const override { return "Trainer"; }
};

class Admin : public User {
public:
    Admin(int id, string n, string e, string p) : User(id, n, e, p) {}
    
    string getRoleName() const override { return "Administrator"; }
};

// --- Pattern: Factory Method (OCP - Open/Closed Principle) ---

enum UserType { CLIENT_TYPE, TRAINER_TYPE, ADMIN_TYPE };

/**
 * @brief Фабрика для створення користувачів.
 * Ізолює логіку створення об'єктів від бізнес-логіки.
 */
class UserFactory {
public:
    static unique_ptr<User> createUser(UserType type, int id, string name, string email, string password) {
        switch (type) {
            case CLIENT_TYPE: return make_unique<Client>(id, name, email, password);
            case TRAINER_TYPE: return make_unique<Trainer>(id, name, email, password);
            case ADMIN_TYPE: return make_unique<Admin>(id, name, email, password);
            default: throw runtime_error("Unknown user type");
        }
    }
};

// --- Бізнес-логіка ---

class Session {
private:
    int id;
    string title;
    int maxSpots;
    int bookedSpots;

public:
    Session(int id, string title, int capacity) 
        : id(id), title(title), maxSpots(capacity), bookedSpots(0) {}

    int getId() const { return id; }
    string getTitle() const { return title; }

    bool book() {
        if (bookedSpots < maxSpots) {
            bookedSpots++;
            return true;
        }
        return false;
    }

    string getInfo() const {
        return title + " (" + to_string(bookedSpots) + "/" + to_string(maxSpots) + ")";
    }
};

/**
 * @brief Сервіс бронювання.
 * Відповідає ЛИШЕ за логіку бронювання (SRP).
 */
class BookingService {
public:
    void bookSession(User& user, Session& session) {
        if (session.book()) {
            // Успіх: сповіщаємо користувача (використання Observer)
            user.update("Booking confirmed for session: " + session.getTitle());
        } else {
            // Невдача
            user.update("Booking failed. Session " + session.getTitle() + " is full.");
        }
    }
};

/**
 * @brief Головний контролер системи (Façade for the logic).
 */
class GymSystem {
private:
    vector<shared_ptr<User>> users; // Використовуємо shared_ptr для зручного керування життям об'єктів
    vector<Session> sessions;
    BookingService bookingService;
    int userIdCounter = 1;

public:
    // Реєстрація через Фабрику
    void registerUser(UserType type, string name, string email, string password) {
        auto newUser = UserFactory::createUser(type, userIdCounter++, name, email, password);
        users.push_back(move(newUser));
        cout << "System: Registered " << name << endl;
    }

    shared_ptr<User> login(string email, string password) {
        auto it = find_if(users.begin(), users.end(), [&](const shared_ptr<User>& u) {
            return u->getEmail() == email && u->verifyPassword(password);
        });

        if (it != users.end()) {
            cout << "System: Welcome back, " << (*it)->getName() << " (" << (*it)->getRoleName() << ")" << endl;
            return *it;
        }
        cout << "System: Login failed." << endl;
        return nullptr;
    }

    void addSession(string title, int capacity) {
        sessions.emplace_back(sessions.size() + 1, title, capacity);
    }

    // Демонстрація роботи
    void showSessions() const {
        cout << "\n--- Available Sessions ---" << endl;
        for (const auto& s : sessions) {
            cout << s.getInfo() << endl;
        }
        cout << "--------------------------\n" << endl;
    }

    // Отримання сесії за індексом (для тесту)
    Session* getSessionByIndex(int index) {
        if (index >= 0 && index < sessions.size()) {
            return &sessions[index];
        }
        return nullptr;
    }
    
    // Доступ до сервісу бронювання (Dependency Injection ready)
    BookingService& getBookingService() {
        return bookingService;
    }
};

// --- Main ---

int main() {
    GymSystem gym;

    // 1. Реєстрація (Використання Factory Pattern всередині)
    gym.registerUser(ADMIN_TYPE, "Ivan Admin", "admin@gym.com", "admin123");
    gym.registerUser(TRAINER_TYPE, "Petro Coach", "coach@gym.com", "fit123");
    gym.registerUser(CLIENT_TYPE, "Oleg Client", "oleg@gmail.com", "pass123");

    // 2. Створення контенту
    gym.addSession("Yoga Morning", 2);
    gym.addSession("Hardcore CrossFit", 1); // Тільки 1 місце

    // 3. Авторизація
    auto currentUser = gym.login("oleg@gmail.com", "pass123");

    if (currentUser) {
        gym.showSessions();

        // 4. Логіка бронювання (Використання Observer Pattern для сповіщення)
        Session* sessionToBook = gym.getSessionByIndex(1); // CrossFit
        
        if (sessionToBook) {
            // Поліморфний виклик: неважливо, хто саме бронює, головне що це User
            gym.getBookingService().bookSession(*currentUser, *sessionToBook);
        }
        
        // Спроба забронювати те саме заняття іншим юзером (імітація)
        cout << "\n(Another user tries to book the same full session...)" << endl;
        auto anotherUser = gym.login("coach@gym.com", "fit123"); // Тренер теж хоче потренуватися
        if(anotherUser && sessionToBook) {
             gym.getBookingService().bookSession(*anotherUser, *sessionToBook);
        }
    }

    // 5. Демонстрація специфічної поведінки (LSP/ISP check)
    // Ми перевіряємо тип динамічно, щоб викликати специфічний метод, 
    // не засмічуючи базовий інтерфейс User непотрібними методами.
    if (auto client = dynamic_pointer_cast<Client>(currentUser)) {
        cout << "\nSpecific Client Action:" << endl;
        client->buyMembership("Gold Pass");
    }

    return 0;
}