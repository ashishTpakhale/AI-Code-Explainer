#include <string>
#include <iostream>
#include "AuthService.cpp"

// AuthController is the entry-point controller handling incoming HTTP-like auth requests.
class AuthController {
private:
    AuthService authService;

public:
    AuthController() = default;

    // Handles incoming login requests by delegating to AuthService
    bool handleLogin(const std::string& username, const std::string& password) {
        std::cout << "[AuthController] Received login request for user: " << username << std::endl;
        
        // Delegate credential validation to AuthService
        bool isSuccess = authService.authenticate(username, password);
        if (isSuccess) {
            std::cout << "[AuthController] Login succeeded. Session initialized." << std::endl;
            return true;
        } else {
            std::cout << "[AuthController] Login rejected. Returning 401 Unauthorized." << std::endl;
            return false;
        }
    }
};
