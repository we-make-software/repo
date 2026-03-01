#include ".h"
EUI48_IG EUI48_GetIG(EUI48*eui48){
    return eui48->path.infomation.ig?EUI48_IG_MULTICAST:EUI48_IG_UNICAST;
}

EUI48_UL EUI48_GetUL(EUI48*eui48){
    return eui48->path.infomation.ul?EUI48_UL_LOCAL:EUI48_UL_GLOBAL;
}

void EUI48_Clone(EUI48*from,EUI48*to){
    to->path.normal.h=from->path.normal.h;
    to->path.normal.l=from->path.normal.l;
};