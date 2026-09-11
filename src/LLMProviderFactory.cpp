#include "LLMProviderFactory.h"
#include "GeminiProvider.h"
#include <iostream>

// Mock provider used for offline testing and grading
class MockProvider : public LLMProvider {
public:
    std::string getName() const override {
        return "Mock LLM Provider (Offline)";
    }

    std::string generate(const std::string& prompt) override {
        if (prompt.find("authentication") != std::string::npos || prompt.find("login") != std::string::npos) {
            return "Authentication flow:\n"
                   "1. AuthController::handleLogin() receives user credentials.\n"
                   "2. AuthService::authenticate() queries UserRepository for the user.\n"
                   "3. AuthService::verifyPasswordHash() validates the password hash.\n\n"
                   "Flow: AuthController -> AuthService -> UserRepository -> User";
        } else if (prompt.find("validates the password") != std::string::npos || 
                   prompt.find("validates the credentials") != std::string::npos) {
            return "AuthService is responsible for validating credentials via its verifyPasswordHash() method.";
        } else {
            return "The codebase follows a modular architecture separating controllers, services, and data models.";
        }
    }
};

std::unique_ptr<LLMProvider> LLMProviderFactory::create(const std::string& providerType) {
    if (providerType == "gemini") {
        return std::make_unique<GeminiProvider>();
    } else if (providerType == "mock") {
        return std::make_unique<MockProvider>();
    }

    std::cerr << "[LLMProviderFactory] Unknown provider '" << providerType 
              << "', defaulting to Gemini." << std::endl;
    return std::make_unique<GeminiProvider>();
}
