#include <iostream>
#include <cstring>

void secret_function() {
    std::cout << "secret_function() executed!\n";
    std::cout << "Stack smashing attack succeeded!\n";
}

void vulnerable_function(const char* input) {
    char buffer[10];
    
    std::cout << "Buffer address: " << (void*)buffer << "\n";
    std::cout << "Input size: " << strlen(input) << " bytes\n";
    
    // No bounds checking!
    strcpy(buffer, input);
    
    std::cout << "Copied to buffer: " << buffer << "\n";
    std::cout << "Returning from vulnerable_function...\n";
}

int main() {
    std::cout << "Stack smashing demo\n";
    std::cout << "Secret function at: " << (void*)secret_function << "\n\n";
    
    // Test 1: Safe input
    std::cout << "Test 1: Safe Input\n";
    vulnerable_function("Hello");
    std::cout << "Returns safely\n\n";
    
    // Test 2: Large overflow (may crash or trigger protection)
    std::cout << "Test 2: Stack Smashing\n";
    char large_input[100];
    memset(large_input, 'r', 99);
    large_input[99] = '\0';
    
    std::cout << "Attempting stack smashing...\n";
    vulnerable_function(large_input);
    
    std::cout << "Program finished normally.\n";
    return 0;
}