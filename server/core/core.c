#include "../network/.init"
#define Project_Name core
#define Project_InitAndExit
#include "../project/.setup"
Project_Init_Setup{
    Init(network);
}
Project_Exit_Setup{
    Exit(network);
}
