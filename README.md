# BufferOverflow :notebook:

<p align="center">
  <img width="700" height="300" src="./img/1.png">
</p>

## Introduction

In a simple way, I will show how a BufferOverFlow is created in a program written in the C language. For this laboratory, we will use codeblocks to create a simple code in C with a secret function. To view the buffer in memory, we will also use an assembly debugger, called x64dbg.

Softwares;
* [CodeBlocks for C](https://www.codeblocks.org)

* [Debugger x64dbg](https://x64dbg.com)

## Basics about x64dbg

In this step, let's quickly talk about windows and some information about x64dbg.

Explaining each window.

1 - The memory address is given in hexadecimal.<br>
2 - The value that is in the memory address of window 1 is informed.<br>
3 - In the form of mnemonics, instructions for each address are provided.
<p align="center">
  <img width="900" height="300" src="./img/9.png">
</p>
4 - These are the registers (32bit). Registers are used to store temporary data and memory addresses during program execution.<br>
5 - This is the Flags field. These are special bits in a register that represent the result of an arithmetic or logical operation. They are used to indicate conditions such as equality, greater than, less than, among others, and are often used in conditional branch instructions to control program flow.<br><br>

<p align="center">
  <img width="500" height="300" src="./img/10.png">
</p>

6 - The name of this window is **Stack**. A special memory region used to store temporary data, return addresses, and function call contexts. The **Stack** is a LIFO (Last In, First Out) data structure.
<p align="center">
  <img width="700" height="250" src="./img/11.png">
</p>

These are the 6 basic windows that we will use to visualize the program's BufferOverFlow process.

I think we're ready to go! :)

## Buffer? OverFlow?

The BufferOverflow is a security vulnerability that occurs when a program attempts to store more data in a buffer than it was designed to hold. This could allow an attacker to execute malicious code or corrupt important data on the affected system.

Below we can see an example of how this works. In this case, a variable with a size of 8 bytes is declared, however, there is no limitation on sending data to the program, causing BufferOverFlow when sending 14 bytes.

<p align="center">
  <img width="700" height="200" src="./img/2.png">
</p>

In some cases, we can perform RCE by exploiting a BufferOverFlow, changing and overwriting the next EIP memory address (Let's talk about this)

## How are we going to do it?

First, let's create a basic code in C in codeblocks as shown in the example below.

```
#include <stdio.h>
#include <string.h>

void secret() {
    printf("Secret Function\n");
}

int main(int argc, char **argv){
    char test[12];
    gets(test);
    printf("You send: %s\n", test);
    return(0);
}
```
We are creating 2 functions, the main one which only shows the text we type. And we have another function called secret. As we can see, the main function does not call the secret function, so let's try to use the BufferOverFlow vulnerability to try to call the secret function by overwriting memory addresses.

<p align="center">
  <img width="600" height="100" src="./img/3.png">
</p>

To make this laboratory simpler, we are using the GCC compiler and 32-bit Codeblocks, however, the exploration logic for 64-bits is the same.

## Let's explore!

Assembly language is complex and has a lot of theory to be explained, so I'm going directly to the exploration points explaining it in a basic way.

1 - Let's jump directly to the function of our program, clicking on the RUN button (Arrow >) until we find the **EntryPoint** of Buffer.exe (Name of my C). 

Right after the **EntryPoint**, we have some basic executions of the program, it is not our main function yet.

<p align="center">
  <img width="900" height="300" src="./img/4.png">
</p>

2 - Let's go to our MAIN function. Using the **F8** button, we go to the function line **[buffer.401000]** (This name may change to different compilers). We enter this function with **F7**.

<p align="center">
  <img width="900" height="80" src="./img/5.png">
</p>

3 - We will be sent to start a new role, however, it is not yet the main one.

<p align="center">
  <img width="900" height="150" src="./img/6.png">
</p>

Navigating through the function using **F8**, we find a new function **[buffer.401348]**. This is our main function of the C program. We enter it **(F7)**.
<p align="center">
  <img width="900" height="150" src="./img/7.png">
</p>

This is the beginning of our MAIN function.
<p align="center">
  <img width="900" height="190" src="./img/8.png">
</p>

4 - We have arrived at our function! But what does this part do?

<p align="center">
  <img width="600" height="300" src="./img/12.png">
</p>

- Quick Explanation<br>

```
push ebp           # Responsible for the base of the stack. Sends the EBP value to stack.
mov ebp,esp        # Moves the value of the ESP register to EBP.
and esp,FFFFFFF0   # Sets the 4 least significant bits to 0.
sub esp,20         # Subtract 20 from ESP. At this point, space is created in the stack
call buffer.401930 # Function call
lea eax,dword ptr ss:[esp+14]   # Retrieves the stack address pointed to by ESP with +14 offset and sends it to EAX.
mov dword ptr ss:[esp],eax      # Sends the value of EAX to the value of the STACK address pointed to by ESP
call <JMP.&gets>   # Function that waits for input at the prompt.
...
After that, it retrieves the value to be shown in the prompt again.
```

5 - Let's send some Data and observe the Stack (explanation above)

* This is the stack before the GETS function. There is an address [0060FF0C] responsible for returning this function.

<p align="center">
  <img width="900" height="250" src="./img/13.png">
</p>

* Let's send 10 'A' (``AAAAAAAAAA``) in prompt. 

Cool, we observe the filling of the field reserved to receive the entered string.

<p align="center">
  <img width="900" height="250" src="./img/14.png">
</p>

6 - Sending more Data

* This function has a return address, called at the end of our function to return to executing our program (outside the main function). As we can see above, the return address is below the field reserved for data. Can we send more data and fill in the return value for this function?

  Counting the bytes of 41 (A) we will arrive at the return address sending 28 A's. We will try?
  ``AAAAAAAAAAAAAAAAAAAAAAAAAAAA`` (28 A)

<p align="center">
  <img width="900" height="250" src="./img/15.png">
</p>

<p align="center">
Oops, it looks like we wrote the return address with A. Let's see the return.
</p>
  

* At the end of the function, RET is called, which uses the ESP address where we overwrite it with A. When trying to look for the memory value **41414141**, it does not exist and causes **ACCESS_VIOLATION**.

  <p align="center">
  <img width="900" height="300" src="./img/16.png">
</p>

<p align="center">
Voilà! we have a BufferOverFlow overwriting the return address of our function.
</p>


<p align="center">
  <img width="400" height="260" src="https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExdGlkOHdqYWQwMndwM2hudmRyODZ2aGdwMTVxd2JoZjdqcXMzbmJ3ZiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/nbvFVPiEiJH6JOGIok/giphy.gif">
</p>


7 - Where is our secret function?