cmake_minimum_required(VERSION 3.16)
project(AICodebaseExplainer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Project sources
set(SOURCES
    src/AppConfig.cpp
    src/Conversation.cpp
    src/ExplanationRequestBuilder.cpp
    src/GeminiProvider.cpp
    src/LLMProviderFactory.cpp
    src/RepositoryAnalyzer.cpp
    src/main.cpp
)

add_executable(ai_codebase_explainer ${SOURCES})

if(WIN32)
    target_link_libraries(ai_codebase_explainer PRIVATE winhttp)
else()
    find_package(CURL REQUIRED)
    target_link_libraries(ai_codebase_explainer PRIVATE CURL::libcurl)
endif()
