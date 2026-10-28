// =============================================================================
// include/mcs_01_datentypen.h
// Pylovara MCS Kernel – Modul 01: Datentypen
// Basierend auf Single Source of Truth (SSoT) Version 00.58
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// =============================================================================

#ifndef MCS_01_DATENTYPEN_H
#define MCS_01_DATENTYPEN_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// KERN-DATENTYPEN
// Physische Gatter-Breite im Brain-Machine-Core (BMC)
// Definiert die Anzahl der "Drähte" für eine Information
// =============================================================================

// B1 – Bit-Impuls (1 Bit)
// Standardeinheit für Wahrheiten und Sentiatoren
typedef uint8_t B1;
#define B1_ON   1   // Schalterzustand aktiv (Gatter eingeschaltet)
#define B1_OFF  0   // Schalterzustand inaktiv

// B8 – Kern-Byte (8 Bit)
// UTF-8 Zeichen, einfache Sensorwerte, Tastatureingaben
typedef uint8_t B8;

// B16 – Logik-Link (16 Bit)
// MCS-Standard-Operatoren und Befehlskennungen
typedef uint16_t B16;

// B32 – Adressierung-Pfad (32 Bit)
// Legacy-Kompatibilität (x86-Architekturen außerhalb BMC)
typedef uint32_t B32;

// B64 – High-Speed-Pfad (64 Bit)
// Moderne Adressierung, komplexe Proteine, Warp-Kanäle
typedef uint64_t B64;

// B_INF – Massen-Typ (logisch unendlich erweiterbar)
// Steuerzustand für Öffnen/Schließen von Strukturen
typedef B64 B_INF;
#define B_INF_OPEN  ((B_INF)1)
#define B_INF_CLOSE ((B_INF)0)

// =============================================================================
// ID-DNA-GENERATOR DATENTYPEN
// =============================================================================

// B_pA – Platzhalter für kryptogenetische Abgleichs-ID
typedef struct {
    B64 shadow_id;  // Wird später durch DNA-Generator gefüllt
} B_pA;

// =============================================================================
// PROTEIN-GRUNDSTRUKTUREN
// =============================================================================

// p-KEY – Kryptogenetische Identitätssignatur (variable Länge)
typedef struct {
    uint32_t length;  // Länge in Bytes
    B8*      data;    // Dynamisch allokiert – Signaturdaten
} p_KEY;

// INIT_V – Initialisierungsvektor (Start-Protein)
typedef struct {
    B64 header;       // Mindestens 64-Bit-Initialisierungsvektor
    // Erweiterbar je nach Protein-Typ
} INIT_V;

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool b1_is_on(B1 bit) {
    return bit == B1_ON;
}

static inline void b_inf_open(B_INF* state) {
    if (state) *state = B_INF_OPEN;
}

static inline void b_inf_close(B_INF* state) {
    if (state) *state = B_INF_CLOSE;
}

// =============================================================================
// Ende Modul 01
// =============================================================================

#endif // MCS_01_DATENTYPEN_H
