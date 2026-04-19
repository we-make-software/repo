<div style="background-color: #0d0d0d; color: #fff; padding: 20px; border-radius: 10px;">

<p align="center">
  <a href="../readme.md" style="color: #fff;">Back to Sync</a>
</p>

# Sync Protection Module

Extends the Sync framework with time-based and count-based access control.

## Architecture
<p align="center">
  <img src="core.svg" alt="Sync Protection Diagram">
</p>

## Overview
The Sync Protection module provides lifecycle management with expiration (UTC) and usage limits (limitation). Delayed work handles expiration. Limits can be dynamically increased or decreased.

## Usage
Helper functions (Increase, Decrease, Event, Immediate, GetLimition) do not manage internal locks. Callers must wrap these calls within a `SyncProtectionLock` and `SyncProtectionUnlock` block. The `SyncProtectionLock` macro includes an `if` statement for compact error handling (e.g., `SyncProtectionLock(scsp)return;`).

</div>