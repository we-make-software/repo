#pragma once
#include "../../core/.h"
#include "../../overflow/.h"
DefineStructHeadBody(networkDevice){
    u8 version;
    Network*network;
    Overflow overflow;
    StructListNode(node);
};
