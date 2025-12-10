#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

enum Role { CLIENT, TRAINER, ADMIN };

struct Date {
    int day, month, year;
    
    string toString() const {
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }
};

class Membership {
private:
    string type;
    Date expiryDate;
    bool isActive;

public:
    Membership() : isActive(false), expiryDate({0,0,0}) {}

    bool getIsActive() const { return isActive; }
    
    void activate(string newType, Date newExpiry) {
        type = newType;
        expiryDate = newExpiry;
        isActive = true;
        cout << "   [System]: Membership '" << type << "' activated until " << expiryDate.toString() << endl;
    }

    void suspend() {
        if (isActive) {
            isActive = false;
            cout << "   [System]: Membership suspended." << endl;
        }
    }
};

class User {
protected:
    int id;
    string name;
    string email;
    string password;
    Role role;
    string phone;

public:
    User(int id, string name, string email, string password, Role role, string phone)
        : id(id), name(name), email(email), password(password), role(role), phone(phone) {}

    virtual ~User() {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    Role getRole() const { return role; }

    bool checkPassword(string inputPassword) const {
        return password == inputPassword;
    }

    virtual void printRole() const {
        cout << "User";
    }
};

class Client : public User {
public:
    Membership membership;

    Client(int id, string name, string email, string password, string phone)
        : User(id, name, email, password, CLIENT, phone) {}

    void printRole() const override { cout << "Client"; }
};

class Trainer : public User {
public:
    Trainer(int id, string name, string email, string password, string phone)
        : User(id, name, email, password, TRAINER, phone) {}

    void printRole() const override { cout << "Trainer"; }
};

class Admin : public User {
public:
    Admin(int id, string name, string email, string password, string phone)
        : User(id, name, email, password, ADMIN, phone) {}

    void printRole() const override { cout << "Admin"; }
};

class Session {
private:
    int id;
    string name;
    string trainerName;
    string datetime;
    int maxSpots;
    int bookedSpots;

public:
    Session(int id, string name, string trainer, string date, int capacity)
        : id(id), name(name), trainerName(trainer), datetime(date), maxSpots(capacity), bookedSpots(0) {}

    int getId() const { return id; }
    string getName() const { return name; }
    
    bool hasSpace() const {
        return bookedSpots < maxSpots;
    }

    void bookSpot() {
        if (hasSpace()) bookedSpots++;
    }

    void display() const {
        cout << "ID: " << id << " | " << name << " (" << trainerName << ") | Date: " << datetime 
             << " | Spots: " << (maxSpots - bookedSpots) << "/" << maxSpots << endl;
    }
};

class Booking {
private:
    int id;
    int userId;
    int sessionId;

public:
    Booking(int id, int uId, int sId) : id(id), userId(uId), sessionId(sId) {}
};

class GymSystem {
private:
    vector<User*> users;
    vector<Session> sessions;
    vector<Booking> bookings;
    
    User* currentUser;
    
    int userIdCounter = 1;
    int sessionIdCounter = 1;
    int bookingIdCounter = 1;

public:
    GymSystem() : currentUser(nullptr) {}

    ~GymSystem() {
        for (User* u : users) {
            delete u;
        }
        users.clear();
    }

    void registerUser(string name, string email, string password, Role role, string phone) {
        User* newUser = nullptr;
        
        switch (role) {
            case CLIENT: newUser = new Client(userIdCounter++, name, email, password, phone); break;
            case TRAINER: newUser = new Trainer(userIdCounter++, name, email, password, phone); break;
            case ADMIN: newUser = new Admin(userIdCounter++, name, email, password, phone); break;
        }

        if (newUser) {
            users.push_back(newUser);
            cout << "[Registration]: ";
            newUser->printRole();
            cout << " " << name << " successfully added." << endl;
        }
    }

    bool login(string email, string password) {
        auto it = find_if(users.begin(), users.end(), [&](User* u) {
            return u->getEmail() == email && u->checkPassword(password);
        });

        if (it != users.end()) {
            currentUser = *it;
            cout << "\n[Login]: Welcome, " << currentUser->getName() << "!" << endl;
            return true;
        } else {
            cout << "\n[Error]: Invalid email or password." << endl;
            return false;
        }
    }

    void logout() {
        if (currentUser) {
            cout << "[Logout]: Goodbye, " << currentUser->getName() << ".\n" << endl;
            currentUser = nullptr;
        }
    }

    void buyMembership(string type) {
        if (currentUser && currentUser->getRole() == CLIENT) {
            Client* client = static_cast<Client*>(currentUser);
            client->membership.activate(type, {31, 12, 2025});
        } else {
            cout << "[Error]: Only clients can buy memberships." << endl;
        }
    }

    void addSession(string name, string date, int capacity) {
        if (currentUser && (currentUser->getRole() == TRAINER || currentUser->getRole() == ADMIN)) {
            sessions.push_back(Session(sessionIdCounter++, name, currentUser->getName(), date, capacity));
            cout << "[Schedule]: Session '" << name << "' added." << endl;
        } else {
            cout << "[Error]: Insufficient permissions." << endl;
        }
    }

    void viewSchedule() {
        cout << "\n--- CURRENT SCHEDULE ---" << endl;
        if (sessions.empty()) {
            cout << "Schedule is empty." << endl;
        } else {
            for (const auto& s : sessions) {
                s.display();
            }
        }
        cout << "------------------------" << endl;
    }

    void bookSession(int sessionId) {
        if (!currentUser || currentUser->getRole() != CLIENT) {
            cout << "[Error]: Only clients can book." << endl;
            return;
        }

        Client* client = static_cast<Client*>(currentUser);
        if (!client->membership.getIsActive()) {
            cout << "[Error]: No active membership!" << endl;
            return;
        }

        auto it = find_if(sessions.begin(), sessions.end(), [&](Session& s) {
            return s.getId() == sessionId;
        });

        if (it != sessions.end()) {
            if (it->hasSpace()) {
                it->bookSpot();
                bookings.push_back(Booking(bookingIdCounter++, client->getId(), sessionId));
                cout << "[Booking]: Successfully booked '" << it->getName() << "'." << endl;
            } else {
                cout << "[Error]: No spots available." << endl;
            }
        } else {
            cout << "[Error]: Session with ID " << sessionId << " not found." << endl;
        }
    }

    void generateReport() {
        if (currentUser && currentUser->getRole() == ADMIN) {
            cout << "\n=== ADMIN REPORT ===" << endl;
            cout << "Total users: " << users.size() << endl;
            cout << "Total sessions: " << sessions.size() << endl;
            cout << "Total bookings: " << bookings.size() << endl;
            cout << "====================" << endl;
        } else {
            cout << "[Error]: Access denied." << endl;
        }
    }
};

int main() {
    GymSystem system;

    cout << "--- 1. REGISTRATION ---" << endl;
    system.registerUser("Ivan Admin", "admin", "1111", ADMIN, "000");
    system.registerUser("Peter Trainer", "coach", "2222", TRAINER, "123");
    system.registerUser("Oleg Client", "client", "3333", CLIENT, "456");

    if (system.login("coach", "2222")) {
        system.addSession("Yoga", "12.12.2025 10:00", 2);
        system.addSession("Boxing", "12.12.2025 18:00", 10);
        system.logout();
    }

    if (system.login("client", "3333")) {
        system.viewSchedule();

        cout << "\n(Attempting to book without membership):" << endl;
        system.bookSession(1); 

        cout << "\n(Buying membership):" << endl;
        system.buyMembership("Gold Gym Pass");

        cout << "\n(Booking again):" << endl;
        system.bookSession(1); 
        system.bookSession(2); 
        
        system.viewSchedule();
        
        system.logout();
    }

    if (system.login("admin", "1111")) {
        system.generateReport();
        system.logout();
    }

    return 0;
}