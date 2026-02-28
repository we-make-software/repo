#include ".h"
DefineStructHeadBody(OverflowSetting){
    u128 ref;
    u8 index;
};
DefineStructBody(Overflow){
    OverflowSetting moment,block;
};
bool OverFlowIncrement(Overflow*overflow)
{
    if(overflow->moment.index>overflow->block.index||(overflow->moment.index==overflow->block.index&&overflow->moment.ref>=overflow->block.ref))
        return false;
    if(overflow->moment.ref==(u128)-1){
        overflow->moment.ref=0;
        overflow->moment.index++;
        return true;
    }

    overflow->moment.ref++;
    return true;
}
bool OverFlowDecrement(Overflow*overflow)
{
    if(overflow->moment.ref==0){
        if(overflow->moment.index==0)
            return true;
        overflow->moment.index--;
        overflow->moment.ref=(u128)-1;
        return false;
    }
    overflow->moment.ref--;
    return (overflow->moment.index==0 && overflow->moment.ref==0);
}
void OverFlowBlock(Overflow*overflow)
{
    overflow->block=overflow->moment;
}
void OverFlowInit(Overflow*overflow)
{
    overflow->moment.ref=0;
    overflow->moment.index=0;
    overflow->block.ref=(u128)-1;
    overflow->block.index=(u8)-1;
}