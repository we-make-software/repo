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