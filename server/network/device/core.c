#include ".h"
#include "../.h"
#include "../../EUI48/.h"
#include "../../overflow/.h"
DefineStructBody(networkDevice){
    EUI48 eui48;
    Network*network;
    Overflow overflow;
    StructListNode(node);
};
