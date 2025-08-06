#include <iostream>
#include <cstring>

void vulnerable_function(const char* input) {
    char buffer[10];
    
    std::cout << "Buffer address: " << (void*)buffer << "\n";
    std::cout << "Input size: " << strlen(input) << " bytes\n";
    
    strcpy(buffer, input);
    
    std::cout << "Copied to buffer: " << buffer << "\n";
    std::cout << "Returning from vulnerable_function...\n";
}

int main() {
    std::cout << "Stack Smashing Safe Test\n";
    vulnerable_function("Hello");
    std::cout << "Returns safely\n";

    std::cout << "Stack Smashing Overflow Test\n";
    char large_input[100];
    memset(large_input, 'r', 99);
    large_input[99] = '\0';

    std::cout << "Attempting large overflow...\n";
    vulnerable_function(large_input);
    std::cout << "Finished large overflow test\n";

    return 0;
}