#pragma once
#include "../core/.h"
DefineStructHead(Network);
DeclareListHead(Networks);
DeclareLock(NetworkLock);
void network_init(void);
void network_exit(void);
struct packet_type*GetPacketTypeByNetwork(Network*network);
struct list_head*GetNetworkDevicesByNetwork(Network*network);