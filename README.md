# Stack Smashing Demonstration

## Overview
This project demonstrates how a **stack buffer overflow** can overwrite the return address and lead to crashes or code redirection when stack protection is disabled.  
It also shows how enabling **stack canaries** in the compiler can detect such corruptions and stop execution before damage occurs.

Two builds are produced:
- **vulnerable** – compiled without stack protection (`-fno-stack-protector` or `/GS-`), allowing overflows to corrupt the stack.
- **protected** – compiled with stack protection (`-fstack-protector-all` or `/GS`), which inserts canaries to detect overflows.

## Build & Run
```bash
# Clone the repo
git clone https://github.com/AniDashyan/stack_smashing
cd stack_smashing

# Build the project
cmake -S . -B build
cmake --build build --target all_demos
````

The `all_demos` target will build both versions and run them sequentially.

## Example Output

```
Running vulnerable version:
Stack smashing demo
Secret function at: 0x7ff7282b1460

Test 1: Safe Input
Buffer address: 0xa8675ffc46
Input size: 5 bytes
Copied to buffer: Hello
Returning from vulnerable_function...
Returns safely

Test 2: Stack Smashing
Attempting stack smashing...
Buffer address: 0xa8675ffc46
Input size: 99 bytes
Copied to buffer: rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
Returning from vulnerable_function...
(Error)
```

*(Actual addresses and messages may vary depending on OS and compiler.)*

## How does it work?

The program contains a vulnerable function:

```cpp
void vulnerable_function(const char* input) {
    char buffer[10];
    strcpy(buffer, input); // No bounds checking!
}
```

In the **vulnerable** build:

* A long input overwrites the return address on the stack.
* This can cause unpredictable behavior, crashes, or — if crafted — redirection to other code.
* Modern systems may still block execution due to DEP or ASLR, but corruption occurs.

In the **protected** build:

* The compiler inserts a **stack canary** (a secret value) before the return address.
* When the function returns, the canary is checked.
* If it has been changed (due to overflow), the program is aborted before returning, preventing execution of corrupted control flow.

The output illustrates:

* **Safe input** works fine.
* **Overflow without canary** leads to crashes/unpredictable behavior.
* **Overflow with canary** is detected and the program is terminated safely.
