<div style="background-color: #0d0d0d; color: #fff; padding: 20px; border-radius: 10px;">

# Common Module

Shared resources and utilities used across the Server project.

## Architecture
<p align="center">
  <img src="common_diagram.svg" alt="Common Diagram">
</p>

## Submodules

### Sync
A reference-counted synchronization framework using mutexes. It ensures safe object deletion by tracking active references and providing initialization/cleanup hooks.

- **SyncInit**: Initialize synchronization object.
- **SyncGet**: Increment reference count.
- **SyncLock/Unlock**: Safe mutex operations.
- **SyncDelete**: Decrement count and trigger cleanup if zero.

</div>
