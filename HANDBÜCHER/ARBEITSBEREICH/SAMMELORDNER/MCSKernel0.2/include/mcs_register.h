#ifndef MCS_REGISTER_H
#define MCS_REGISTER_H

// --- Kern-Schnittstellen für das Gatter ---
void ALU_REINIGEN_B64(void);
void ALU_REGISTER_BINDEN(int reg_nr, long wert);
long mcs_register_extrahiere_protein(int reg_nr);

// --- Brücken-Funktionen (Wichtig für den Core) ---
void mcs_register_setze(int reg_nr, const char *wert_str);
char * mcs_register_hole(int reg_nr);
long mcs_register_hole_num(int reg_nr);

// --- System-Integrität ---
int ALU_AIMS_HANDSHAKE(long impuls);

#endif
