#include <string>
#include <vector>
#include <optional>
#include "User.cpp"

// UserRepository manages persistence and retrieval of User entities.
class UserRepository {
private:
    std::vector<User> database;

public:
    UserRepository() {
        // Seed default test user credentials for demonstration
        database.emplace_back("usr_101", "alice", "hash_secret123", "ADMIN");
        database.emplace_back("usr_102", "bob", "hash_qwerty456", "DEVELOPER");
    }

    std::optional<User> findByUsername(const std::string& username) {
        for (const auto& user : database) {
            if (user.getUsername() == username) {
                return user;
            }
        }
        return std::nullopt;
    }
};
