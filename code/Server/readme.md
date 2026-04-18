<div style="background-color: #0d0d0d; color: #fff; padding: 20px; border-radius: 10px;">

# Server Kernel Module

A custom Linux kernel module for high-performance network processing and hardware management.

## System Architecture

The core of the system is built on a highly compact macro-based naming convention and a layered initialization sequence.

### Setup & Macro Layer
The fundamental naming, memory management, and debugging facilities are unified in `.setup`.

<p align="center">
  <img src="setup_diagram.svg" alt="Setup Diagram">
</p>

### Initialization Flow
The system follows a strict layer-by-layer initialization order (and reverse de-initialization) managed in `main.c`.

<p align="center">
  <img src="main_diagram.svg" alt="Main Diagram">
</p>

<p align="center">
  <b>Navigate to Modules:</b><br>
  <a href="Debug/readme.md" style="color: #fff;">Debug</a> | 
  <a href="Common/readme.md" style="color: #fff;">Common</a> | 
  <a href="Hardware/readme.md" style="color: #fff;">Hardware</a> | 
  <a href="Network/readme.md" style="color: #fff;">Network</a> | 
  <a href="OSI/readme.md" style="color: #fff;">OSI</a>
</p>

## Project Structure

- **Debug/**: Custom logging framework.
- **Common/**: Shared resources and utilities.
- **Hardware/**: Low-level drivers for Memory, Network, and Storage.
- **Network/**: Integration with Linux networking stack.
- **OSI/**: Implementation of networking protocols.

</div>
