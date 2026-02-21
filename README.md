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
<pre><code>
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
</code></pre>

  <p>
    This creates one pattern for function declarations, so the code base stays uniform.
    It also avoids repeating attributes and keywords everywhere.
  </p>
</details>