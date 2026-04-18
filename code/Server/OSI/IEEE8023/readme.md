<div style="background-color: #0d0d0d; color: #fff; padding: 20px; border-radius: 10px;">

# IEEE 802.3 Module

Implementation of the Ethernet framing standard (IEEE 802.3). 

Note: The module is named `IEEE8023` in the code and filesystem to remain compatible with hierarchical macro naming conventions.

## Architecture
<p align="center">
  <img src="core.svg" alt="IEEE 802.3 Diagram">
</p>

## Overview
This module handles Ethernet framing, providing the base layer for all upper-level protocols in the OSI stack. It is responsible for identifying the EtherType and routing the packet to the appropriate protocol handler.

</div>
