#pragma once
#include ".setup"

#define StructLock(...)                 struct mutex __VA_ARGS__

#define InitLock(lock)                  mutex_init(&(lock))
#define WaitForLock(lock)               mutex_lock(&(lock))
#define ReleaseLock(lock)               mutex_unlock(&(lock))

/* init many */

#define InitLockManyPtr(...) \
    ({ \
        struct mutex *locks[] = { __VA_ARGS__, NULL }; \
        int i = 0; \
        while(locks[i]){ \
            mutex_init(locks[i]); \
            i++; \
        } \
        0; \
    })

#define InitLockMany1(a)                 InitLockManyPtr(&(a))
#define InitLockMany2(a,b)               InitLockManyPtr(&(a),&(b))
#define InitLockMany3(a,b,c)             InitLockManyPtr(&(a),&(b),&(c))
#define InitLockMany4(a,b,c,d)           InitLockManyPtr(&(a),&(b),&(c),&(d))
#define InitLockMany5(a,b,c,d,e)         InitLockManyPtr(&(a),&(b),&(c),&(d),&(e))
#define InitLockMany6(a,b,c,d,e,f)       InitLockManyPtr(&(a),&(b),&(c),&(d),&(e),&(f))

#define _PickInitLockMany(_1,_2,_3,_4,_5,_6,NAME,...) NAME
#define InitLockMany(...) \
    _PickInitLockMany(__VA_ARGS__, \
        InitLockMany6,InitLockMany5,InitLockMany4,InitLockMany3,InitLockMany2,InitLockMany1 \
    )(__VA_ARGS__)

/* lock many */

#define WaitForLockManyPtr(...) \
    ({ \
        struct mutex *locks[] = { __VA_ARGS__, NULL }; \
        int i = 0; \
        while(locks[i]){ \
            mutex_lock(locks[i]); \
            i++; \
        } \
        0; \
    })

#define WaitForLockMany1(a)              WaitForLockManyPtr(&(a))
#define WaitForLockMany2(a,b)            WaitForLockManyPtr(&(a),&(b))
#define WaitForLockMany3(a,b,c)          WaitForLockManyPtr(&(a),&(b),&(c))
#define WaitForLockMany4(a,b,c,d)        WaitForLockManyPtr(&(a),&(b),&(c),&(d))
#define WaitForLockMany5(a,b,c,d,e)      WaitForLockManyPtr(&(a),&(b),&(c),&(d),&(e))
#define WaitForLockMany6(a,b,c,d,e,f)    WaitForLockManyPtr(&(a),&(b),&(c),&(d),&(e),&(f))

#define _PickWaitForLockMany(_1,_2,_3,_4,_5,_6,NAME,...) NAME
#define WaitForLockMany(...) \
    _PickWaitForLockMany(__VA_ARGS__, \
        WaitForLockMany6,WaitForLockMany5,WaitForLockMany4,WaitForLockMany3,WaitForLockMany2,WaitForLockMany1 \
    )(__VA_ARGS__)

/* unlock many (reverse order) */

#define ReleaseLockManyPtr(...) \
    ({ \
        struct mutex *locks[] = { __VA_ARGS__, NULL }; \
        int n = 0; \
        while(locks[n]) n++; \
        while(n-- > 0){ \
            mutex_unlock(locks[n]); \
        } \
        0; \
    })

#define ReleaseLockMany1(a)              ReleaseLockManyPtr(&(a))
#define ReleaseLockMany2(a,b)            ReleaseLockManyPtr(&(a),&(b))
#define ReleaseLockMany3(a,b,c)          ReleaseLockManyPtr(&(a),&(b),&(c))
#define ReleaseLockMany4(a,b,c,d)        ReleaseLockManyPtr(&(a),&(b),&(c),&(d))
#define ReleaseLockMany5(a,b,c,d,e)      ReleaseLockManyPtr(&(a),&(b),&(c),&(d),&(e))
#define ReleaseLockMany6(a,b,c,d,e,f)    ReleaseLockManyPtr(&(a),&(b),&(c),&(d),&(e),&(f))

#define _PickReleaseLockMany(_1,_2,_3,_4,_5,_6,NAME,...) NAME
#define ReleaseLockMany(...) \
    _PickReleaseLockMany(__VA_ARGS__, \
        ReleaseLockMany6,ReleaseLockMany5,ReleaseLockMany4,ReleaseLockMany3,ReleaseLockMany2,ReleaseLockMany1 \
    )(__VA_ARGS__)

#define GlobalLock(...)        struct mutex __VA_ARGS__
#define DeclareLock(name)      extern struct mutex name

    /*
How to use lock macros

1) Put a mutex inside a struct

DefineStruct(obj){
    StructLock(lock);
};

2) Init one lock

struct obj a;
InitLock(a.lock);

3) Init many locks (no need to type &)

struct obj b,c;
InitLockMany(a.lock,b.lock,c.lock);

4) Lock one

WaitForLock(a.lock);
critical section 
ReleaseLock(a.lock);

5) Lock many

WaitForLockMany(a.lock,b.lock,c.lock);
critical section 
ReleaseLockMany(a.lock,b.lock,c.lock);

ReleaseLockMany unlocks in reverse order compared to the lock order.
That makes the pair behave like a stack.
*/