#!/bin/bash
#simply generate all ...
echo "==============================="
echo "== Linux                     =="
echo "==============================="
make CLANG=1 PLATFORM=Linux -j8
make CLANG=1 PLATFORM=Linux final
echo "==============================="
echo "== Android                   =="
echo "==============================="
make PLATFORM=Android -j8
make PLATFORM=Android final
echo "==============================="
echo "== Windows                   =="
echo "==============================="
make PLATFORM=Windows -j8
make PLATFORM=Windows final
echo "==============================="
echo "== MacOs                     =="
echo "==============================="
make CLANG=1 PLATFORM=MacOs -j8
make CLANG=1 PLATFORM=MacOs final
