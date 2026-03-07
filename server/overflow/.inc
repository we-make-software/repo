#pragma once
#include ".modify"
void overflowInit(overflow*);
bool overflowIncrement(overflow*);
bool overflowDecrement(overflow*);
void overflowBlock(overflow*);
void overflowSetBlock(overflow*,u8,u128);