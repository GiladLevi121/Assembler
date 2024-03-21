cmake_minimum_required(VERSION 3.25)
project(ASSEMBLER C)

set(CMAKE_C_STANDARD 99)

add_executable(ASSEMBLER assembler.c globaldefinitionsNStructures.h label.c label.h filesUtil.c filesUtil.h firstPassMainFile.c firstPassMainFile.h line.c line.h lexTree.c lexTree.h globaldefinitionsNStructures.c labelNDefineValidation.c labelNDefineValidation.h parrser.c parrser.h lexTreeValidation.c lexTreeValidation.h)
