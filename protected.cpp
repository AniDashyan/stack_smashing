#include <iostream>
#include <cstring>

void vulnerable_function(const char* input) {
    char buffer[10]; 
    
    std::cout << "Buffer address: " << (void*)buffer << "\n";
    std::cout << "Input size: " << strlen(input) << " bytes\n";
    std::cout << "Buffer size: 10 bytes\n";
    
    // Same vulnerable code but compiler will add stack protection
    strcpy(buffer, input);
    
    std::cout << "Copied to buffer: " << buffer << "\n";
    std::cout << "Returning from vulnerable_function...\n";
}

int main() {
    std::cout << "=== PROTECTED VERSION (With Stack Canaries) ===\n\n";
    
    std::cout << "Stack Smashing Safe Test Protected Version\n";
    vulnerable_function("Hello");
    std::cout << "-> Normal input completed successfully\n\n";

    std::cout << "Stack Smashing Overflow Test Protected Version\n";
    std::cout << "Creating large input to test stack protection...\n";
    
    // Same large input that caused crash in vulnerable version
    char large_input[100];
    memset(large_input, 'X', 99);
    large_input[99] = '\0';
    
    std::cout << "Attempting to copy " << strlen(large_input) 
              << " bytes into 10-byte buffer...\n";
    std::cout << "Stack canary should detect the overflow and terminate safely...\n";
    
    vulnerable_function(large_input);
    
    // This line should not execute - stack protector should terminate first
    std::cout << "-> If you see this, stack protection may not be working\n";
    
    return 0;
}