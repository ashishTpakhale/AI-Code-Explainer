#include "AppConfig.h"
#include <cstdlib>

AppConfig::AppConfig() {
    const char* envKey = std::getenv("GEMINI_API_KEY");
    if (envKey != nullptr) {
        apiKey = envKey;
    }
    modelName = "gemini-3.6-flash";
}

AppConfig& AppConfig::getInstance() {
    static AppConfig instance;
    return instance;
}

const std::string& AppConfig::getApiKey() const {
    return apiKey;
}

void AppConfig::setApiKey(const std::string& key) {
    apiKey = key;
}

bool AppConfig::hasApiKey() const {
    return !apiKey.empty();
}

const std::string& AppConfig::getModelName() const {
    return modelName;
}

void AppConfig::setModelName(const std::string& model) {
    modelName = model;
}
