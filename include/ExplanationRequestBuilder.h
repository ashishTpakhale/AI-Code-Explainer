#pragma once

#include "ExplanationRequest.h"
#include "Codebase.h"
#include "Conversation.h"
#include <string>
#include <vector>

// Builder class to construct an ExplanationRequest step-by-step.
class ExplanationRequestBuilder {
private:
    std::string question;
    std::string codebaseContext;
    std::vector<std::string> sourceFiles;
    std::string conversationHistory;
    std::string instructions;

public:
    ExplanationRequestBuilder();

    ExplanationRequestBuilder& setQuestion(const std::string& q);
    ExplanationRequestBuilder& setCodebase(const Codebase& cb);
    ExplanationRequestBuilder& setConversation(const Conversation& conv);
    ExplanationRequestBuilder& setInstructions(const std::string& inst);

    ExplanationRequest build() const;
};
