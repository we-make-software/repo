Core means, to me, the central, essential part.

In code, it typically means the part that is the foundation, the minimum functionality that everything else is built on top of.

In a computer context, “core” can also mean a CPU core, a physical or logical core that can run threads.

When you write something like server_core_init(), “core” usually means “the main part of the server”, the part that actually sets up the most important things, while the rest is just a wrapper, glue code, or extra features.

Why a function does not inline across different .o files

Many developers think the keyword inline forces the compiler to inline a function everywhere. It does not. In C and C++, inline is mostly a hint and a linkage rule. The compiler is always allowed to ignore it and emit a normal call.

The key rule

A compiler can only inline a function when it can see the full function body at the call site during compilation.

GCC compiles each .c file separately into its own .o file. Each .c file is a separate translation unit. During that step, the compiler only knows what is inside that one translation unit, plus what is included by headers.

If you only have a prototype in the header, but the function body is in a different .c file, then the caller cannot see the body. The compiler cannot inline what it cannot see. Result: you get a normal function call across .o files.

What works without LTO

If you want inlining without any special build setup, you must put the full function body in a header, so every .c file that includes the header can see it.

In practice, the common pattern is:

Put small helper functions in headers

Mark them static inline so each translation unit can inline them, and so you avoid multiple definition linker problems

That is why performance critical code and kernel code often keeps tiny helpers in headers.

What LTO changes

LTO means Link Time Optimization.

Normally, the compiler cannot optimize across .o files because each .o is built from one .c file in isolation. With LTO enabled, the build keeps extra information so the toolchain can optimize at link time with a whole program view. That can enable inlining across different .o files.

Why kernel modules often break when you “just add -flto”

For Linux kernel modules, you usually cannot force real LTO by only adding -flto to one external module Makefile. The kernel build system has its own rules for LTO and linking. If the kernel itself is not built with the matching LTO setup, forcing -flto in just one module can produce warnings and failures during modpost or linking.

The practical conclusion

If you want reliable “across files inline” without LTO, the function body must be in the header.

If you want the body in a separate .c and still inline across .o files, then you need LTO, and it should be enabled for the whole build setup, not only for one module.

Share, follow, and discuss with the top pro developers in this group about C and C++. Even if you have a task, just follow the rules. Sometimes the rules can change to guide the group and help everyone.

Admins, moderators, and users are here to help you and keep this group a professional place.

If you are shy, or you have any reason not to show your real profile, we have anonymous options for this group. You can set a nickname for this group only, so nobody can see who you are as a member. Only admins and moderators can see your real account, and we will not share who you are.

You can also choose a profile photo for this group only.
Example nickname: big_nerd2026 😄

We just hit over 563k users. Please share and post content related to C and C++. Ask if you need help. We have many users ready to help you.

Visibility: 0% to 100%. 