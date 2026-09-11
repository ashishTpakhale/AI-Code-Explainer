#include "GeminiProvider.h"
#include "AppConfig.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <winhttp.h>
#else
#include <curl/curl.h>
#endif

using json = nlohmann::json;

GeminiProvider::GeminiProvider() {
    apiKey = AppConfig::getInstance().getApiKey();
    modelName = AppConfig::getInstance().getModelName();
}

GeminiProvider::GeminiProvider(std::string apiKey, std::string modelName)
    : apiKey(std::move(apiKey)), modelName(std::move(modelName)) {}

std::string GeminiProvider::getName() const {
    return "Google Gemini (" + modelName + ")";
}

#ifdef _WIN32
std::string GeminiProvider::sendHttpRequest(const std::string& host, const std::string& path, const std::string& jsonBody) {
    std::string response;

    std::wstring wHost(host.begin(), host.end());
    std::wstring wPath(path.begin(), path.end());

    HINTERNET hSession = WinHttpOpen(L"AICodebaseExplainer/1.0",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        return "[GeminiProvider Error] WinHttpOpen failed: " + std::to_string(GetLastError());
    }

    HINTERNET hConnect = WinHttpConnect(hSession, wHost.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(hSession);
        return "[GeminiProvider Error] WinHttpConnect failed: " + std::to_string(err);
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", wPath.c_str(),
                                           NULL, WINHTTP_NO_REFERER,
                                           WINHTTP_DEFAULT_ACCEPT_TYPES,
                                           WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "[GeminiProvider Error] WinHttpOpenRequest failed: " + std::to_string(err);
    }

    std::wstring headers = L"Content-Type: application/json\r\n";
    BOOL bResults = WinHttpSendRequest(hRequest,
                                       headers.c_str(), headers.length(),
                                       (LPVOID)jsonBody.c_str(), jsonBody.length(),
                                       jsonBody.length(), 0);

    if (bResults) {
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    }

    if (bResults) {
        DWORD dwSize = 0;
        do {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
            if (dwSize == 0) break;

            std::vector<char> buffer(dwSize + 1, 0);
            DWORD dwDownloaded = 0;
            if (WinHttpReadData(hRequest, buffer.data(), dwSize, &dwDownloaded)) {
                response.append(buffer.data(), dwDownloaded);
            }
        } while (dwSize > 0);
    } else {
        response = "[GeminiProvider Network Error] WinHttp request failed: " + std::to_string(GetLastError());
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return response;
}
#else
static size_t curlWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string GeminiProvider::sendHttpRequest(const std::string& host, const std::string& path, const std::string& jsonBody) {
    CURL* curl = curl_easy_init();
    if (!curl) return "[GeminiProvider Error] Failed to initialize libcurl.";

    std::string url = "https://" + host + path;
    std::string responseBuffer;

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 45L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return std::string("[GeminiProvider Network Error] libcurl error: ") + curl_easy_strerror(res);
    }
    return responseBuffer;
}
#endif

std::string GeminiProvider::generate(const std::string& prompt) {
    if (apiKey.empty()) {
        return "[GeminiProvider Error] GEMINI_API_KEY environment variable is not set.\n"
               "Please set your Gemini API key in your terminal:\n"
               "  PowerShell: $env:GEMINI_API_KEY=\"your_api_key_here\"\n"
               "  Command Prompt: set GEMINI_API_KEY=your_api_key_here\n";
    }

    std::string host = "generativelanguage.googleapis.com";
    std::string path = "/v1beta/models/" + modelName + ":generateContent?key=" + apiKey;

    json payload;
    payload["contents"] = json::array({
        {
            {"parts", json::array({
                {{"text", prompt}}
            })}
        }
    });

    std::string responseBuffer = sendHttpRequest(host, path, payload.dump());

    if (responseBuffer.rfind("[GeminiProvider", 0) == 0) {
        return responseBuffer;
    }

    try {
        json responseJson = json::parse(responseBuffer);

        if (responseJson.contains("error")) {
            std::string errMsg = responseJson["error"].value("message", "Unknown API error");
            return "[Gemini API Error]: " + errMsg;
        }

        if (responseJson.contains("candidates") && !responseJson["candidates"].empty()) {
            auto& candidate = responseJson["candidates"][0];
            if (candidate.contains("content") && candidate["content"].contains("parts")) {
                auto& parts = candidate["content"]["parts"];
                if (!parts.empty() && parts[0].contains("text")) {
                    return parts[0]["text"].get<std::string>();
                }
            }
        }

        return "[GeminiProvider Error] Unexpected response format received from Gemini:\n" + responseBuffer;

    } catch (const std::exception& e) {
        return std::string("[GeminiProvider JSON Parsing Error]: ") + e.what() + 
               "\nRaw Response: " + responseBuffer;
    }
}
