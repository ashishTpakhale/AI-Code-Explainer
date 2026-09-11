#include <string>

// User domain entity representing an authenticated or stored user.
class User {
private:
    std::string id;
    std::string username;
    std::string passwordHash;
    std::string role;

public:
    User(std::string id, std::string username, std::string passwordHash, std::string role)
        : id(std::move(id)), username(std::move(username)),
          passwordHash(std::move(passwordHash)), role(std::move(role)) {}

    const std::string& getId() const { return id; }
    const std::string& getUsername() const { return username; }
    const std::string& getPasswordHash() const { return passwordHash; }
    const std::string& getRole() const { return role; }
};
