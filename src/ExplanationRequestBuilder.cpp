#include "ExplanationRequestBuilder.h"
#include <sstream>

ExplanationRequestBuilder::ExplanationRequestBuilder() {
    instructions = 
        "You are an AI codebase explanation assistant.\n"
        "Explain the user's question using ONLY the provided repository context.\n"
        "Do not invent classes, functions, relationships or behavior that are not supported by the source code.\n\n"
        "When possible:\n"
        "1. Explain the execution flow.\n"
        "2. Identify relevant classes/functions.\n"
        "3. Explain how components interact.\n"
        "4. Mention the source files supporting the explanation.\n\n"
        "The user may ask follow-up questions.\n"
        "Use the conversation history to understand references such as:\n"
        "\"that class\", \"this function\", \"why does it do that?\", etc.\n\n"
        "FORMATTING INSTRUCTIONS FOR TERMINAL DISPLAY:\n"
        "- Format your response cleanly for a command-line interface.\n"
        "- DO NOT use markdown bold asterisks (avoid **word** or *word*). Instead, use UPPERCASE or clear indentation for emphasis.\n"
        "- Use clean bullet points with '-' or numbered lists.\n"
        "- Avoid cluttering the output with raw markdown syntax.\n";
}

ExplanationRequestBuilder& ExplanationRequestBuilder::setQuestion(const std::string& q) {
    question = q;
    return *this;
}

ExplanationRequestBuilder& ExplanationRequestBuilder::setCodebase(const Codebase& cb) {
    codebaseContext = cb.getFormattedContext();
    sourceFiles = cb.getFilePaths();
    return *this;
}

ExplanationRequestBuilder& ExplanationRequestBuilder::setConversation(const Conversation& conv) {
    conversationHistory = conv.getFormattedHistory();
    return *this;
}

ExplanationRequestBuilder& ExplanationRequestBuilder::setInstructions(const std::string& inst) {
    instructions = inst;
    return *this;
}

ExplanationRequest ExplanationRequestBuilder::build() const {
    std::ostringstream prompt;

    prompt << instructions << "\n"
           << "========================================\n"
           << "CODEBASE CONTEXT:\n"
           << "========================================\n"
           << (codebaseContext.empty() ? "No codebase files provided.\n" : codebaseContext) << "\n"
           << "========================================\n"
           << "CONVERSATION HISTORY:\n"
           << "========================================\n"
           << conversationHistory << "\n"
           << "========================================\n"
           << "CURRENT QUESTION:\n"
           << "========================================\n"
           << question << "\n";

    return ExplanationRequest(question, prompt.str(), sourceFiles);
}
