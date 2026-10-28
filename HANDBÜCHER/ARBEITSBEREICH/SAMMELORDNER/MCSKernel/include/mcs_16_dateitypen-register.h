// =============================================================================
// include/mcs_16_dateitypen-register.h
// Pylovara MCS Kernel – Modul 16: Dateitypen-Register
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Registrierung und Typisierung von Dateiformaten
// =============================================================================

#ifndef MCS_16_DATEITYPEN_REGISTER_H
#define MCS_16_DATEITYPEN_REGISTER_H

#include "mcs_01_datentypen.h"           // B64
#include "mcs_07_boxis.h"                // mcs_boxis_t – Standard-Boxis-Typ
#include "mcs_14_adressierung.h"         // mcs_adresse_t – Speicherort
#include "mcs_15_mcs-cmd-register.h"     // mcs_cmd_eintrag_t – CMDs bei Laden/Ausführen
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Dateitypen-Register – Grunddefinition
// =============================================================================

// Das Dateitypen-Register ist das zentrale Archiv aller bekannten Dateiformate
// im Pylovara-System. Es ermöglicht präzise Typisierung, Validierung und
// sichere Handhabung von Dateien – analog zu den Kern-Datentypen (Modul 01),
// jedoch auf Dateiebene.
// Jeder registrierte Typ definiert:
// - Extension und Beschreibung
// - Sicherheits- und Verarbeitungsregeln
// - Standard-Boxis für Öffnen
// - Verknüpfung mit MCS-Commands (automatische Aktionen)
// - Kryptogenetische Prüfpflicht
// Dies schützt das System vor unbefugtem oder gefährlichem Code und
// gewährleistet granulare Kontrolle (z. B. *.protein nur in SANDBOX).

typedef enum {
    DATEITYP_TEXT = 0,              // Reine Textdateien (kernel-notes, info-notes)
    DATEITYP_AUSFUEHRBAR = 1,       // Ausführbares Protein (binär oder MCS-Script)
    DATEITYP_DATENTRAEGER = 2,      // Serialisierter Protein-Container
    DATEITYP_PROTONSTROM = 3,       // Dump eines Protonen-Feeds oder Warps
    DATEITYP_KONFIG = 4,            // System- oder Master-Control-Konfiguration
    DATEITYP_LOG = 5,               // redflag-notes, Diagnose-Logs
    DATEITYP_PHOTONIK = 6,          // Lichtkreis-Parameter, Resonator-Definitionen
    DATEITYP_ERWEITERUNG = 7        // Dynamisch hinzugefügt (z. B. AI-Plugin)
} dateityp_klasse_t;

// Sicherheitsstufe beim Laden und Öffnen
typedef enum {
    DATEITYP_SICHERHEIT_SANDBOX_OBLIGAT = 0, // Muss in SANDBOX geöffnet werden
    DATEITYP_SICHERHEIT_EXEC_ERLAUBT   = 1, // Darf in EXEC-Boxis ausgeführt werden
    DATEITYP_SICHERHEIT_LESBAR_NUR     = 2, // Nur Lesen – keine Ausführung
    DATEITYP_SICHERHEIT_UNBESCHRAENKT  = 3  // Voller Zugriff – nur für Kern-Dateitypen
} dateityp_sicherheit_t;

// =============================================================================
// Struktur eines Dateityp-Eintrags
// =============================================================================

typedef struct mcs_dateityp_eintrag {
    // Identifikation
    B64                 typ_id;                 // Eindeutige ID im Register
    const char*         extension;              // z. B. ".kernel-notes"
    const char*         beschreibung;           // Menschlesbare Beschreibung

    // Klassifikation
    dateityp_klasse_t   klasse;
    dateityp_sicherheit_t sicherheit;

    // Verarbeitungsregeln
    B1                  ausfuehrbar;            // 1 = Darf als Protein ausgeführt werden
    B1                  serialisierbar;         // 1 = Kann in Feed/Warp gespeichert werden
    B1                  krypt_pruefung_erforderlich; // 1 = Signatur zwingend prüfen

    // Standard-Boxis für sicheres Öffnen
    B64                 standard_boxis_typ;     // z. B. BOXIS_SANDBOX oder BOXIS_EXEC

    // Automatische Befehle
    struct mcs_cmd_eintrag* cmd_beim_laden;     // Optional: Befehl beim Laden
    struct mcs_cmd_eintrag* cmd_beim_ausfuehren; // Optional: Befehl bei Ausführung

    // Speicher- und Größenregeln
    mcs_adresse_t       standard_speicherort;   // Typischer Ablageort
    B64                 max_groesse;            // Sicherheitslimit in Bytes

    // Master-Control-Integration
    B64                 prioritaet;             // Priorität bei Verarbeitung
    B1                  immutable;              // 1 = Kern-Dateityp – unveränderbar

    // Herkunft und Integrität
    B64                 ersteller_node;
    B64                 erstellungs_zeit;
    B64                 signatur_hash;

    // Verkettung im Register
    struct mcs_dateityp_eintrag* next_typ;
    struct mcs_dateityp_eintrag* prev_typ;

    // Zukunftssicherheit (z. B. MIME-Type, AI-Klassifikation)
    B64                 reserved[6];

} mcs_dateityp_eintrag_t;

// =============================================================================
// Globales Dateitypen-Register
// =============================================================================

typedef struct mcs_dateitypen_register {
    struct mcs_dateityp_eintrag* kopf;
    struct mcs_dateityp_eintrag* schwanz;
    B64                         eintrag_anzahl;
    B64                         max_eintraege;          // Sicherheitslimit
    B1                          schreibgeschuetzt;      // Produktionsmodus
    B64                         version;
    B64                         reserved[8];
} mcs_dateitypen_register_t;

// =============================================================================
// Kern-Dateitypen (direkt aus deinem SSOT)
// =============================================================================

#define MCS_DATEITYP_KERNEL_NOTES { \
.typ_id                      = 1, \
.extension                   = ".kernel-notes", \
.beschreibung                = "Trockene, numerierte Hauptdaten (Lexikon/Index)", \
.klasse                      = DATEITYP_TEXT, \
.sicherheit                  = DATEITYP_SICHERHEIT_UNBESCHRAENKT, \
.ausfuehrbar                 = 0, \
.serialisierbar              = 1, \
.krypt_pruefung_erforderlich = 1, \
.standard_boxis_typ          = BOXIS_SENSOR, \
.cmd_beim_laden              = NULL, \
.cmd_beim_ausfuehren         = NULL, \
.standard_speicherort        = MCS_ADRESSE_HIGH_SPEED(0xKERNEL_NOTES_BASE), \
.max_groesse                 = 1024 * 1024, /* 1 MB */ \
.prioritaet                  = 90, \
.immutable                   = 1, \
.ersteller_node              = 0, \
.erstellungs_zeit            = 0, \
.signatur_hash               = 0, \
.next_typ                    = NULL, \
.prev_typ                    = NULL, \
.reserved                    = {0} \
}

#define MCS_DATEITYP_INFO_NOTES { \
.typ_id                      = 2, \
.extension                   = ".info-notes", \
.beschreibung                = "Ausführliche, semantische Erklärungen", \
.klasse                      = DATEITYP_TEXT, \
.sicherheit                  = DATEITYP_SICHERHEIT_UNBESCHRAENKT, \
.ausfuehrbar                 = 0, \
.serialisierbar              = 1, \
.krypt_pruefung_erforderlich = 1, \
.standard_boxis_typ          = BOXIS_SENSOR, \
.cmd_beim_laden              = NULL, \
.cmd_beim_ausfuehren         = NULL, \
.standard_speicherort        = MCS_ADRESSE_HIGH_SPEED(0xINFO_NOTES_BASE), \
.max_groesse                 = 10 * 1024 * 1024, /* 10 MB */ \
.prioritaet                  = 85, \
.immutable                   = 1, \
.ersteller_node              = 0, \
.erstellungs_zeit            = 0, \
.signatur_hash               = 0, \
.next_typ                    = NULL, \
.prev_typ                    = NULL, \
.reserved                    = {0} \
}

#define MCS_DATEITYP_PROTEIN_BIN { \
.typ_id                      = 100, \
.extension                   = ".protein", \
.beschreibung                = "Ausführbares oder unausführbares Protein", \
.klasse                      = DATEITYP_AUSFUEHRBAR, \
.sicherheit                  = DATEITYP_SICHERHEIT_SANDBOX_OBLIGAT, \
.ausfuehrbar                 = 1, \
.serialisierbar              = 1, \
.krypt_pruefung_erforderlich = 1, \
.standard_boxis_typ          = BOXIS_EXEC, \
.cmd_beim_laden              = NULL, \
.cmd_beim_ausfuehren         = NULL, /* später: MCS_CMD_AUSFUEHREN_PROTEIN */ \
.standard_speicherort        = MCS_ADRESSE_NULL, \
.max_groesse                 = 50 * 1024 * 1024, /* 50 MB */ \
.prioritaet                  = 95, \
.immutable                   = 1, \
.ersteller_node              = 0, \
.erstellungs_zeit            = 0, \
.signatur_hash               = 0, \
.next_typ                    = NULL, \
.prev_typ                    = NULL, \
.reserved                    = {0} \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_dateityp_ist_ausfuehrbar(const mcs_dateityp_eintrag_t* dt) {
    return dt && dt->ausfuehrbar == 1;
}

static inline bool mcs_dateityp_erfordert_sandbox(const mcs_dateityp_eintrag_t* dt) {
    return dt && dt->sicherheit == DATEITYP_SICHERHEIT_SANDBOX_OBLIGAT;
}

static inline bool mcs_dateityp_ist_immutable(const mcs_dateityp_eintrag_t* dt) {
    return dt && dt->immutable == 1;
}

static inline bool mcs_dateityp_hat_signaturpruefung(const mcs_dateityp_eintrag_t* dt) {
    return dt && dt->krypt_pruefung_erforderlich == 1;
}

// =============================================================================
// Ende Modul 16
// =============================================================================

#endif // MCS_16_DATEITYPEN_REGISTER_H
