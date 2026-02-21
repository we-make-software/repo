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