<div style="background-color: #0d0d0d; color: #fff; padding: 20px; border-radius: 10px;">

<p align="center">
  <a href="../readme.md" style="color: #fff;">Back to Common</a>
</p>

# Sync Module

A reference-counted synchronization framework built on Linux kernel mutexes.

## Architecture
<p align="center">
  <img src="core.svg" alt="Sync Diagram">
</p>

<p align="center">
  <b>Navigate to Submodules:</b><br>
  <a href="Protection/readme.md" style="color: #fff;">Protection</a>
</p>
## Overview
The Sync module handles object lifecycles through reference counting. It prevents use-after-free errors and ensures safe object destruction by allowing hooks (`before` and `after`) to be called when the reference count reaches zero.

## Usage
The `SyncLock` macro includes an `if` statement for compact error handling (e.g., `SyncLock(scs)return;`).

</div>