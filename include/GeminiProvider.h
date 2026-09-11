#pragma once

#include "LLMProvider.h"
#include <string>

// Concrete LLMProvider connecting to Google Gemini API over HTTPS.
class GeminiProvider : public LLMProvider {
private:
    std::string apiKey;
    std::string modelName;

    std::string sendHttpRequest(const std::string& host, const std::string& path, const std::string& jsonBody);

public:
    GeminiProvider();
    GeminiProvider(std::string apiKey, std::string modelName);

    std::string generate(const std::string& prompt) override;
    std::string getName() const override;
};
