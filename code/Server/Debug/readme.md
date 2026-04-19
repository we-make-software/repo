<div style="background-color: #0d0d0d; color: #fff; padding: 20px; border-radius: 10px;">

<p align="center">
  <a href="../readme.md" style="color: #fff;">Back to Server</a>
</p>

# Debug Module

A custom real-time logging framework that writes directly to the kernel filesystem.

## Debug Architecture
<p align="center">
  <img src="core.svg" alt="Debug Diagram">
</p>

## Overview
The module provides a layered approach to logging, allowing for organized real-time tracing of kernel events.

- **Output**: Represents a log file on disk.
- **Message**: A unique category within a log file.
- **Value**: Data logged under a message category.

</div>