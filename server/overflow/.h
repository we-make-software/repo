#pragma once
#include "../core/.h"
DefineStructHeadBody(OverflowSetting){
    u128 ref;
    u8 index;
};
DefineStructHeadBody(Overflow){
    OverflowSetting moment,block;
};
void OverFlowInit(Overflow*overflow);
bool OverFlowIncrement(Overflow*overflow);
bool OverFlowDecrement(Overflow*overflow);
void OverFlowBlock(Overflow*overflow);
void OverFlowSetBlock(Overflow*overflow,u8 index,u128 ref);