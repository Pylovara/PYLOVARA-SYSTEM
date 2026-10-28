#ifndef MCS_ALU_RECHNER_H
#define MCS_ALU_RECHNER_H

#include <stdint.h>

// Verarbeitet den Inhalt einer Boxi [× ... ×]
// Kann einfache Mathe und Register-Referenzen wie (1) + 500
long mcs_alu_berechne(const char* ausdruck);

// Führt Logik-Operationen für Wahrheiten aus (==, !=, >, <)
int mcs_alu_vergleiche(long a, long b, const char* op);

#endif
