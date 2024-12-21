//#include <windows.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include "my_obfusction.h" // Ensure this file has include guards

// Macro Definitions for Obfuscation
#define FUNCTION_START() 
#define FUNCTION_END() 
#define BLOCK(...) { __VA_ARGS__ }
#define IF(expr, ...) if (expr) { __VA_ARGS__; }
#define RETURN(...) return (__VA_ARGS__)
#define ELSE(...) else { __VA_ARGS__; }
// Global variable for secret message
std::string secret_message = "Hello, World!";

// Function to read the flag input from the user
static std::string ReadFlag() {
    FUNCTION_START();
    std::string flag;
    std::cout << "Enter the flag: \n";
    std::cin >> flag;
    FUNCTION_END();
    RETURN(flag);
}

// Function to check if the flag is correct
static bool checkFlag(std::string flag, std::string realflag) {
    FUNCTION_START();
    IF(flag == realflag) {
        RETURN(true);  // Ensure this line ends with a semicolon
    }
    RETURN(false);  // Ensure this line ends with a semicolon
    FUNCTION_END();
}

// Entry point for the application
int main() {
    // Decode the secret message (obfuscation example)
    std::string decoded_message = decode_string(encode_string(secret_message));

    // Read the flag from the user
    std::string flag = ReadFlag();

    // Define the messages for correct and incorrect flag
    std::string messageWin = "That is the correct flag : -)";
    std::string messageFalse = "That is the incorrect flag, try again :-(";

    // Check if the entered flag matches the secret flag
    IF(checkFlag(flag, secret_message)) {
        decoded_message = decode_string(encode_string(messageWin));
        std::cout << messageWin << std::endl;
    }

   
        ELSE (
        decoded_message = decode_string(encode_string(messageFalse));
    std::cout << messageFalse << std::endl;
        );

    std::cin.get();
    return 0;
}
