#include "RepositoryAnalyzer.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

RepositoryAnalyzer::RepositoryAnalyzer() {
    // Register supported source file extensions
    supportedExtensions = {
        ".cpp", ".c", ".cc", ".cxx",
        ".h", ".hpp", ".hxx",
        ".java",
        ".py",
        ".js", ".jsx", ".ts", ".tsx"
    };
}

bool RepositoryAnalyzer::isSupportedExtension(const std::string& ext) const {
    std::string lowerExt = ext;
    std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);
    return supportedExtensions.find(lowerExt) != supportedExtensions.end();
}

bool RepositoryAnalyzer::shouldSkipDirectory(const std::string& dirName) const {
    // Ignore version control, build output, cache, package, and hidden directories
    return (dirName == ".git" || dirName == ".github" || dirName == ".vscode" ||
            dirName == ".vs" || dirName == "build" || dirName == "bin" ||
            dirName == "obj" || dirName == "node_modules" || dirName == "__pycache__" ||
            dirName == ".next" || dirName == "dist" || dirName == ".idea");
}

Codebase RepositoryAnalyzer::analyze(const std::string& rootPath) const {
    Codebase codebase;

    fs::path targetDir(rootPath);
    if (!fs::exists(targetDir) || !fs::is_directory(targetDir)) {
        std::cerr << "[RepositoryAnalyzer] Error: Path does not exist or is not a directory: " 
                  << rootPath << std::endl;
        return codebase;
    }

    try {
        for (auto it = fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied);
             it != fs::recursive_directory_iterator(); ++it) {
            if (it->is_directory()) {
                if (shouldSkipDirectory(it->path().filename().string())) {
                    it.disable_recursion_pending();
                    continue;
                }
            } else if (it->is_regular_file()) {
                std::string ext = it->path().extension().string();
                if (isSupportedExtension(ext)) {
                    std::ifstream file(it->path());
                    if (file.is_open()) {
                        std::stringstream buffer;
                        buffer << file.rdbuf();

                        SourceFile sf;
                        std::error_code ec;
                        auto rel = fs::relative(it->path(), targetDir, ec);
                        sf.relativePath = ec ? it->path().string() : rel.string();
                        std::replace(sf.relativePath.begin(), sf.relativePath.end(), '\\', '/');
                        sf.filename = it->path().filename().string();
                        sf.content = buffer.str();

                        codebase.addFile(std::move(sf));
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[RepositoryAnalyzer] Exception while scanning: " << e.what() << std::endl;
    }

    return codebase;
}
