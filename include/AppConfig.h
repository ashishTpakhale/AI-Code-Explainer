#pragma once

#include <string>

// Singleton class managing application-wide configuration and credentials.
class AppConfig {
private:
    std::string apiKey;
    std::string modelName;

    AppConfig();

public:
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;
    AppConfig(AppConfig&&) = delete;
    AppConfig& operator=(AppConfig&&) = delete;

    // Returns the global thread-safe singleton instance
    static AppConfig& getInstance();

    const std::string& getApiKey() const;
    void setApiKey(const std::string& key);
    bool hasApiKey() const;

    const std::string& getModelName() const;
    void setModelName(const std::string& model);
};
