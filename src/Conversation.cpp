#include "Conversation.h"
#include <sstream>

void Conversation::addUserMessage(const std::string& text) {
    messages.push_back({"User", text});
}

void Conversation::addAssistantMessage(const std::string& text) {
    messages.push_back({"Assistant", text});
}

const std::vector<Message>& Conversation::getMessages() const {
    return messages;
}

std::string Conversation::getFormattedHistory() const {
    if (messages.empty()) {
        return "No prior conversation.";
    }

    std::ostringstream oss;
    for (const auto& msg : messages) {
        oss << msg.role << ": " << msg.content << "\n\n";
    }
    return oss.str();
}

bool Conversation::isEmpty() const {
    return messages.empty();
}

void Conversation::clear() {
    messages.clear();
}
