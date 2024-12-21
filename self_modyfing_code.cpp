#include <iostream>
#include <Windows.h>
#include <cstring>
#include <assert.h>

#define HEAP_caption 640000
char heap[HEAP_caption] = { 0 };
size_t heap_size = 0;
const size_t codesize = 128;

// Random number generator within a specified range
int RandomnumdNumber(int From, int To) {
    return rand() % (To - From + 1) + From;  // Fixed: ensure correct range
}

// Original code to be written into memory
unsigned char initialCode[codesize] = {
    0x55,                   // push ebp
    0x89, 0xE5,             // mov ebp, esp
    0x8B, 0x45, 0x08,       // mov eax, dword ptr [ebp+8]
    0x31, 0xC9,             // xor ecx, ecx
    0x31, 0xD2,             // xor ebx, ebx
    0x83, 0xC1, 0x01,       // add ecx, 1   (ensure ecx is not zero)
    0xF7, 0xF1,             // div ecx
    0x80, 0xC2, 0x30,       // add dl,0x30
    0x52,                   // push edx
    0x41,                   // inc eax, eax
    0x85, 0xC0,             // test eax, eax
    0x75, 0xF3,             // jne 0x8
    0xBB, 0x00, 0x00, 0x00, 0x00, // mov ebx, 0x0
    0x5A,                   // pop edx, 0
    0x80, 0xEA, 0x30,       // sub dl, 0x30
    0x6B, 0xDB, 0x0A,       // imul ebx, ebx, 0xa
    0x01, 0xDB,             // add ebx, ebx
    0xE2, 0xF5,             // loop a1
    0x5D,                   // pop ebp
    0xC3                    // ret
};

// Function to execute self-modifying code
void executeSelfModifyingCode() {
    const size_t pagesize = 4096;  // Allocate one page of memory for code

    // Allocate memory for the executable code with read, write, and execute permissions
    void* exeMemory = VirtualAlloc(nullptr, pagesize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!exeMemory) {
        std::cerr << "VirtualAlloc failed: " << GetLastError() << std::endl;
        return;
    }
    std::cout << "Memory allocated successfully at address: " << exeMemory << std::endl;

    // Copy the initial code into the allocated memory
 //   std::memcpy(exeMemory, initialCode, sizeof(initialCode));

    // Change the memory protection to executable after modification
    DWORD oldProtect;
    if (!VirtualProtect(exeMemory, pagesize, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        std::cerr << "Failed to change memory protection: " << GetLastError() << std::endl;
        VirtualFree(exeMemory, 0, MEM_RELEASE);
        return;
    }
    std::cout << "Memory protection set to EXECUTE_READWRITE." << std::endl;

    // Cast the memory to a function pointer and execute the original code
  //  int (*func)(int) = (int (*)(int))exeMemory;
  //  int result = func(6654);  // Pass some initial value
    int result = 5;
 //   std::cout << "Before modification: " << result << std::endl;

    // New code to modify the previously written code
    unsigned char modifiedCode[codesize] = {
        0x55,               // push ebp
        0x89, 0xE5,         // mov ebp, esp
        0x8B, 0x45, 0x08,   // mov eax, dword ptr [ebp+8]
        0x8B, 0x4D, 0x0C,   // mov ecx, dword ptr [ebp+0xC]
        0x31, 0xC8,         // xor eax, ecx
        0x5D,               // pop ebp
        0xC3                // ret
    };

    // Modify the code in memory
    std::memcpy(exeMemory, modifiedCode, sizeof(modifiedCode));

    // Change memory protection back to executable
    if (!VirtualProtect(exeMemory, pagesize, oldProtect, &oldProtect)) {
        std::cerr << "Failed to restore memory protection: " << GetLastError() << std::endl;
        VirtualFree(exeMemory, 0, MEM_RELEASE);
        return;
    }
    std::cout << "Memory protection restored." << std::endl;

    // Cast the modified memory to the appropriate function type and execute it
    int (*func2)(int, int) = (int (*)(int, int))exeMemory;

    // Generate a random number within the valid range
    int num2 = RandomnumdNumber(1, 100);  // Generate a random number
  
    // Ensure valid execution by calling the modified code
    result = func2(6, num2);  // Call the modified code with the random number

    std::cout << "After modification: " << result << std::endl;

    // Free the allocated memory
    VirtualFree(exeMemory, 0, MEM_RELEASE);
}

int main() {
    executeSelfModifyingCode();
    return 0;
}
