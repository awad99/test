#include <Windows.h>
#include <iostream>
#include <string>

class antiDebug {
private:
    std::string codepass() {
        const char* pass = "87468364";
        const char* pass2 = "not_awadh";
        size_t pass_len = strlen(pass);  // Get the length of the password
        std::string result(pass_len, '\0');  // Allocate space for the result

        // Perform XOR on each character
        for (size_t i = 0; i < pass_len; ++i) {
            result[i] = pass[i] ^ pass2[i];
        }

        return result; // Return the XORed result as a string
    }

    // Check for debugger using TickCount
    BOOL IsDebuggerPresentUsingTickCount() {
        DWORD tickReference = GetTickCount();
        Sleep(1500);
        DWORD elapsedTime = GetTickCount() - tickReference;

        return (elapsedTime > 1600); // Adjusted threshold
    }

    // Check for debugger using LocalTime
    BOOL IsDebuggerPresentUsingLocalTime() {
        SYSTEMTIME sysStart, sysEnd;
        FILETIME fStart, fEnd;
        ULARGE_INTEGER uiStart, uiEnd;

        GetLocalTime(&sysStart);
        Sleep(1500);
        GetLocalTime(&sysEnd);

        if (!SystemTimeToFileTime(&sysStart, &fStart) ||
            !SystemTimeToFileTime(&sysEnd, &fEnd))
            return FALSE;

        uiStart.LowPart = fStart.dwLowDateTime;
        uiStart.HighPart = fStart.dwHighDateTime;
        uiEnd.LowPart = fEnd.dwLowDateTime;
        uiEnd.HighPart = fEnd.dwHighDateTime;

        return ((uiEnd.QuadPart - uiStart.QuadPart) / 10000) > 1600; // Adjusted calculation
    }

    // Read user input
    std::string ReadPassword() {
        std::string password;
        std::cout << "Enter the password: ";
        std::cin >> password;
        return password;
    }

    // Check if the input matches the XOR-ed password
    bool checkpass(const std::string& pass) {
        return pass == codepass();
    }

    // Function to output the XOR password using OutputDebugString
    void DetectDebuggerWithOutputDebugString() {
        // Output the XOR-ed password using OutputDebugString
        std::string xorPassword = codepass();  // Get the XOR password
        std::wstring wstr(xorPassword.begin(), xorPassword.end());  // Convert string to wstring for OutputDebugString
        OutputDebugString(wstr.c_str());  // Output the XORed password to debugger

        // Simulating debugger detection logic (without using SEH)
        Sleep(500);  // Introduce a small delay to simulate detection
        OutputDebugString(L"Debugger found or delayed execution detected.\n");

        // Here we can put any logic that can detect debugging behavior without causing stack unwinding issues
    }

public:
    antiDebug() {
        std::string pass = codepass();
        while (true) {

            BOOL bDebuggerPresent = FALSE;
            if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &bDebuggerPresent) && bDebuggerPresent) {
                ExitProcess(-1);  // Exit if debugger is present
            }

            // Check for debugger using the built-in IsDebuggerPresent
            if (IsDebuggerPresent() || IsDebuggerPresentUsingTickCount()) {
                MessageBox(0, L"Debugger detected!", L"Anti-Debugger", MB_ICONERROR);
                break;
            }

            // Try to detect debugger using OutputDebugString
            DetectDebuggerWithOutputDebugString();

            system("pause");

            // Read the password from the user
            std::string password = ReadPassword();

            // Perform additional debugger check before verifying the password
            if (IsDebuggerPresentUsingLocalTime()) {
                MessageBox(0, L"Debugger detected!", L"Anti-Debugger", MB_ICONERROR);
                break;
            }

            // Verify the password
            if (checkpass(password)) {
                MessageBox(0, L"You win!", L"Password Check", MB_OK);
                break;
            }
            else {
                MessageBox(0, L"Try again", L"Password Failed", MB_ICONERROR);
            }
        }
    }
};
