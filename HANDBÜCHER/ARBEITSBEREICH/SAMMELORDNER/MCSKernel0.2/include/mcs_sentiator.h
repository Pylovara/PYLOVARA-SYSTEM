#ifndef MCS_SENTIATOR_H
#define MCS_SENTIATOR_H

/**
 * @file mcs_sentiator.h
 * @brief MCS-Sentiator-Gattersteuerung (Boxi-Evaluation)
 * * Implementiert die Signalsteuerung für den Aktionskreislauf.
 * Jede Entscheidung basiert auf dem Inhalt einer Boxi (Protein-Feed).
 */

/**
 * Kern-Logik: Evaluiere eine Boxi (insb. Blankernenner) für den Sentiator.
 * @param typ 1 für ¶ (KANN), 2 für ¶¶ (NICHT), 6 für ſ (REINHEIT), etc.
 * @param boxi_wert Der Inhalt des nächsten Tokens (z.B. "(3)" oder "¨(3)¨").
 * @return 1 (Signalfluss erlaubt/Gatter offen), 0 (Sperrung).
 */
int mcs_sentiator_evaluiere_boxi(int typ, const char* boxi_wert);

/**
 * AIMS-Handshake-Validierung
 * Prüft die Protein-Signatur auf System-Integrität (þ10).
 */
int ALU_AIMS_HANDSHAKE(long protein_signatur);

#endif
