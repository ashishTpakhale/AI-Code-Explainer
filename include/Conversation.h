#pragma once

#include <string>
#include <vector>

struct Message {
    std::string role; // "User" or "Assistant"
    std::string content;
};

// Maintains in-memory dialogue history across multiple turns.
class Conversation {
private:
    std::vector<Message> messages;

public:
    Conversation() = default;

    void addUserMessage(const std::string& text);
    void addAssistantMessage(const std::string& text);

    const std::vector<Message>& getMessages() const;
    std::string getFormattedHistory() const;
    bool isEmpty() const;
    void clear();
};
