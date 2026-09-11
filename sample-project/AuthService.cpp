#include <string>
#include <iostream>
#include "UserRepository.cpp"

// AuthService encapsulates authentication logic, hashing, and token generation.
class AuthService {
private:
    UserRepository userRepo;

    // Internal helper to simulate password hash comparison
    bool verifyPasswordHash(const std::string& rawPassword, const std::string& storedHash) {
        // In this prototype, hashes are prefixed with "hash_"
        return ("hash_" + rawPassword) == storedHash;
    }

public:
    AuthService() = default;

    // Validates credentials against UserRepository and verifies password hash
    bool authenticate(const std::string& username, const std::string& password) {
        auto userOpt = userRepo.findByUsername(username);
        if (!userOpt.has_value()) {
            std::cout << "[AuthService] User not found: " << username << std::endl;
            return false;
        }

        const User& user = userOpt.value();
        if (verifyPasswordHash(password, user.getPasswordHash())) {
            std::cout << "[AuthService] Password verified successfully for: " << username << std::endl;
            return true;
        }

        std::cout << "[AuthService] Password validation failed for: " << username << std::endl;
        return false;
    }
};
