# Server

Low level Linux kernel development written in C.

This project follows its own structure and file layout designed for modular low level systems.


## File Types

This project uses both standard C files and custom project file types.

`.c`  
Implementation files where the logic lives.

`.h`  
Header files containing shared declarations and structures.

`.init`  
Header style files used to declare module initialization and exit functions used by the project structure.

`.setup`  
Project connection files used to link parts of the system together.

`.inc`  
Include style files used as part of the internal project structure.

`Makefile`  
Build configuration used by the build system.

Files without extension  
Some internal project files intentionally have no extension and are part of the project structure.


## Project Idea

The system is designed around small connected modules instead of large source files.  
Each part of the system connects through the project structure and keeps the code organized and easy to extend.


## Philosophy

Simple structure  
Clear module separation  
Low level C  
Performance focused design


## Contributing

If you want to work with this project, follow the existing structure and keep the coding style consistent with the repository.