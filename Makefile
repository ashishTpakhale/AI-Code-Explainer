CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -static -lwinhttp

SRCS = src/AppConfig.cpp \
       src/Conversation.cpp \
       src/ExplanationRequestBuilder.cpp \
       src/GeminiProvider.cpp \
       src/LLMProviderFactory.cpp \
       src/RepositoryAnalyzer.cpp \
       src/main.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = ai_codebase_explainer.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
