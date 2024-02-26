# BufferOverflow C :notebook:

<p align="center">
  <img width="700" height="300" src="./img/1.png">
</p>

## Introduction

In a simple way, I will show how a BufferOverFlow is created in a program written in the C language. For this laboratory, we will use codeblocks to create a simple code in C with a secret function. To view the buffer in memory, we will also use an assembly debugger, called x64dbg.

Softwares;
* [CodeBlocks for C](https://www.codeblocks.org)

* [Debugger x64dbg](https://x64dbg.com)


## Buffer? OverFlow?

The BufferOverflow is a security vulnerability that occurs when a program attempts to store more data in a buffer than it was designed to hold. This could allow an attacker to execute malicious code or corrupt important data on the affected system.

Below we can see an example of how this works. In this case, a variable with a size of 8 bytes is declared, however, there is no limitation on sending data to the program, causing BufferOverFlow when sending 14 bytes.

<p align="center">
  <img width="700" height="200" src="./img/2.png">
</p>

In some cases, we can perform RCE by exploiting a BufferOverFlow, changing and overwriting the next EIP memory address (Let's talk about this)



### Working ....



