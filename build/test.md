## LLVM Project

```bash
> git clone https://github.com/llvm/llvm-project.git
> cd llvm-project
> git reset --hard 3b5b5c1ec4a3095ab096dd780e84d7ab81f3d7ff
> git status --ignored
```

## Demo File

```c++
// main.cc
#include <iostream>

int main() {
    std::cout << "hello" << std::endl;
}
```

## Build-A

```bash
> cmake -G Ninja -S runtimes -B build -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind"
> ninja -C build cxx cxxabi unwind

> clang++-18 -O3 -nostdinc++ -isystem /root/llvm-project/build/include/c++/v1 -nostdlib++ -c main.cc -v
> clang++-18 main.o -nostdlib++ -L /root/llvm-project/build/lib -Wl,-Bstatic -lc++ -lc++abi -Wl,-Bdynamic -o demo -v

> llvm-strip-18 demo
3366104 byte -> 2866720 byte (3.3M -> 2.8M)
```

## Build-B

```bash
> cmake -G Ninja -S runtimes -B build -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" -DCMAKE_CXX_FLAGS="-ffunction-sections -fdata-sections"
> ninja -C build cxx cxxabi unwind

> clang++-18 -O3 -nostdinc++ -isystem /root/llvm-project/build/include/c++/v1 -nostdlib++ -c main.cc -v
> clang++-18 main.o -nostdlib++ -L /root/llvm-project/build/lib -Wl,-Bstatic -lc++ -lc++abi -Wl,-Bdynamic -Wl,--gc-sections -o demo -v

> llvm-strip-18 demo
1199248 byte -> 936936 byte (1.2M -> 916K)
```

## Build-C

```bash
> cmake -G Ninja -S runtimes -B build \
    -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" \
    -DCMAKE_CXX_FLAGS="-ffunction-sections -fdata-sections" \
    -DLIBCXX_ENABLE_RTTI=OFF \
    -DLIBCXX_ENABLE_EXCEPTIONS=OFF \
    -DLIBCXXABI_ENABLE_EXCEPTIONS=OFF \
    -DLIBCXX_ENABLE_TIME_ZONE_DATABASE=OFF \
    -DLIBCXX_ENABLE_WIDE_CHARACTERS=OFF \
    -DLIBCXX_ENABLE_FILESYSTEM=OFF
> ninja -C build cxx cxxabi unwind

> clang++-18 -O3 -fno-exceptions -fno-rtti -nostdinc++ -isystem /root/llvm-project/build/include/c++/v1 -nostdlib++ -c main.cc -v
> clang++-18 main.o -nostdlib++ -L /root/llvm-project/build/lib -Wl,-Bstatic -lc++ -lc++abi -Wl,-Bdynamic -Wl,--gc-sections -o demo -v

> llvm-strip-18 demo
382544 byte -> 330360 byte (376K -> 324K)
```

## Build-D

```
diff --git a/libcxxabi/CMakeLists.txt b/libcxxabi/CMakeLists.txt
index da998d2221dc..4146714c3459 100644
--- a/libcxxabi/CMakeLists.txt
+++ b/libcxxabi/CMakeLists.txt
@@ -284,6 +284,7 @@ if (LIBCXXABI_ENABLE_EXCEPTIONS)
   add_c_compile_flags_if_supported(-funwind-tables)
 else()
   add_compile_flags_if_supported(-fno-exceptions)
+  add_compile_flags_if_supported(-fno-rtti)
   add_compile_flags_if_supported(-EHs-)
   add_compile_flags_if_supported(-EHa-)
 endif()
diff --git a/libcxxabi/src/CMakeLists.txt b/libcxxabi/src/CMakeLists.txt
index f4722c3b352d..735ef62a520b 100644
--- a/libcxxabi/src/CMakeLists.txt
+++ b/libcxxabi/src/CMakeLists.txt
@@ -16,7 +16,7 @@ set(LIBCXXABI_SOURCES
   # Internal files
   abort_message.cpp
   fallback_malloc.cpp
-  private_typeinfo.cpp
+  # private_typeinfo.cpp
 )

 if (LIBCXXABI_ENABLE_NEW_DELETE_DEFINITIONS)
```

```bash
> cmake -G Ninja -S runtimes -B build \
    -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" \
    -DCMAKE_CXX_FLAGS="-ffunction-sections -fdata-sections -flto=full" \
    -DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=lld-18" \
    -DLIBCXX_ENABLE_RTTI=OFF \
    -DLIBCXX_ENABLE_EXCEPTIONS=OFF \
    -DLIBCXXABI_ENABLE_EXCEPTIONS=OFF \
    -DLIBCXX_ENABLE_TIME_ZONE_DATABASE=OFF \
    -DLIBCXX_ENABLE_WIDE_CHARACTERS=OFF \
    -DLIBCXX_ENABLE_FILESYSTEM=OFF \
    -DCMAKE_C_COMPILER=clang-18 -DCMAKE_CXX_COMPILER=clang++-18
> ninja -C build cxx cxxabi

> clang++-18 -O3 -flto=full -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident -nostdinc++ -isystem /root/llvm-project/build/include/c++/v1 -nostdlib++ -c main.cc -v
> clang++-18 main.o -flto=full -fuse-ld=lld-18 -nostdlib++ -L /root/llvm-project/build/lib -Wl,-Bstatic -lc++ -lc++abi -Wl,-Bdynamic -Wl,--gc-sections -o demo -v

> "/usr/bin/ld.lld-18" --lto-O3 -O3 -z relro --hash-style=gnu --build-id --eh-frame-hdr -m elf_x86_64 -pie -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o demo /lib/x86_64-linux-gnu/Scrt1.o /lib/x86_64-linux-gnu/crti.o /usr/bin/../lib/gcc/x86_64-linux-gnu/13/crtbeginS.o -L/root/llvm-project/build/lib -L/usr/bin/../lib/gcc/x86_64-linux-gnu/13 -L/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../lib64 -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib64 -L/usr/lib/llvm-18/bin/../lib -L/lib -L/usr/lib -plugin /usr/lib/llvm-18/bin/../lib/LLVMgold.so -plugin-opt=mcpu=x86-64 main.o -Bstatic -lc++ -lc++abi -Bdynamic --gc-sections -lc /usr/bin/../lib/gcc/x86_64-linux-gnu/13/crtendS.o /lib/x86_64-linux-gnu/crtn.o

> llvm-strip-18 demo
264832 byte -> 156208 byte (260K -> 156K)

> llvm-objcopy-18 --remove-section .eh_frame --remove-section .eh_frame_hdr demo
> llvm-readelf-18 -S demo
```

## Zig Build

```bash
> zig c++ -O3 -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident -c main.cc -v
> zig c++ main.o -Wl,--gc-sections -o demo -v

> zig c++ -O3 -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident main.cc -o demo
```

## Zig build klotski

```bash
# FILE: /usr/local/bin/zig-cc

#!/usr/bin/env sh
eval zig cc $@

# FILE: /usr/local/bin/zig-c++

#!/usr/bin/env sh
eval zig c++ $@

# FILE: /usr/local/bin/zig-ar

#!/usr/bin/env sh
eval zig ar $@

# FILE: /usr/local/bin/zig-ranlib

#!/usr/bin/env sh
eval zig ranlib $@
```

```bash
> cmake -DCMAKE_TOOLCHAIN_FILE=build/zig.toolchain.cmake -GNinja -Bcmake-build
> cmake --build cmake-build -v
```

## Clang build klotski

```bash
> cmake -DCMAKE_TOOLCHAIN_FILE=build/clang.toolchain.cmake -GNinja -Bcmake-build
> cmake --build cmake-build -v
```

```bash
> cmake -G Ninja -S runtimes -B build \
    -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" \
    -DCMAKE_CXX_FLAGS="-ffunction-sections -fdata-sections -flto=full" \
    -DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=lld-18" \
    -DLIBCXX_ENABLE_RTTI=OFF \
    -DLIBCXX_ENABLE_EXCEPTIONS=OFF \
    -DLIBCXXABI_ENABLE_EXCEPTIONS=OFF \
    -DLIBCXX_ENABLE_TIME_ZONE_DATABASE=OFF \
    -DCMAKE_C_COMPILER=clang-18 -DCMAKE_CXX_COMPILER=clang++-18
> ninja -C build cxx cxxabi
```

```bash
> cmake -DCMAKE_TOOLCHAIN_FILE=build/fast.toolchain.cmake -GNinja -Bcmake-build
> cmake --build cmake-build --target klotski_core -v
> cmake --build cmake-build --target bm_klsk_all_cases -v
> cmake --build cmake-build --target bm_klsk_codec -v
> cmake --build cmake-build --target bm_klsk_fast_cal -v
> cmake --build cmake-build --target bm_klsk_group -v
> cmake --build cmake-build --target bm_klsk_ranges -v
> cmake --build cmake-build --target bm_klsk_utility -v
```
