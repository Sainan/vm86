# vm86

A wanna-be assembler, disassembler, and virtual machine library for x86_64, specifically on Windows.

## Project Overview

Stuff that you should be fine to use:
- **vanitysec**: A simple CLI tool for renaming sections in the provided Windows executable file (.exe or .dll) — for when `/pdbaltpath` and `/base` are not enough vanity

Stuff that you probably shouldn't use\*:
- **vm86**: The library itself.
- **vm86 example**: An example of vm86 in action.
- **vm86 test**: The "testing suite" for vm86.

\* I made this for fun and learning about x86_64 bytecode and the Windows PE format. That doesn't mean it's entirely useless, but probably not what you want to use for whatever your use-case may be.
