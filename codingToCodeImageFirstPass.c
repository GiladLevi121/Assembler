
#include "codingToCodeImageFirstPass.h"
#include "SettingLexTree.h"
#include "baseTransitionUtiles.h"

void codeToCodeImage(lexTree* thisLexTree) {
    /*add validation check up to this point*/
    codeFirstWordInCodeImage(thisLexTree);
}

void codeFirstWordInCodeImage(lexTree* thisLexTree){
    OrderSentence thisOrderSentence = thisLexTree->content.orderContent;
    char * binaryOpCode = getBinaryOpCode(thisOrderSentence.opcode);

}

char* getBinaryOpCode(commandOpcode thisCommandOpCode){
    return intToBinaryString(thisCommandOpCode, BINARY_OPCODE_LENGTH);
}






