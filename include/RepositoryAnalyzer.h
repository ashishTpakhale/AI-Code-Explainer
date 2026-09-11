#pragma once

#include "Codebase.h"
#include <string>
#include <unordered_set>

// RepositoryAnalyzer performs recursive file scanning on a target directory,
// filtering for supported source code extensions and loading their contents.
class RepositoryAnalyzer {
private:
    std::unordered_set<std::string> supportedExtensions;

    bool isSupportedExtension(const std::string& ext) const;
    bool shouldSkipDirectory(const std::string& dirName) const;

public:
    RepositoryAnalyzer();

    // Analyzes the repository at rootPath and returns a populated Codebase
    Codebase analyze(const std::string& rootPath) const;
};
