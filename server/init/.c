#include ".h"
Void(init,execute,void){
    InitCache(endpoint_device);
    Call(hardware_network,init);
}