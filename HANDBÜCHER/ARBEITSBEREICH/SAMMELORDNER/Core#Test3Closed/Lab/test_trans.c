// Lab/test_trans.c
#include <stdio.h>
#include "mcs_token.h"
#include "mcs_transaktion.h"

int main() {
    const char input_bytes[] = {
        0xC2, 0xA2, 0x21,          // ¢!
        0x20,                      // space
        0xC2, 0xBB,                // »
        0x5B,                      // [
        0x27, 0x65, 0x63, 0x68, 0x6F, 0x27,  // 'echo'
        0x7C,                      // |
        0x22, 0x48, 0x61, 0x6C, 0x6C, 0x6F, 0x22,  // "Hallo"
        0x5D,                      // ]
        0xC2, 0xAB,                // «
        0x20,                      // space
        0x21, 0xC2, 0xA2,         // !¢
        0x00
    };
    mcs_lexer_t* lex = mcs_lexer_new((const char*)input_bytes);
    return 0;
}
