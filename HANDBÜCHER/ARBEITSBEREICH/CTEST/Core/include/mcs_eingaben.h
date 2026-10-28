// include/mcs_eingaben.h — MCS Eingabe-Container (v3.0, @kernel 07)
#ifndef MCS_EINGABEN_H
#define MCS_EINGABEN_H

#include <stddef.h>
#include "mcs_token.h"   // ← mcs_token_type_t
// Gemäß @kernel 07: 5 Eingabetypen — klar getrennt, sicher, modular
typedef enum {
    MCS_EINGABE_PRINT,       // "..." — Ausgabe (echo, notify-send, print)
    MCS_EINGABE_SYSTEM_CMD,  // '...' — System-Befehl (bash, sh, zsh, powershell)
    MCS_EINGABE_MCS_CMD,     // ’...’ — MCS-eigene Befehle (später: register/kernel 99)
    MCS_EINGABE_BLANKERNER,  // ¨...¨ — Transport-Container (Daten, nicht Code)
    MCS_EINGABE_NOTIZ        // ,... — Metadaten/Tagging (für Suche, Logs, AI)
} mcs_eingabe_typ_t;

// das hier MCS_EINGABE_SYSTEM_CMD ist eins zu eins das jeweilige
// system mit der jeweiligen shell oder direkt system verbindungen
// bei linux ist es meist bash zsh oder fisch etc , bei microsoft nt und powershell
// und bei mac os glaub darvin

// Container-Struktur — *kein Code-Ausführen hier*, nur Repräsentation
typedef struct {
    mcs_eingabe_typ_t typ;
    char* inhalt;          // z.B. "Hallo", "sudo pacman -Syyu", "$pgrep firefox"
    size_t laenge;
    int quote_start;       // Byte-Offset des öffnenden Quotes (z.B. ")
    int quote_end;         // Byte-Offset des schließenden Quotes
} mcs_eingabe_t;

// Public API — parsing & cleanup
mcs_eingabe_t* mcs_parse_eingabe_from_token(mcs_token_type_t quote_typ, const char* start, int len);
void mcs_free_eingabe(mcs_eingabe_t* e);

// Hilfsfunktionen — für Protein/Proton-Parser
int mcs_is_eingabe_quote(mcs_token_type_t tt);
mcs_eingabe_typ_t mcs_token_to_eingabe_typ(mcs_token_type_t tt);

#endif // MCS_EINGABEN_H
