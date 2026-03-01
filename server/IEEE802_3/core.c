#include ".h"
void IEEE802_3_RX(struct sk_buff*skb,Network*network,u8*buff){
    IEEE802_3*ieee802_3=(IEEE802_3*)buff;
    
    printk(KERN_INFO "IEEE802_3_RX:OK");
}