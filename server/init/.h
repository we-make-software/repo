#ifndef init_h
    #define init_h
    #include "../server.h"
    DefineVoid(init,execute,void);
    #ifndef HeadView
    #define HeadView
        #include "../define/.h"
    #endif
    #ifndef BodyView
    #define BodyView
        #include "../define/.c"
    #endif
#endif