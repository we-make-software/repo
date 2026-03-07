#include ".modify"

bool overflowIncrement(overflow*ov)
{
    if(ov->moment.index>ov->block.index||(ov->moment.index==ov->block.index&&ov->moment.ref>=ov->block.ref))
        return false;
    if(ov->moment.ref==(u128)-1){
        ov->moment.ref=0;
        ov->moment.index++;
        return true;
    }
    ov->moment.ref++;
    return true;
}
bool overflowDecrement(overflow*ov)
{
    if(ov->moment.ref==0){
        if(ov->moment.index==0)
            return true;
        ov->moment.index--;
        ov->moment.ref=(u128)-1;
        return false;
    }
    ov->moment.ref--;
    return (ov->moment.index==0 && ov->moment.ref==0);
}
void overflowSetBlock(overflow*ov,u8 index,u128 ref)
{
    if(index<ov->block.index||(index==ov->block.index&&ref<ov->block.ref)){
        ov->block.index=index;
        ov->block.ref=ref;
    }
}
void overflowBlock(overflow*ov)
{
    if(ov->moment.index<ov->block.index||(ov->moment.index==ov->block.index&&ov->moment.ref<ov->block.ref))
        ov->block=ov->moment;
}
void overflowInit(overflow*ov)
{
    ov->moment.ref=0;
    ov->moment.index=0;
    ov->block.ref=(u128)-1;
    ov->block.index=(u8)-1;
}