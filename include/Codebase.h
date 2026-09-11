#pragma once

#include <string>
#include <vector>
#include <sstream>

// Represents a single source code file discovered in the target repository
struct SourceFile {
    std::string relativePath;
    std::string filename;
    std::string content;
};

// Represents the analyzed codebase containing all loaded source files
class Codebase {
private:
    std::vector<SourceFile> files;

public:
    Codebase() = default;

    void addFile(SourceFile file) {
        files.push_back(std::move(file));
    }

    const std::vector<SourceFile>& getFiles() const {
        return files;
    }

    size_t size() const {
        return files.size();
    }

    bool empty() const {
        return files.empty();
    }

    // Formats all source files into a structured markdown/text block for LLM prompt context
    std::string getFormattedContext() const {
        std::ostringstream oss;
        for (const auto& file : files) {
            oss << "==================================================\n";
            oss << "FILE: " << file.relativePath << "\n";
            oss << "==================================================\n";
            oss << file.content << "\n\n";
        }
        return oss.str();
    }

    std::vector<std::string> getFilePaths() const {
        std::vector<std::string> paths;
        paths.reserve(files.size());
        for (const auto& file : files) {
            paths.push_back(file.relativePath);
        }
        return paths;
    }
};
