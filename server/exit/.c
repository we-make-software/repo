#include ".h"
Void(exit,execute,void){
    Call(hardware_network,exit);
    ExitCache(endpoint_device);
}