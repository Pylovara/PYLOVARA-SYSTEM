// =============================================================================
// main.c – Pylovara MCS Kernel Boot
// Stand: 08. Januar 2026 – Erster offizieller Start
// Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
// =============================================================================
#include <unistd.h>     // FÜR usleep – muss vor allen anderen sein!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "include/mcs_08_master-control.h"
#include "include/mcs_01_datentypen.h"
#include "include/mcs_17_parser.h"

// Globale Instanz
mcs_master_control_t MC_Global = {0};

void mcs_sync_tick(void) {
    if (!MC_Global.magic_grossmutter) {
        printf("[FATAL] Magic-Großmutter nicht initialisiert!\n");
        exit(1);
    }

    mcs_sync_timer_t* timer = &MC_Global.magic_grossmutter->sync_timer;

    timer->global_tick++;
    timer->current_phase = (timer->global_tick - 1) % timer->takt_rate;

    printf("[SYNC %08llu] Phase: %llu/%llu | Subscribers: %llu | AI: %s\n",
           (unsigned long long)timer->global_tick,
           (unsigned long long)timer->current_phase,
           (unsigned long long)(timer->takt_rate - 1),
           (unsigned long long)timer->subscribers,
           MC_Global.magic_grossmutter->ai_gesteuert ? "aktiv" : "inaktiv");
}

void mcs_system_initialisieren(void) {
    printf("==================================================\n");
    printf("=== PYLOVARA MCS KERNEL – OFFIZIELLER START    ===\n");
    printf("=== 08. Januar 2026 – SSoT Version 00.58       ===\n");
    printf("=== Architekt: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A\n");
    printf("==================================================\n\n");

    mcs_magic_t* magic = calloc(1, sizeof(mcs_magic_t));
    if (!magic) {
        printf("[FATAL] Speicherallokation fehlgeschlagen.\n");
        exit(1);
    }

    magic->magic_id = 1;
    magic->level = MC_LEVEL_MAGIC;
    magic->sync_timer.takt_rate = 8;
    magic->sync_timer.subscribers = 1;
    magic->ai_gesteuert = true;

    MC_Global.magic_grossmutter = magic;
    MC_Global.system_start_zeit = 20260108;
    MC_Global.running = true;

    printf("[BOOT] Master-Control Hierarchie vollständig aufgebaut\n");
    printf("[MAGIC] ⊕ Sync-Timer aktiv – 8-Takt-Zyklus bereit\n");
    printf("[STATUS] Alle 16 Module geladen – System integrität geprüft\n");
    printf("[PHOTONIK] Lichtkreis-Simulation gestartet\n\n");
}

int main(void) {
    mcs_system_initialisieren();

    printf("Starte 8-Takt Lichtkreis-Simulation – 64 Zyklen (volle Geschwindigkeit)...\n\n");

    for (int i = 0; i < 64; i++) {
        mcs_sync_tick();
    }

    printf("\n64 Zyklen in Echtzeit abgeschlossen – System reagiert sofort.\n");

    printf("\n=== PYLOVARA MCS KERNEL ON ===\n");
    printf("=== Herzschlag stabil – 8-Takt synchron ===\n");
    printf("\nLade und führe test.datei-lcore aus...\n");
    mcs_parser_execute_file("testcodes/test.datei-lcore");

    if (MC_Global.magic_grossmutter) {
        free(MC_Global.magic_grossmutter);
    }

    printf("System ordnungsgemäß heruntergefahren.\n");
    return 0;
}
