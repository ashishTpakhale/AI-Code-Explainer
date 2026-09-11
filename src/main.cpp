#include "AppConfig.h"
#include "RepositoryAnalyzer.h"
#include "Conversation.h"
#include "ExplanationRequestBuilder.h"
#include "LLMProviderFactory.h"
#include "TerminalFormatter.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    TerminalFormatter::initTerminal();

    std::cout << "========================================\n";
    std::cout << "      AI CODEBASE EXPLAINER\n";
    std::cout << "   (Creational Patterns Prototype)\n";
    std::cout << "========================================\n\n";

    // Singleton: access global configuration
    AppConfig& config = AppConfig::getInstance();
    std::cout << "[Singleton: AppConfig] Configuration initialized.\n";
    std::cout << "  - Target Model: " << config.getModelName() << "\n";
    if (config.hasApiKey()) {
        std::cout << "  - Gemini API Key: Detected from environment (GEMINI_API_KEY).\n";
    } else {
        std::cout << "  - Gemini API Key: Not detected in environment.\n";
    }
    std::cout << "\n";

    // Repository selection & analysis
    std::string defaultRepo = "./sample-project";
    if (fs::exists("./sample-project")) {
        defaultRepo = "./sample-project";
    } else if (fs::exists("../sample-project")) {
        defaultRepo = "../sample-project";
    }

    std::string repoPath = defaultRepo;
    if (argc > 1) {
        repoPath = argv[1];
    } else {
        std::cout << "Enter repository path (default: " << defaultRepo << "): \n> ";
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) {
            repoPath = input;
        }
    }

    std::cout << "\nAnalyzing repository at: " << repoPath << " ...\n";
    RepositoryAnalyzer analyzer;
    Codebase codebase = analyzer.analyze(repoPath);

    if (codebase.empty()) {
        std::cout << "Warning: No supported source files found in: " << repoPath << "\n";
        std::cout << "Please ensure the path exists and contains .cpp, .h, .py, .java, .js, or .ts files.\n\n";
    } else {
        std::cout << "Found " << codebase.size() << " source file(s):\n";
        for (const auto& filePath : codebase.getFilePaths()) {
            std::cout << "  - " << filePath << "\n";
        }
        std::cout << "Repository analyzed successfully.\n\n";
    }

    // Factory: instantiate provider
    std::string providerType = "gemini";
    if (!config.hasApiKey()) {
        std::cout << "------------------------------------------------------------\n";
        std::cout << "Notice: GEMINI_API_KEY is not set in your environment.\n";
        std::cout << "You can:\n";
        std::cout << "  [1] Paste your Gemini API key now\n";
        std::cout << "  [2] Press Enter to run with offline Mock Provider\n";
        std::cout << "Your choice / API Key: ";
        std::string keyInput;
        std::getline(std::cin, keyInput);

        if (!keyInput.empty() && keyInput != "2") {
            config.setApiKey(keyInput);
            providerType = "gemini";
            std::cout << "[Singleton: AppConfig] Updated API key in global configuration.\n";
        } else {
            providerType = "mock";
        }
        std::cout << "------------------------------------------------------------\n\n";
    }

    std::unique_ptr<LLMProvider> provider = LLMProviderFactory::create(providerType);
    std::cout << "[Factory: LLMProviderFactory] Active Provider: " << provider->getName() << "\n\n";

    Conversation conversation;
    std::string promptLabel = "Ask your question (or type 'exit' to quit):\n> ";

    // Interactive query loop
    while (true) {
        std::cout << promptLabel;
        std::string question;
        std::getline(std::cin, question);

        if (question.empty()) {
            continue;
        }

        std::string lowerInput = question;
        std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

        if (lowerInput == "exit" || lowerInput == "quit" || lowerInput == "q" ||
            lowerInput == "n" || lowerInput == "no") {
            std::cout << "\nThank you for using AI Codebase Explainer. Goodbye!\n";
            break;
        }

        if (lowerInput == "y" || lowerInput == "yes") {
            promptLabel = "What is your question?\n> ";
            continue;
        }

        std::cout << "\nThinking...\n";

        // Builder: assemble request
        ExplanationRequest request = ExplanationRequestBuilder()
                                        .setQuestion(question)
                                        .setCodebase(codebase)
                                        .setConversation(conversation)
                                        .build();

        std::string rawExplanation = provider->generate(request.getPrompt());
        std::string formattedExplanation = TerminalFormatter::format(rawExplanation);

        std::cout << "\n----------------------------------------\n";
        std::cout << "EXPLANATION\n";
        std::cout << "----------------------------------------\n";
        std::cout << formattedExplanation << "\n\n";

        std::cout << "----------------------------------------\n";
        std::cout << "SOURCE EVIDENCE\n";
        std::cout << "----------------------------------------\n";
        if (request.getSourceFilePaths().empty()) {
            std::cout << "No source files were loaded in repository context.\n";
        } else {
            for (const auto& path : request.getSourceFilePaths()) {
                std::cout << "- " << path << "\n";
            }
        }
        std::cout << "----------------------------------------\n\n";

        conversation.addUserMessage(question);
        conversation.addAssistantMessage(rawExplanation);

        promptLabel = "Ask a follow-up question (or type 'exit' to quit):\n> ";
    }

    return 0;
}
