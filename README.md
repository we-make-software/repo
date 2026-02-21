![Elephant](./elephant.png)

<small>
    C is a programming language. It is not assembly by itself. A compiler translates C source code into machine code (often through an assembly step). In this article I only talk about one compilers: GCC.

    On Linux you can check which CPU instruction set features are available by running:
    grep -m1 -i 'flags' /proc/cpuinfo

    This will print a list of CPU “flags”. These flags show what the CPU supports, for example:

    64-bit / basics: lm, nx

    SIMD: sse, sse2, ssse3, sse4_1, sse4_2, avx, avx2, fma

    Crypto acceleration: aes, pclmulqdq, sha_ni, vaes, vpclmulqdq

    Bit/arith helpers: popcnt, bmi1, bmi2, adx

    Virtualization (AMD): svm, npt, avic

    Cache/QoS: cat_l3, cdp_l3, mba

    Security features/mitigations: smep, smap, ibrs, ibpb, stibp, ssbd

    Different compilers can generate different machine code from the same C source, and the generated code can also change depending on which CPU features (SSE/AVX/AVX2, etc.) are enabled when you compile.

    As developers, we need to optimize code and think outside the box to make it faster and improve the overall flow.

    Some developers think that making more functions means less assembly. In my view, we should write clean C and let the compiler generate the assembly for us.

</small>