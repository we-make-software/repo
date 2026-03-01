#include ".h"

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
void OverFlowSetBlock(Overflow*overflow,u8 index,u128 ref)
{
    if(index<overflow->block.index||(index==overflow->block.index&&ref<overflow->block.ref)){
        overflow->block.index=index;
        overflow->block.ref=ref;
    }
}
void OverFlowBlock(Overflow*overflow)
{
    if(overflow->moment.index<overflow->block.index||(overflow->moment.index==overflow->block.index&&overflow->moment.ref<overflow->block.ref))
        overflow->block=overflow->moment;
}
void OverFlowInit(Overflow*overflow)
{
    overflow->moment.ref=0;
    overflow->moment.index=0;
    overflow->block.ref=(u128)-1;
    overflow->block.index=(u8)-1;
}