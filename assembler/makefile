assembler: assembler.o baseTransitionUtiles.o entryExternDeclaration.o filesUtil.o firstPassCoding.o firstPassMainFile.o globaldefinitionsNStructures.o label.o labelNDefineValidation.o lexTree.o lexTreeValidation.o line.o memoryImage.o parser.o secondPassLexTreeValidation.o secondPassMainFile.o SettingLexTree.o secondPassCodding.o macroUtiles.o preAssembblerMainFile.o
	gcc -ansi -pedantic -Wall assembler.o baseTransitionUtiles.o entryExternDeclaration.o filesUtil.o firstPassCoding.o firstPassMainFile.o globaldefinitionsNStructures.o label.o labelNDefineValidation.o lexTree.o lexTreeValidation.o line.o memoryImage.o parser.o secondPassLexTreeValidation.o secondPassMainFile.o SettingLexTree.o secondPassCodding.o macroUtiles.o preAssembblerMainFile.o -o assembler

assembler.o: assembler.c secondPassMainFile.h entryExternDeclaration.h memoryImage.h label.h label.h firstPassMainFile.h globaldefinitionsNStructures.h
	gcc -c -ansi -pedantic -Wall assembler.c -o assembler.o	

baseTransitionUtiles.o: baseTransitionUtiles.c baseTransitionUtiles.h globaldefinitionsNStructures.h
	gcc -c -ansi -pedantic -Wall baseTransitionUtiles.c -o baseTransitionUtiles.o

entryExternDeclaration.o: entryExternDeclaration.c entryExternDeclaration.h labelNDefineValidation.h
	gcc -c -ansi -pedantic -Wall entryExternDeclaration.c -o entryExternDeclaration.o

filesUtil.o: filesUtil.c filesUtil.h globaldefinitionsNStructures.h line.h
	gcc -c -ansi -pedantic -Wall filesUtil.c -o filesUtil.o

firstPassCoding.o: firstPassCoding.c firstPassCoding.h parser.h memoryImage.h lexTreeValidation.h baseTransitionUtiles.h SettingLexTree.h 
	gcc -c -ansi -pedantic -Wall firstPassCoding.c -o firstPassCoding.o

firstPassMainFile.o: firstPassMainFile.c firstPassMainFile.h entryExternDeclaration.h memoryImage.h firstPassCoding.h firstPassCoding.h label.h lexTreeValidation.h SettingLexTree.h line.h globaldefinitionsNStructures.h filesUtil.h 
	gcc -c -ansi -pedantic -Wall firstPassMainFile.c -o firstPassMainFile.o

globaldefinitionsNStructures.o: globaldefinitionsNStructures.h globaldefinitionsNStructures.c
	gcc -c -ansi -pedantic -Wall globaldefinitionsNStructures.c -o globaldefinitionsNStructures.o

label.o: label.c label.h globaldefinitionsNStructures.h labelNDefineValidation.h
	gcc -c -ansi -pedantic -Wall label.c -o label.o

labelNDefineValidation.o: labelNDefineValidation.c labelNDefineValidation.h parser.h globaldefinitionsNStructures.h
	gcc -c -ansi -pedantic -Wall labelNDefineValidation.c -o labelNDefineValidation.o

lexTree.o: lexTree.c lexTree.h
	gcc -c -ansi -pedantic -Wall lexTree.c -o lexTree.o

lexTreeValidation.o: lexTreeValidation.c lexTreeValidation.h parser.h labelNDefineValidation.h label.h
	gcc -c -ansi -pedantic -Wall lexTreeValidation.c -o lexTreeValidation.o	

line.o: line.c line.h globaldefinitionsNStructures.h
	gcc -c -ansi -pedantic -Wall line.c -o line.o

memoryImage.o: memoryImage.c memoryImage.h globaldefinitionsNStructures.h
	gcc -c -ansi -pedantic -Wall memoryImage.c -o memoryImage.o

parser.o: parser.c parser.h globaldefinitionsNStructures.h
	gcc -c -ansi -pedantic -Wall parser.c -o parser.o

secondPassLexTreeValidation.o: secondPassLexTreeValidation.c secondPassLexTreeValidation.h lexTreeValidation.h lexTree.h globaldefinitionsNStructures.h
	gcc -c -ansi -pedantic -Wall secondPassLexTreeValidation.c -o secondPassLexTreeValidation.o

secondPassMainFile.o: secondPassMainFile.c secondPassMainFile.h secondPassLexTreeValidation.h firstPassMainFile.h lexTreeValidation.h SettingLexTree.h lexTree.h filesUtil.h label.h globaldefinitionsNStructures.h memoryImage.h secondPassCodding.h
	gcc -c -ansi -pedantic -Wall secondPassMainFile.c -o secondPassMainFile.o	

SettingLexTree.o: SettingLexTree.c SettingLexTree.h parser.h label.h lexTree.h
	gcc -c -ansi -pedantic -Wall SettingLexTree.c -o SettingLexTree.o

secondPassCodding.o: baseTransitionUtiles.h firstPassCoding.h secondPassCodding.h secondPassCodding.c
	gcc -c -ansi -pedantic -Wall secondPassCodding.c -o secondPassCodding.o

macroUtiles.o: macroUtiles.c macroUtiles.h
	gcc -c -ansi -pedantic -Wall macroUtiles.c -o macroUtiles.o

preAssembblerMainFile.o: preAssembblerMainFile.c preAssembblerMainFile.h macroUtiles.h parser.h labelNDefineValidation.h
	gcc -c -ansi -pedantic -Wall preAssembblerMainFile.c -o preAssembblerMainFile.o






