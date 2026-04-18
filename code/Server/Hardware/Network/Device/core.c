#include ".setup"

static void Before(StructHelp(Server,Common,Sync)*scs)
{
    StructMemory(Server,Hardware,Network,Device)*sdnd=container_of(scs,StructMemory(Server,Hardware,Network,Device),scsp.scs);
    list_del(&sdnd->node);
}

static void After(StructHelp(Server,Common,Sync)*scs)
{
    StructMemory(Server,Hardware,Network,Device)*sdnd=container_of(scs,StructMemory(Server,Hardware,Network,Device),scsp.scs);
    MemoryDelete(sdnd);
}

Fn(StructMemory(Server,Hardware,Network,Device)*,Server,Hardware,Network,Device,Get)(StructAlign(Server,OSI,EUI48)*soe,struct timespec64 utc)
{
}

InitLibrary(Server,Hardware,Network,Device)
{
}

QuitLibrary(Server,Hardware,Network,Device)
{
}
