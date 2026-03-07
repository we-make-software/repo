#pragma once
#include ".setup"
void overflowInit(overflow*overflow);
bool overflowIncrement(overflow*overflow);
bool overflowDecrement(overflow*overflow);
void overflowBlock(overflow*overflow);
void overflowSetBlock(overflow*overflow,u8 index,u128 ref);