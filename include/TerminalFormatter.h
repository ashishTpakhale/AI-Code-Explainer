#pragma once

#include <string>
#include <regex>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

// Utility to render markdown elements as clean ANSI text in terminal output.
class TerminalFormatter {
public:
    static void initTerminal() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }

    static std::string format(const std::string& input) {
        std::string text = input;

        // Convert **bold** to ANSI Bold
        std::regex boldRegex(R"(\*\*(.+?)\*\*)");
        text = std::regex_replace(text, boldRegex, "\033[1m$1\033[0m");

        // Strip lone markdown asterisks
        std::regex italicRegex(R"((^|[^\*])\*([^\*\s][^\*]*?)\*([^\*]|$))");
        text = std::regex_replace(text, italicRegex, "$1$2$3");

        // Convert `code` to ANSI Cyan
        std::regex codeRegex(R"(`([^`]+?)`)");
        text = std::regex_replace(text, codeRegex, "\033[36m$1\033[0m");

        // Format markdown headers
        std::regex h3Regex(R"(###\s+([^\r\n]+))");
        text = std::regex_replace(text, h3Regex, "\033[1;33m[>] $1\033[0m");

        std::regex h2Regex(R"(##\s+([^\r\n]+))");
        text = std::regex_replace(text, h2Regex, "\033[1;35m[#] $1\033[0m");

        // Convert horizontal rules
        std::regex hrRegex(R"(\r?\n---\r?\n)");
        text = std::regex_replace(text, hrRegex, "\n----------------------------------------\n");

        return text;
    }
};
