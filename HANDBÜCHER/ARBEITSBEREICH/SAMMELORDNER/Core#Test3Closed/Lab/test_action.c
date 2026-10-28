// Lab/test_action.c — FINAL
#include <stdio.h>
#include "mcs_action.h"

int main() {
    // Hex-literal — garantiert korrekt
    const char input_bytes[] = {
        0xC2, 0xBB,                    // »
        0x5B,                          // [
        0x27, 0x70, 0x67, 0x72, 0x65, 0x70, 0x27,  // 'pgrep'
        0x7C,                          // |
        0xC2, 0xA8, 0x66, 0x69, 0x72, 0x65, 0x66, 0x6F, 0x78, 0xC2, 0xA8,  // ¨firefox¨
        0x7C,                          // |
        0x28, 0x31, 0x29,              // (1)
        0xC2, 0xA8, 0x24, 0x70, 0x67, 0x72, 0x65, 0x70, 0x20, 0x66, 0x69, 0x72, 0x65, 0x66, 0x6F, 0x78, 0xC2, 0xA8,  // ¨$pgrep firefox¨
        0x5D,                          // ]
        0xC2, 0xAB,                    // «
        0xC2, 0xAB, 0xC2, 0xAB, 0x74, 0x31,  // «««t1
        0x00
    };
    mcs_action_t* a = mcs_parse_action(input_bytes);
    if (!a) {
        printf("❌ Parse error\n");
        return 1;
    }

    printf("✅ Action parsed:\n");
    printf("   cmd            = %s\n", a->cmd ?: "(null)");
    printf("   feed.id        = %d\n", a->feed.id);
    printf("   blankernenner  = %s\n", a->blankernenner ?: "(null)");
    printf("   arg.type       = %d (%s)\n", a->arg.type,
           (a->arg.type == ARG_TIME) ? "TIME" : "OTHER");
    printf("   arg.value      = %d\n", a->arg.value);

    mcs_free_action(a);
    return 0;
}
