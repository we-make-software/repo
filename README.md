<img src="./elephant.png" alt="Elephant">

<details>
  <summary><b>CPU flags & what they mean (GCC)</b></summary>
  <br>

  <p>
    C is a programming language, not assembly. A compiler (here: GCC) translates C source code into machine code.
  </p>

  <p>
    Check CPU features on Linux:
    <br>
    <code>grep -m1 -i 'flags' /proc/cpuinfo</code>
  </p>

  <p>Common flags:</p>

  <ul>
    <li><b>64-bit:</b> <code>lm</code>, <code>nx</code></li>
    <li><b>SIMD:</b> <code>sse</code>, <code>sse2</code>, <code>ssse3</code>, <code>sse4_1</code>, <code>sse4_2</code>, <code>avx</code>, <code>avx2</code>, <code>fma</code></li>
    <li><b>Crypto:</b> <code>aes</code>, <code>pclmulqdq</code>, <code>sha_ni</code>, <code>vaes</code>, <code>vpclmulqdq</code></li>
    <li><b>Bit/arith:</b> <code>popcnt</code>, <code>bmi1</code>, <code>bmi2</code>, <code>adx</code></li>
    <li><b>Virt (AMD):</b> <code>svm</code>, <code>npt</code>, <code>avic</code></li>
    <li><b>Cache/QoS:</b> <code>cat_l3</code>, <code>cdp_l3</code>, <code>mba</code></li>
    <li><b>Security:</b> <code>smep</code>, <code>smap</code>, <code>ibrs</code>, <code>ibpb</code>, <code>stibp</code>, <code>ssbd</code></li>
  </ul>

  <p>
    The machine code you get can change depending on enabled CPU features (SSE/AVX/AVX2, etc.) and compile flags.
  </p>

  <p>
    As developers, we optimize for performance and flow. Write clean C, and let the compiler generate efficient assembly.
  </p>
</details>
<details>
  <summary><b>Human-readable C vs. hard-to-read ASM</b></summary>

  <p>
    Human-readable code matters. Assembly can be hard to read, even when it is correct.
    C often makes the intent obvious: you can see the logic directly, while the compiler
    translates it into machine instructions.
  </p>

  <p>
    Most software is built on standards. Sometimes we follow existing standards, and
    sometimes we define internal standards for our own projects to keep code consistent.
  </p>

</details>
<details>
  <summary><b>Garbage Collection, Platforms, GCC, and building a DSL</b></summary>
  <p>
    Let’s be clear up front: C has no built-in garbage collector.
    You manage memory yourself, and that is part of the language’s power and responsibility.
  </p>

  <p>
    If you don’t want to build an entire operating system from scratch, you can use an existing platform.
    A common choice is the Linux kernel (for example, Torvalds’ repository), and you can even test against
    release candidates (RC) if you want the newest changes.
  </p>

  <p>
    Make sure GCC is installed. It can be valuable to experiment with the newest GCC versions:
    better diagnostics, new optimizations, and sometimes a “better” way to write code (in the sense of what
    the compiler warns about and what it can optimize cleanly).
  </p>

  <p>
    And if plain C is not enough for your style, we can build our own DSL on top of C
    (macros, code generation, or a small preprocessor) while still producing standard C that GCC can compile.
  </p>
</details>
<details>
  <summary><b>C and C++ are not the same</b></summary>

  <p>
    C and C++ are not the same language.
    C is often used where you want direct control, such as low level code, kernels, drivers, and tight systems code.
  </p>

  <p>
    C++ is most commonly used in user space. In user space you have a full platform around you:
    standard libraries, runtimes, allocators, exceptions and RTTI if enabled, and higher level abstractions.
    That can make development faster, but it can also add overhead if you are not careful.
  </p>

  <p>
    C++ does not have a built in garbage collector by default, but user space gives you many memory management
    models and frameworks, so the platform often feels closer to managed systems than bare metal C.
  </p>

  <p>
    Some things must be user space. In my opinion, that should mainly be user view logic,
    such as UI, apps, and high level behavior. The low level core should stay simple and controlled.
  </p>
</details>
<details>
  <summary><b>Between the compiler and the code</b></summary>

  <p>
    The Makefile sits between your code and the compiler.
    It defines how files are compiled, which flags are used, which libraries are linked,
    and how the final output is built.
  </p>

  <p>
    Small changes in the Makefile can change the generated machine code,
    because optimization levels, target CPU settings, and link options matter.
  </p>
</details>
<details>
  <summary><b>Cross build: kernel object for testing, vmlinuz for real development</b></summary>

  <p>
    I cross build in two modes.
    First, I build a kernel object for fast testing and iteration.
    Second, I build a full vmlinuz image when I want real development and full system validation.
  </p>

  <p>
    The goal is simple: quick feedback during experiments, and a complete bootable kernel when it matters.
  </p>
</details>
<details>
  <summary><b>GitHub push and teamwork</b></summary>

  <p>
    Teamwork is great as long as the team understands the basics.
    There are many ways to solve a problem. The goal is not to teach a team member how to code,
    but to understand the logic behind a choice.
  </p>

  <p>
    Information is valuable, and GitHub is a great place to share it.
  </p>
</details>
<details>
  <summary><b>Smart coding, fast coding, and why a DSL helps</b></summary>

  <p>
    You should do things that make coding smart, but you should also keep it fast.
    There can be obstacles, or a different approach than you first expected.
  </p>

  <p>
    That is why a DSL can be useful. It creates a bridge between the compiler, testing,
    and the way you write code. The goal is to iterate quickly while still producing
    clean C that the compiler can optimize.
  </p>
</details>
<details>
  <summary><b>Servers, trust, and building a platform</b></summary>

  <p>
    A server is a place where you receive and process data, and where you build a good relationship with users.
    There are many malicious tools and sometimes hostile people.
  </p>

  <p>
    If you follow the laws in different countries and provide the best possible safety for users,
    you may be able to build a large platform.
  </p>
</details>
<details>
  <summary><b>VS Code IntelliSense bypass for kernel headers</b></summary>

  <p>
    VS Code IntelliSense can report false errors when working with Linux kernel headers.
    I use this block to bypass missing macros and types in the editor, without changing real builds.
  </p>

  <pre><code>
#ifdef __INTELLISENSE__
  #define MODULE_LICENSE(x)
  #define MODULE_AUTHOR(x)
  #define MODULE_DESCRIPTION(x)
  typedef struct { s64 counter; } atomic64_t;
  #define READ_ONCE(x) (x)
  #define WRITE_ONCE(x, v) ((x) = (v))
#endif
  </code></pre>
</details>
<details>
  <summary><b>Headers depend on the platform</b></summary>

  <p>
    The headers you include depend on the platform you build against.
    When working on Linux kernel code, you will typically include Linux kernel headers
    for the subsystem you are targeting.
  </p>

  <pre><code>
#include &lt;linux/module.h&gt;
#include &lt;linux/kernel.h&gt;
#include &lt;linux/notifier.h&gt;
#include &lt;linux/reboot.h&gt;
#include &lt;linux/ethtool.h&gt;
#include &lt;linux/netdevice.h&gt;
#include &lt;linux/skbuff.h&gt;
#include &lt;linux/etherdevice.h&gt;
#include &lt;linux/if_packet.h&gt;
#include &lt;linux/ktime.h&gt;
#include &lt;linux/timekeeping.h&gt;
#include &lt;linux/slab.h&gt;
#include &lt;linux/types.h&gt;
#include &lt;linux/atomic.h&gt;
#include &lt;linux/blkdev.h&gt;
#include &lt;linux/mutex.h&gt;
#include &lt;linux/rtnetlink.h&gt;
  </code></pre>
</details>
<details>
  <summary><b>Why inline can be useful</b></summary>

  <p>
    Inline can remove the function call overhead. No call and return, and no extra stack setup.
    That can matter in hot paths.
  </p>

  <p>
    Inline also gives the compiler more context, so it can optimize across the call boundary.
    It can propagate constants, fold branches, and remove unused work.
  </p>

  <p>
    The downside is bigger code size if you inline too much, which can hurt instruction cache.
    That is why I use it where it matters, not everywhere.
  </p>
</details>
<details>
  <summary><b>Inline, macros, and define</b></summary>

  <p>
    I use macros to enforce a consistent style for function declarations and inlining.
    The goal is simple: make intent explicit and keep the compiler free to inline aggressively.
  </p>
<pre><code class="language-c">

    #define Inline(type)\
            static inline type __attribute__((always_inline,flatten,__unused__))

    #define Function(type,project,name,...)\
            Inline(type) project##_##name(__VA_ARGS__)

    #define Void(project,name,...)\
            Function(void,project,name,__VA_ARGS__)

    #define DefineFunction(type,project,name,...)\
            Function(type,project,name,__VA_ARGS__)

    #define DefineVoid(project,name,...)\
            Void(project,name,__VA_ARGS__)
    
    #define Call(project,name,...)\
            project##_##name(__VA_ARGS__)

</code></pre>

  <p>
    This creates one pattern for function declarations, so the code base stays uniform.
    It also avoids repeating attributes and keywords everywhere.
  </p>
</details>
<details>
  <summary><b>void in the parameter list</b></summary>

  <p>
    In C, <code>f()</code> in a declaration means “unknown parameter list”.
    If the function takes zero arguments, write <code>f(void)</code>.
    This also avoids <code>-Werror=strict-prototypes</code> errors when using my variadic macros.
  </p>

<pre><code class="language-c">
    /* 0 parameters */
    DefineVoid(init,execute,void);
    Void(init,execute,void){
    }
    Call(init,execute);

    /* with parameters */
    DefineVoid(net,rx,buffer buff,NS*ns);
    Void(net,rx,buffer buff,NS*ns){
    }
    Call(net,rx,buff,ns);
</code></pre>
</details>
<details>
  <summary><b>Global cache allocator macros</b></summary>

  <p>
    <code>InitGlobalCache(type)</code> generates a single global <code>kmem_cache</code> for a struct type and a small API around it.
    It is meant for fast allocations in hot paths by reusing slab objects instead of calling <code>kmalloc</code> for every instance.
  </p>

  <p>
    What it generates for each <code>type</code>:
    a static <code>type_cache</code> pointer,
    <code>InitCache_type()</code> which creates the cache with <code>kmem_cache_create</code> and <code>BUG_ON</code> if it fails,
    <code>ExitCache_type()</code> which destroys the cache,
    <code>Alloc_type()</code> which allocates with <code>GFP_ATOMIC</code>,
    and <code>Free_type(ptr)</code> which frees back to the cache.
  </p>

  <p>
    Call pattern:
    initialize once at startup with <code>InitCache(type)</code>,
    allocate with <code>CacheAlloc(type)</code>,
    free with <code>CacheFree(type,ptr)</code>,
    and destroy at shutdown with <code>ExitCache(type)</code>.
  </p>
</details>
<details>
  <summary><b>Struct definition helpers</b></summary>

  <p>
    These macros enforce a consistent naming scheme for types and make intent explicit at the declaration site.
    I always forward declare the underlying struct name, create a typedef alias, then open the struct body in one step.
  </p>

  <p>
    <code>DefineStruct(name)</code> creates a normal struct type using the internal name <code>s_name</code> and typedefs it to <code>name</code>.
    This keeps user facing types short while still having a predictable internal symbol name.
  </p>

  <p>
    <code>DefineNetworkStruct(name)</code> does the same but marks the struct as <code>packed</code>, for network headers and on wire layouts where padding must not be inserted.
    <code>NetworkAddStruct</code> is a helper for declaring additional packed structs inline.
  </p>

  <p>
    <code>DefineDiskStruct(name)</code> is for disk aligned data. It forward declares an internal <code>ds_name</code> type and typedefs it to <code>name</code>.
    <code>DiskStruct(name)</code> opens the aligned struct body, aligned to 4096 bytes, matching my sector based storage layout.
  </p>
</details>
<details>
  <summary><b>Core typedef aliases</b></summary>

  <p>
    I use short aliases for common kernel types to keep call sites compact and consistent across the project.
    <code>Lock</code> maps to <code>struct mutex</code>, <code>buffer</code> maps to <code>struct sk_buff*</code>, and <code>list</code> maps to <code>struct list_head</code>.
  </p>
</details>

<details>
  <summary><b>List helpers</b></summary>

  <p>
    Small wrappers around the kernel list API to make list operations read like intent.
    <code>InitList</code> initializes a list head, <code>AddList</code> inserts a node into a list, and <code>UpdateList</code> moves an existing node to a new position or head.
  </p>
</details>

<details>
  <summary><b>Field assignment helpers</b></summary>

  <p>
    <code>Set</code> and <code>Clear</code> are used to make direct field writes explicit.
    <code>Set</code> assigns a value, <code>Clear</code> clears a bitmask from a field.
  </p>
</details>

<details>
  <summary><b>Lock helpers</b></summary>

  <p>
    I wrap the kernel mutex API to keep lock usage consistent with the rest of my DSL helpers.
    <code>InitLock</code> initializes a mutex, <code>WaitForLock</code> acquires it, and <code>ReleaseLock</code> releases it.
  </p>
</details>

<details>
  <summary><b>Online status flag</b></summary>

  <p>
    A single global runtime flag used as a fast gate in hot paths.
    <code>OnlineStatus</code> reads the current state using <code>READ_ONCE</code> to avoid compiler reordering and tearing.
  </p>
</details>
<details>
  <summary><b>Network device hook and offload control</b></summary>

  <p>
    Purpose: attach a packet handler to every non loopback, non usb network device and keep state per device while the service is running.
  </p>

  <p>
    Init flow: iterate <code>for_each_netdev</code>, skip devices that are loopback, have a zero MAC, or are behind a usb bus. For each remaining device allocate a <code>hardware_network</code> entry, store the current <code>features</code>, <code>wanted_features</code>, and <code>gso_partial_features</code>. Then clear GRO, LRO, GSO, TSO, checksum offloads, SG, and VLAN offload flags, call <code>netdev_update_features</code>, and finally hook <code>ETH_P_ALL</code> via <code>dev_add_pack</code>.
  </p>

  <p>
    Exit flow: for each tracked device remove the packet hook with <code>dev_remove_pack</code>, restore the saved feature flags, call <code>netdev_update_features</code>, and free the entry. Uses RTNL around feature changes and pack add remove, and calls <code>synchronize_net</code> after init and exit to flush in flight packets.
  </p>
</details>