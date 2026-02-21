<img src="./elephant.png" alt="Elephant">

<details>
  <summary><b>CPU flags, compilers, and optimization (click to expand)</b></summary>

  <p>
    C is a programming language. It is not assembly by itself. A compiler translates C source code into machine code (often through an assembly step). In this article I only talk about one compiler: GCC.
  </p>

  <p>
    On Linux you can check which CPU instruction set features are available by running:<br>
    <code>grep -m1 -i 'flags' /proc/cpuinfo</code>
  </p>

  <p>
    This will print a list of CPU “flags”. These flags show what the CPU supports, for example:
  </p>

  <ul>
    <li>64-bit / basics: <code>lm</code>, <code>nx</code></li>
    <li>SIMD: <code>sse</code>, <code>sse2</code>, <code>ssse3</code>, <code>sse4_1</code>, <code>sse4_2</code>, <code>avx</code>, <code>avx2</code>, <code>fma</code></li>
    <li>Crypto acceleration: <code>aes</code>, <code>pclmulqdq</code>, <code>sha_ni</code>, <code>vaes</code>, <code>vpclmulqdq</code></li>
    <li>Bit/arith helpers: <code>popcnt</code>, <code>bmi1</code>, <code>bmi2</code>, <code>adx</code></li>
    <li>Virtualization (AMD): <code>svm</code>, <code>npt</code>, <code>avic</code></li>
    <li>Cache/QoS: <code>cat_l3</code>, <code>cdp_l3</code>, <code>mba</code></li>
    <li>Security features/mitigations: <code>smep</code>, <code>smap</code>, <code>ibrs</code>, <code>ibpb</code>, <code>stibp</code>, <code>ssbd</code></li>
  </ul>

  <p>
    Different machine code can be produced from the same C source depending on the compiler and which CPU features (SSE/AVX/AVX2, etc.) are enabled at compile time.
  </p>

  <p>
    As developers, we need to optimize code and think outside the box to make it faster and improve the overall flow.
  </p>

  <p>
    Some developers think that making more functions means less assembly. In my view, we should write clean C and let the compiler generate the assembly for us.
  </p>
</details>