#pragma once
#include "../core/.h"
#include "../EUI48/.h"
DefineStructHead(Network);
DeclareListHead(Networks);
DeclareLock(NetworkLock);
void network_init(void);
void network_exit(void);
struct packet_type*GetPacketTypeByNetwork(Network*network);
struct list_head*GetNetworkDevicesByNetwork(Network*network);
void IEEE802_3_RX(struct sk_buff*skb,Network*network,u8*buff);
struct sk_buff*network_PX(Network*network);
void network_CX(Network*network,struct sk_buff*buff);
void network_TX(Network*network,struct sk_buff*buff,IEEE802_3*ieee802_3);
EUI48*network_Get(Network*network);
#include "device/.h"

