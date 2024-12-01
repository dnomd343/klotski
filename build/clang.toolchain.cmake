set(CMAKE_C_COMPILER             "clang-18")
set(CMAKE_C_FLAGS                "-march=native")
set(CMAKE_C_FLAGS_DEBUG          "-g")
set(CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE        "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -g -DNDEBUG")

set(CMAKE_CXX_COMPILER             "clang++-18")
set(CMAKE_CXX_FLAGS                "-march=native -stdlib=libc++")
set(CMAKE_CXX_FLAGS_DEBUG          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DNDEBUG")

set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++ -fuse-ld=lld-18")
set(CMAKE_SHARED_LINKER_FLAGS "-stdlib=libc++ -fuse-ld=lld-18")

set(CMAKE_AR     "llvm-ar-18")
set(CMAKE_LINKER "clang++-18")
set(CMAKE_RANLIB "llvm-ranlib-18")