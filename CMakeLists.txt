cmake_minimum_required(VERSION 3.25)
project(ASSEMBLER C)

set(CMAKE_C_STANDARD 90)

add_compile_options(-Wall)

add_executable(ASSEMBLER assembler.c globaldefinitionsNStructures.h label.c label.h filesUtil.c filesUtil.h firstPassMainFile.c firstPassMainFile.h line.c line.h SettingLexTree.c SettingLexTree.h globaldefinitionsNStructures.c labelNDefineValidation.c labelNDefineValidation.h parser.c parser.h lexTreeValidation.c lexTreeValidation.h firstPassCoding.c firstPassCoding.h baseTransitionUtiles.h baseTransitionUtiles.c lexTree.h lexTree.c memoryImage.h memoryImage.c secondPassMainFile.h secondPassMainFile.c entryExternDeclaration.h entryExternDeclaration.c secondPassLexTreeValidation.c secondPassLexTreeValidation.h preAssembblerMainFile.c preAssembblerMainFile.h macroUtiles.c macroUtiles.h parser.h parser.h parser.h secondPassCodding.c secondPassCodding.h)
