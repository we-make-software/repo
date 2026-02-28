#pragma once
#include ".h"



#define DefineStructHead(name)\
        struct s_##name;\
        typedef struct s_##name name

#define DefineStructBody(name)\
        struct s_##name

#define DefineStructHeadBody(name)\
        DefineStructHead(name);\
        DefineStructBody(name)


#define DefineNetworkStructHead(name)\
        struct __attribute__((packed))ns_##name;\
        typedef struct ns_##name name

#define DefineNetworkStructBody(name)\
        struct __attribute__((packed))ns_##name

#define DefineNetworkStructHeadBody(name)\
        DefineNetworkStructHead(name);\
        DefineNetworkStructBody(name)



#define DefineDiskStructHead(name)\
        struct __aligned((4096))ds_##name;\
        typedef struct ds_##name name

#define DefineDiskStructBody(name)\
        struct __aligned((4096))ds_##name

#define DefineDiskStructHeadBody(name)\
        DefineDiskStructHead(name);\
        DefineDiskStructBody(name)


#define NetworkAddStruct\
        struct __attribute__((packed))

#define DiskStruct(name)\
        struct __aligned((4096))ds_##name