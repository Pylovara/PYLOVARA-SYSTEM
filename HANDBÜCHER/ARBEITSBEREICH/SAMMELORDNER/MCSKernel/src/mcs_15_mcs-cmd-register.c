// =============================================================================
// mcs_15_mcs-cmd-register.c
// Pylovara MCS Kernel - Modul 15: MCS-CMD-Register
// Basierend auf SSOT Version 00.58 / 00.59
// Status: Zentrales Befehls- und Operationsregister
// =============================================================================

#ifndef MCS_15_MCS_CMD_REGISTER_C
#define MCS_15_MCS_CMD_REGISTER_C

#include "mcs_01_datentypen.c"        // B1, B8, B16, B32, B64
#include "mcs_02_transaktionsrahmen.c" // mcs_transaktionsrahmen_t
#include "mcs_06_aktion.c"            // mcs_aktion_t (CMD löst Aktionen aus)
#include "mcs_14_adressierung.c"     // mcs_adresse_t (Zieladressierung von CMDs)
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>   // für malloc
#include <string.h>    // für memset
// =============================================================================
// MCS-CMD-Register - Grunddefinition
// =============================================================================

// Das MCS-CMD-Register ist das zentrale Befehlsregister des Pylovara-Systems.
// Es enthält alle verfügbaren Master-Control-System-Befehle (MCS-Commands),
// die innerhalb eines Transaktionsrahmens (¢! ... !¢) ausgeführt werden können.
// Befehle können ausführbare Proteine starten, Aktionen triggern, Register
// manipulieren oder systemweite Steuerung übernehmen.
// Das Register ist immutable im Kern (wie Wahrheiten/Regeln), aber erweiterbar
// durch dynamische Einträge (z. B. AI-generierte CMDs).

typedef enum {
    CMD_TYP_SYSTEM      = 0,     // Kernbefehle (unveränderbar, z. B. feed cache clear)
    CMD_TYP_AKTION      = 1,     // Direkte Aktionstrigger (SCHUB, WARP, etc.)
    CMD_TYP_CONTROL     = 2,     // Master-Control-Operationen (Node-Switch, Sync)
    CMD_TYP_ERWEITERUNG = 3,     // Dynamisch hinzugefügt (z. B. durch AI oder Plugin)
    CMD_TYP_DEBUG       = 4      // Nur in Entwicklung/Sandbox (redflag-log, dump)
} mcs_cmd_typ_t;

// Status eines CMD-Eintrags
typedef enum {
    CMD_STATUS_AKTIV     = 0,
    CMD_STATUS_INAKTIV   = 1,    // Deaktiviert durch Master-Control
    CMD_STATUS_GESPERRT  = 2,    // Sicherheitsblockade
    CMD_STATUS_EXPERIMENTELL = 3 // Nur in Baustelle-Modus
} mcs_cmd_status_t;

// =============================================================================
// Hauptstruktur eines CMD-Eintrags im Register
// =============================================================================

typedef struct mcs_cmd_eintrag {
    // Identifikation
    B64                  cmd_id;             // Eindeutige Befehls-ID
    const B8*            cmd_name;           // Menschlesbarer Name (z. B. "feed cache clear")
    mcs_cmd_typ_t        typ;

    // Status und Sicherheit
    mcs_cmd_status_t     status;
    B1                   immutable;          // 1 = Kernbefehl, darf nicht gelöscht werden

    // Parameter-Spezifikation (wie viele und welche Typen erwartet)
    B64                  parameter_anzahl;   // 0 = kein Parameter, 1-x = Argumente
    // Array von Typen könnte später hinzugefügt werden (z. B. B64, Adresse, etc.)

    // Ausführungsziel
    mcs_adresse_t        standard_ziel;      // Default-Adresse (z. B. globaler Feed-Cache)
    mcs_aktion_t*        ausloesende_aktion; // Direkte Aktion bei Ausführung (optional)

    // Timing und Priorität
    B64                  prioritaet;         // Standardpriorität bei Ausführung
    B64                  max_ausfuehrungszeit; // Obergrenze in Zyklen (Schutz vor Hängen)

    // Herkunft und Validierung
    B64                  ersteller_node;     // Welcher Node hat den CMD registriert
    B64                  erstellungs_zeit;
    B64                  signatur_hash;      // Kryptogenetische Integrität

    // Verkettung im Register
    struct mcs_cmd_eintrag* next_cmd;
    struct mcs_cmd_eintrag* prev_cmd;

    B64                  reserved[6];

} mcs_cmd_eintrag_t;

// =============================================================================
// Das globale MCS-CMD-Register (Singleton-ähnlich)
// =============================================================================

typedef struct mcs_cmd_register {
    mcs_cmd_eintrag_t*   kopf;               // Erster Eintrag (Kernbefehle)
    mcs_cmd_eintrag_t*   schwanz;            // Letzter Eintrag (für schnelles Anhängen)
    B64                  eintrag_anzahl;
    B64                  max_eintraege;          // Sicherheitslimit
    B1                   schreibgeschuetzt;      // 1 = Nur Kernbefehle erlaubt (Produktion)
    B64                  version;                // Register-Version (bei Änderungen inkrementiert)
    B64                  reserved[8];

} mcs_cmd_register_t;

// =============================================================================
// Konstanten für Kernbefehle (Beispiele aus deinem SSOT)
// =============================================================================

#define CMD_FEED_CACHE_CLEAR { \
.cmd_id              = 1, \
.cmd_name            = "feed cache clear", \
.typ                 = CMD_TYP_SYSTEM, \
.status              = CMD_STATUS_AKTIV, \
.immutable           = 1, \
.parameter_anzahl    = 1, \
.standard_ziel       = ADRESSE_HIGH_SPEED(0xFFFF0000), /* Beispiel globaler Cache */ \
.ausloesende_aktion  = NULL, \
.prioritaet          = 95, \
.max_ausfuehrungszeit = 100, \
.ersteller_node      = 0, \
.erstellungs_zeit    = 0, \
.signatur_hash       = 0, \
.next_cmd            = NULL, \
.prev_cmd            = NULL \
}

#define CMD_SYS_WRITE { \
.cmd_id              = 2, \
.cmd_name            = "sys_write", \
.typ                 = CMD_TYP_AKTION, \
.status              = CMD_STATUS_AKTIV, \
.immutable           = 1, \
.parameter_anzahl    = 1, \
.standard_ziel       = ADRESSE_NULL, \
.ausloesende_aktion  = NULL, \
.prioritaet          = 90, \
.max_ausfuehrungszeit = 50, \
.ersteller_node      = 0, \
.erstellungs_zeit    = 0, \
.signatur_hash       = 0, \
.next_cmd            = NULL, \
.prev_cmd            = NULL \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool cmd_ist_aktiv(const mcs_cmd_eintrag_t* cmd) {
    return cmd->status == CMD_STATUS_AKTIV;
}

static inline bool cmd_ist_immutable(const mcs_cmd_eintrag_t* cmd) {
    return cmd->immutable == 1;
}

static inline bool cmd_hat_parameter(const mcs_cmd_eintrag_t* cmd) {
    return cmd->parameter_anzahl > 0;
}

static inline bool register_schreibgeschuetzt(const mcs_cmd_register_t* reg) {
    return reg->schreibgeschuetzt == 1;
}

// Weitere Funktionen kommen später:
// cmd_ausfuehren(), cmd_registrieren(), cmd_suchen(), cmd_validieren(), register_initialisieren() ...

#endif // MCS_15_MCS_CMD_REGISTER_H
