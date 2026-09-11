#pragma once

#include <string>

// Abstract interface representing an LLM backend service.
class LLMProvider {
public:
    virtual ~LLMProvider() = default;

    // Generates a text response from the underlying language model given a prompt.
    virtual std::string generate(const std::string& prompt) = 0;

    // Returns the provider name for logging and display purposes.
    virtual std::string getName() const = 0;
};
