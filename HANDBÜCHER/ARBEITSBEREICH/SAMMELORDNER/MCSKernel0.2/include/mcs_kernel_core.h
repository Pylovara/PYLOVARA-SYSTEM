// OrdnerPfad: /Pylovara/MCSKernel/include/mcs_kernel_core.h
// Name: MCS-Zentral-Aktor-Header - STABILISIERT §7465

#ifndef MCS_KERNEL_CORE_H
#define MCS_KERNEL_CORE_H

#include "mcs_lexer.h"

// Funktions-Prototypen (Schnittstellen des Kernels)
void mcs_shell_ausfuehren(const char *cmd, char *output_buffer);
void mcs_verarbeite_argument(Token arg_token);
void mcs_inject_vars(char *dest, const char *src);
void mcs_kernel_takt(const char *quellcode);

#endif
