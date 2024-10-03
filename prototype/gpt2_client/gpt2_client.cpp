#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "wininet.lib")

std::string sendPostRequest(const std::string& serverName, const std::string& endpoint, const std::string& data) {
    HINTERNET hSession = InternetOpen(L"HTTP_Client", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetConnect(hSession, std::wstring(serverName.begin(), serverName.end()).c_str(), 8000, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

    const wchar_t* lplpszAcceptTypes[] = { L"application/json", NULL };
    HINTERNET hRequest = HttpOpenRequest(hConnect, L"POST", std::wstring(endpoint.begin(), endpoint.end()).c_str(), NULL, NULL, lplpszAcceptTypes, 0, 0);

    std::string headers = "Content-Type: application/json\r\n";
    std::string jsonData = "{\"prompt\": \"" + data + "\"}";

    
    BOOL bRequestSent = HttpSendRequest(hRequest, std::wstring(headers.begin(), headers.end()).c_str(), headers.length(), (LPVOID)jsonData.c_str(), jsonData.length());

   
    std::string response;
    if (bRequestSent) {
        char buffer[4096];
        DWORD bytesRead;
        while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead != 0) {
            buffer[bytesRead] = '\0';
            response.append(buffer);
        }
    }

  
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);

    return response;
}

int main() {
    std::string server = "localhost";
    std::string endpoint = "/generate/";

    string prompt = "";
    while (true) {
        cout << "Wpisz zapytanie\n";
        getline(cin, prompt);

        if (prompt == "q") break;

        std::string response = sendPostRequest(server, endpoint, prompt);

        std::cout << response << std::endl;
    }

    return 0;
}
