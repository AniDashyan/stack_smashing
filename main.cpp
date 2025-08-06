#include <iostream>
#include <cstring>
#include <csetjmp>

#ifdef _WIN32
    #include <windows.h>
    static jmp_buf jump_buffer;
    LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS* ExceptionInfo) {
        std::cout << "\n[!] Caught access violation (Windows). Continuing execution...\n";
        longjmp(jump_buffer, 1);
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    void setup_crash_handler() {
        SetUnhandledExceptionFilter(windows_exception_handler);
    }
#else
    #include <csignal>
    static sigjmp_buf jump_buffer;
    void sigsegv_handler(int signo) {
        std::cout << "\n[!] Caught segmentation fault (signal " << signo << "). Continuing execution...\n";
        siglongjmp(jump_buffer, 1);
    }
    void setup_crash_handler() {
        struct sigaction sa{};
        sa.sa_handler = sigsegv_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGSEGV, &sa, nullptr);
    }
#endif

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
    setup_crash_handler();

    std::cout << "Stack Smashing\n";
    std::cout << "Secret function at: " << (void*)secret_function << "\n\n";
    
    // Test 1: Safe input
#ifdef _WIN32
    if (setjmp(jump_buffer) == 0)
#else
    if (sigsetjmp(jump_buffer, 1) == 0)
#endif
    {
        std::cout << "Test 1: Safe Input\n";
        vulnerable_function("Hello");
        std::cout << "Returns safely\n\n";
    }

    // Test 2: Stack Smashing
#ifdef _WIN32
    if (setjmp(jump_buffer) == 0)
#else
    if (sigsetjmp(jump_buffer, 1) == 0)
#endif
    {
        std::cout << "Test 2: Stack Smashing\n";
        char large_input[100];
        memset(large_input, 'r', 99);
        large_input[99] = '\0';
        
        std::cout << "Attempting large overflow...\n";
        vulnerable_function(large_input);
        std::cout << "Finished large overflow test\n";
    }

    std::cout << "\nProgram finished normally.\n";
    return 0;
}