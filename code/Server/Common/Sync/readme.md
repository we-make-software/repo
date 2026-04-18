<div style="background-color: #0d0d0d; color: #fff; padding: 20px; border-radius: 10px;">

# Sync Module

A reference-counted synchronization framework built on Linux kernel mutexes.

## Architecture
<p align="center">
  <img src="sync_diagram.svg" alt="Sync Diagram">
</p>

## Overview
The Sync module handles object lifecycles through reference counting. It prevents use-after-free errors and ensures safe object destruction by allowing hooks (`before` and `after`) to be called when the reference count reaches zero.

</div>
