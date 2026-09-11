#pragma once

#include <string>
#include <vector>

// Encapsulates the assembled prompt and metadata ready for the LLM provider.
class ExplanationRequest {
private:
    std::string question;
    std::string assembledPrompt;
    std::vector<std::string> sourceFilePaths;

public:
    ExplanationRequest(std::string question, std::string prompt, std::vector<std::string> sourceFiles)
        : question(std::move(question)), assembledPrompt(std::move(prompt)),
          sourceFilePaths(std::move(sourceFiles)) {}

    const std::string& getQuestion() const { return question; }
    const std::string& getPrompt() const { return assembledPrompt; }
    const std::vector<std::string>& getSourceFilePaths() const { return sourceFilePaths; }
};
