// =============================================================================
// include/mcs_15_mcs-cmd-register.h
// Pylovara MCS Kernel – Modul 15: MCS-CMD-Register
// Basierend auf Single Source of Truth (SSoT) Version 00.58 / 00.59
// Stand: 08. Januar 2026
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// Status: Zentrales Befehls- und Operationsregister
// =============================================================================

#ifndef MCS_15_MCS_CMD_REGISTER_H
#define MCS_15_MCS_CMD_REGISTER_H

#include "mcs_01_datentypen.h"           // B64
#include "mcs_02_transaktionsrahmen.h"   // Rahmen für CMD-Ausführung
#include "mcs_06_aktion.h"               // mcs_aktion_t – CMD löst Aktionen aus
#include "mcs_14_adressierung.h"         // mcs_adresse_t – Zieladressierung
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// MCS-CMD-Register – Grunddefinition
// =============================================================================

// Das MCS-CMD-Register ist das zentrale Befehlsarchiv des Pylovara-Systems.
// Es enthält alle verfügbaren Master-Control-System-Befehle (MCS-Commands),
// die innerhalb eines Transaktionsrahmens (¢! ... !¢) ausgeführt werden können.
// Kernbefehle sind immutable und bilden die unveränderliche Basis.
// Das Register ist erweiterbar durch dynamische Einträge (z. B. AI-generiert).
// Es dient als Single Source of Truth für alle ausführbaren Operationen.

typedef enum {
    CMD_TYP_SYSTEM      = 0,    // Kernbefehle (z. B. feed cache clear) – immutable
    CMD_TYP_AKTION      = 1,    // Direkte Aktionstrigger (SCHUB, WARP, etc.)
    CMD_TYP_CONTROL     = 2,    // Master-Control-Operationen (Node-Switch, Sync)
    CMD_TYP_ERWEITERUNG = 3,    // Dynamisch hinzugefügt (AI oder Plugin)
    CMD_TYP_DEBUG       = 4     // Nur in Entwicklung/Sandbox (Log, Dump)
} mcs_cmd_typ_t;

// Betriebszustand eines Befehls
typedef enum {
    CMD_STATUS_AKTIV         = 0,
    CMD_STATUS_INAKTIV       = 1,   // Deaktiviert durch Master-Control
    CMD_STATUS_GESPERRT      = 2,   // Sicherheitsblockade
    CMD_STATUS_EXPERIMENTELL = 3    // Nur in Baustelle-Modus verfügbar
} mcs_cmd_status_t;

// =============================================================================
// Struktur eines Befehlseintrags
// =============================================================================

typedef struct mcs_cmd_eintrag {
    // Identifikation
    B64                 cmd_id;             // Eindeutige numerische ID
    const char*         cmd_name;           // Menschlesbarer Name (z. B. "feed cache clear")
    mcs_cmd_typ_t       typ;

    // Status und Sicherheit
    mcs_cmd_status_t    status;
    B1                  immutable;          // 1 = Kernbefehl – darf nicht gelöscht/geändert werden

    // Parameter-Spezifikation
    B64                 parameter_anzahl;   // Erwartete Argumente (0 = parameterlos)

    // Ausführungsziel
    mcs_adresse_t       standard_ziel;      // Default-Zieladresse (z. B. globaler Cache)
    struct mcs_aktion*  ausloesende_aktion; // Direkte Aktion bei Ausführung (optional)

    // Scheduling und Sicherheit
    B64                 prioritaet;         // Standardpriorität bei Ausführung
    B64                 max_ausfuehrungszeit; // Obergrenze in Zyklen (Schutz vor Endlosschleifen)

    // Herkunft und Integrität
    B64                 ersteller_node;     // Welcher Master-Control-Node hat registriert
    B64                 erstellungs_zeit;
    B64                 signatur_hash;      // Kryptogenetische Prüfsumme

    // Verkettung im Register
    struct mcs_cmd_eintrag* next_cmd;
    struct mcs_cmd_eintrag* prev_cmd;

    // Zukunftssicherheit (z. B. AI-Gewichtung, Berechtigungs-Flag)
    B64                 reserved[6];

} mcs_cmd_eintrag_t;

// =============================================================================
// Globales MCS-CMD-Register (Singleton)
// =============================================================================

typedef struct mcs_cmd_register {
    struct mcs_cmd_eintrag* kopf;           // Erster Eintrag (Kernbefehle)
    struct mcs_cmd_eintrag* schwanz;        // Letzter Eintrag (schnelles Anhängen)
    B64                     eintrag_anzahl;
    B64                     max_eintraege;      // Sicherheitslimit
    B1                      schreibgeschuetzt;  // 1 = Nur Kernbefehle erlaubt (Produktion)
    B64                     version;            // Inkrementiert bei Änderungen
    B64                     reserved[8];
} mcs_cmd_register_t;

// =============================================================================
// Kernbefehle (Beispiele aus deinem SSOT)
// =============================================================================

#define MCS_CMD_FEED_CACHE_CLEAR { \
.cmd_id                = 1, \
.cmd_name              = "feed cache clear", \
.typ                   = CMD_TYP_SYSTEM, \
.status                = CMD_STATUS_AKTIV, \
.immutable             = 1, \
.parameter_anzahl      = 1, \
.standard_ziel         = MCS_ADRESSE_HIGH_SPEED(0xFFFF0000), \
.ausloesende_aktion    = NULL, \
.prioritaet            = 95, \
.max_ausfuehrungszeit  = 100, \
.ersteller_node        = 0, \
.erstellungs_zeit      = 0, \
.signatur_hash         = 0, \
.next_cmd              = NULL, \
.prev_cmd              = NULL, \
.reserved              = {0} \
}

#define MCS_CMD_SYS_WRITE { \
.cmd_id                = 2, \
.cmd_name              = "sys_write", \
.typ                   = CMD_TYP_AKTION, \
.status                = CMD_STATUS_AKTIV, \
.immutable             = 1, \
.parameter_anzahl      = 1, \
.standard_ziel         = MCS_ADRESSE_NULL, \
.ausloesende_aktion    = NULL, \
.prioritaet            = 90, \
.max_ausfuehrungszeit  = 50, \
.ersteller_node        = 0, \
.erstellungs_zeit      = 0, \
.signatur_hash         = 0, \
.next_cmd              = NULL, \
.prev_cmd              = NULL, \
.reserved              = {0} \
}

// =============================================================================
// Inline-Hilfsfunktionen
// =============================================================================

static inline bool mcs_cmd_ist_aktiv(const mcs_cmd_eintrag_t* cmd) {
    return cmd && cmd->status == CMD_STATUS_AKTIV;
}

static inline bool mcs_cmd_ist_immutable(const mcs_cmd_eintrag_t* cmd) {
    return cmd && cmd->immutable == 1;
}

static inline bool mcs_cmd_hat_parameter(const mcs_cmd_eintrag_t* cmd) {
    return cmd && cmd->parameter_anzahl > 0;
}

static inline bool mcs_cmd_register_schreibgeschuetzt(const mcs_cmd_register_t* reg) {
    return reg && reg->schreibgeschuetzt == 1;
}

// =============================================================================
// Ende Modul 15
// =============================================================================

#endif // MCS_15_MCS_CMD_REGISTER_H
