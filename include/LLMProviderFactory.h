#pragma once

#include "LLMProvider.h"
#include <memory>
#include <string>

// Factory class that encapsulates instantiation of LLMProvider implementations.
class LLMProviderFactory {
public:
    // Instantiates and returns the appropriate LLMProvider based on providerType
    static std::unique_ptr<LLMProvider> create(const std::string& providerType = "gemini");
};
