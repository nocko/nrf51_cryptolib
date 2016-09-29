#!/usr/bin/env bash
clang-tidy \
    -fix \
    -fix-errors \
    -header-filter=.* \
    --checks=readability-braces-around-statements,misc-macro-parentheses \
    *.c \
    -- -DNRF51 -I. -I ../../../nrf-sdk/6.1/nrf51822/Include -I ../../../nrf-sdk/6.1/nrf51822/Include/gcc
clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4, AllowShortFunctionsOnASingleLine: None, KeepEmptyLinesAtTheStartOfBlocks: false}" -i *.{h,c}
