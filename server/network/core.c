#include ".h"
//i place this bce it can be a security risk if some other file includes this and it can cause circular dependencies
DefineStructBody(Network){
    struct packet_type pt;
};

void network_init(void)
{
    /* din network init kode */
}
void network_exit(void)
{
    /* din network exit kode */
}