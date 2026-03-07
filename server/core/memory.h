#pragma once
#include ".setup"
    #define InitGlobalMemory(type) \
            static struct kmem_cache* type##_memory; \
            static inline void InitMemory_##type(void){ \
                type##_memory=kmem_cache_create(#type,sizeof(type),0,SLAB_HWCACHE_ALIGN,NULL); \
            } \
            static inline void ExitMemory_##type(void){ \
                kmem_cache_destroy(type##_memory); \
            } \
            static inline type* Alloc_##type(void){ \
                return (type*)kmem_cache_alloc(type##_memory,GFP_ATOMIC); \
            } \
            static inline void Free_##type(type*ptr){ \
                kmem_cache_free(type##_memory,ptr); \
            }

    #define InitMemory(type)InitMemory_##type()

    #define ExitMemory(type)ExitMemory_##type()

    #define MemoryAlloc(type)Alloc_##type()

    #define MemoryFree(type,ptr)Free_##type(ptr)