// OrdnerPfad: /Pylovara/MCSKernel/Logic/mcs_alu_prozessor.h
#ifndef MCS_ALU_PROZESSOR_H
#define MCS_ALU_PROZESSOR_H

// @kernel-nr: 01 | b64 High-Speed-Pfad
static inline long ALU_LADE_B64(long wert) {
    long ergebnis;
    asm ("movq %1, %0" : "=r" (ergebnis) : "r" (wert));
    return ergebnis;
}

// @kernel-nr: 09 | ALU REINIGEN
static inline void ALU_PURGE_GATTER(void* ziel) {
    asm volatile ("mfence" ::: "memory");
}

// @kernel-nr: 16 | AI Synctimer / Takt-Synchronisation
static inline void ALU_TAKT_SYNC(void) {
    // Hardware-Fence zur Sicherstellung der Gatter-Stabilität
    asm volatile ("mfence" ::: "memory");
    // Optionale Pause für x86_64 zur Stromersparnis im Takt
    asm volatile ("pause" ::: "memory");
}

#endif
