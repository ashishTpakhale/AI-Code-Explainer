# AI-Assisted Codebase Explainer

> A command-line assistant that helps developers understand unfamiliar repositories through natural-language questions, grounded in the source code they provide.

AI-Assisted Codebase Explainer scans a local repository, builds a context-aware request from its source files and your conversation, and uses Google Gemini to explain architecture, behavior, dependencies, and implementation details. Each answer includes the files used as supporting evidence.

---

## What it does

- Scans supported source files in a local repository.
- Answers questions such as "How does authentication work?" or "Where is the database connection created?"
- Keeps conversation context for natural follow-up questions.
- Grounds responses in the scanned codebase instead of generic assumptions.
- Lists the source files that informed every explanation.
- Provides an offline mock mode when a Gemini API key is unavailable.

## How it works

```text
Local repository
      |
      v
Repository analyzer  --->  Codebase context
                                  |
User question + conversation -----+
                                  v
                     Request builder
                                  |
                                  v
                    LLM provider (Gemini or mock)
                                  |
                                  v
          Explanation with source-file evidence
```

The application separates repository analysis, conversation management, request construction, configuration, and LLM integration. The provider interface makes it possible to add other model backends without changing the core CLI workflow.

## Example session

```text
Enter repository path (default: ./sample-project):
> ./my-project

Analyzing repository at: ./my-project ...
Found 7 source file(s).
Repository analyzed successfully.

Ask your question (or type 'exit' to quit):
> How does the auction and bidding system work?

EXPLANATION
The auction system uses a WebSocket-based real-time architecture...

SOURCE EVIDENCE
- auction-backend/main.py
- auction-backend/database.py
- auction-frontend/app/auctioneer/page.tsx

Ask a follow-up question (or type 'exit' to quit):
> Which file connects to Redis?
```

## Prerequisites

- A C++17-compatible compiler (GCC 10+ recommended).
- MSYS2 with GNU Make for the Make-based Windows workflow.
- A Google Gemini API key for live AI-powered explanations. The application can also run in offline mock mode.

### Install build dependencies (MSYS2 UCRT64)

```bash
pacman -S --needed \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-nlohmann-json
```

On Windows, networking uses the native WinHTTP API; no `libcurl` installation is required.

## Build

### Make

```powershell
cd AI-Codebase-Explainer
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\usr\bin;" + $env:PATH

make
```

To remove build artifacts:

```powershell
make clean
```

### CMake

```powershell
cmake -B build -G "Ninja" -DCMAKE_CXX_COMPILER="g++.exe"
cmake --build build
```

The Windows executable links against system libraries including WinHTTP and Kernel32.

## Configure Gemini

Get an API key from [Google AI Studio](https://aistudio.google.com/), then set `GEMINI_API_KEY` before launching the application.

```powershell
# PowerShell
$env:GEMINI_API_KEY="your_api_key_here"

# Windows Command Prompt
set GEMINI_API_KEY=your_api_key_here

# Linux / macOS
export GEMINI_API_KEY="your_api_key_here"
```

The key is read from the environment at runtime and is not written to project files.

## Run

```powershell
# From the project root
.\ai_codebase_explainer.exe

# Or specify a repository directly
.\ai_codebase_explainer.exe path\to\your\project
```

If `GEMINI_API_KEY` is not set, the CLI offers an offline mock provider. This is useful for exploring the interface and validating the local workflow without external API access.

## Supported files

The repository scanner discovers files with these extensions:

`.cpp` `.c` `.cc` `.h` `.hpp` `.java` `.py` `.js` `.jsx` `.ts` `.tsx`

It skips common generated and dependency directories such as `node_modules`, `.git`, `build`, `.next`, and `__pycache__`.

## Project structure

```text
AI-Codebase-Explainer/
|-- CMakeLists.txt
|-- Makefile
|-- include/
|   |-- AppConfig.h
|   |-- Codebase.h
|   |-- Conversation.h
|   |-- ExplanationRequestBuilder.h
|   |-- GeminiProvider.h
|   |-- LLMProvider.h
|   |-- LLMProviderFactory.h
|   `-- RepositoryAnalyzer.h
|-- src/
|   |-- main.cpp
|   |-- AppConfig.cpp
|   |-- Conversation.cpp
|   |-- ExplanationRequestBuilder.cpp
|   |-- GeminiProvider.cpp
|   |-- LLMProviderFactory.cpp
|   `-- RepositoryAnalyzer.cpp
`-- sample-project/
```

## Technology

| Area | Technology |
|---|---|
| Language | C++17 |
| Build | GNU Make or CMake with Ninja |
| Model integration | Google Gemini REST API |
| Networking | Windows WinHTTP API |
| JSON | nlohmann/json |
| Repository scanning | `std::filesystem` |

## License

Free to use and extend.
