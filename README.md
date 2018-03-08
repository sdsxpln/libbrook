This branch is temporary and contains the new library API that will be moved to the trunk.

The intent of this branch is to write a new API compatible with the following compilers:

* `gcc` - from [GNU Compiler Collection](https://gcc.gnu.org);
* `llvm` - from [CLang Project](https://clang.llvm.org);
* `cl` - from [Visual Studio Community 2017](https://www.visualstudio.com/vs).

Bugs:

1. https://github.com/troydhanson/uthash/pull/143
2. https://github.com/troydhanson/uthash/issues/150

It seems `ut*` macros cannot be compiled in `cl` declaring the `/Wall /WX` flags and/or building for Win64, so I'm inclined to leave Visual Studio support. :disappointed: