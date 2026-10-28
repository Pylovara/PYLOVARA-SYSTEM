// Lib/mcs_runner.c — v3.0 FINAL: mit Semantik-Integration, Parallel Run & Transport
// Korrigiert & stabilisiert — kompatibel mit aktuellem Stand (mit operator_count!)
// Lib/mcs_runner.c — v3.0 FINAL: mit Operator-Integration (kernel 09)
#include "mcs_feed.h"
#include "mcs_semantics.h"
#include "mcs_operatoren.h"   // VOR mcs_runner.h — volle Definition
#include "mcs_runner.h"  // NACH mcs_operatoren.h — damit MCS_ERR_SYSTEM bekannt ist
#include "mcs_cmd_register.h"   // ← DAMIT mcs_cmd_execute bekannt ist
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Hilfsfunktion: CPU-Last aus /proc/stat lesen (0–100)
static int get_cpu_load(void) {
    FILE* f = fopen("/proc/stat", "r");
    if (!f) return 50; // Fallback

    long user, nice, system, idle, iowait, irq, softirq;
    char buf[256];
    int ret = 0;

    if (fgets(buf, sizeof(buf), f) && sscanf(buf, "cpu %ld %ld %ld %ld %ld %ld %ld",
        &user, &nice, &system, &idle, &iowait, &irq, &softirq) == 7) {
        long total = user + nice + system + idle + iowait + irq + softirq;
    long work = user + nice + system;
    static long prev_total = 0, prev_work = 0;

    if (prev_total > 0) {
        long delta_total = total - prev_total;
        long delta_work = work - prev_work;
        if (delta_total > 0) {
            ret = (int)(100 * delta_work / delta_total);
        }
    }
    prev_total = total;
    prev_work = work;
        }
        fclose(f);
        return ret < 0 ? 0 : (ret > 100 ? 100 : ret);
}

// Hilfsfunktion: Dynamische Prioritätsanpassung basierend auf CPU-Last
static void adjust_for_cpu_load(void) {
    int cpu_load = get_cpu_load();
    if (cpu_load > 80) {
        nice(5);  // Leicht niedrigere Priorität bei hoher Last
    } else if (cpu_load < 20) {
        nice(-2); // Leicht höhere Priorität bei Leerlauf
    }
}

// 🔹 Hauptfunktion — stabil, fehleraggregierend, linear
int mcs_run_transaktion(mcs_transaktion_t* t) {
    if (!t || !t->is_valid) return MCS_ERR_SYNTAX;

    // 🔹 Dynamische Anpassung *vor* Ausführung
    adjust_for_cpu_load();

    // 🔹 Gesamtergebnis – Fehler sammeln, nicht vorzeitig abbrechen
    int res = MCS_OK;

    // 🔹 1. Operatoren-Auswertung (kernel 09) — SAUBER, OHNE t->then_branch
    if (t->operator_count > 0) {
        for (int i = 0; i < t->operator_count; i++) {
            mcs_operator_t* op = t->operators[i];
            if (!op) continue;

            mcs_truth_t eval = mcs_evaluate_operator(op);
            if (eval == MCS_ERROR) {
                res = MCS_ERR_SYNTAX;
                continue;
            }

            // °, <, > → Zweige ausführen (immer Protein[0] = THEN, Protein[1] = ELSE)
            if ((op->type == MCS_OP_WHEN_NOT ||
                op->type == MCS_OP_WHEN_LT ||
                op->type == MCS_OP_WHEN_GT) && t->protein_count >= 2) {

                if (eval == MCS_TRUE) {
                    // THEN-Zweig: erstes Protein
                    if (t->proteine[0]) {
                        int r = mcs_run_protein(t->proteine[0]);
                        if (r < 0) res = r;
                    }
                } else if (eval == MCS_FALSE) {
                    // ELSE-Zweig: zweites Protein
                    if (t->proteine[1]) {
                        int r = mcs_run_protein(t->proteine[1]);
                        if (r < 0) res = r;
                    }
                }
                }
                // ſ → sofort löschen (Lebenszyklus-Ende)
                else if (op->type == MCS_OP_DATA_RESIDUE && op->data.residue_id >= 0) {
                    mcs_feed_clear(op->data.residue_id);
                }
        }
    }

    // 🔹 2. Transaktions-Bedingung (z.B. ° ['$terminal']) — Fallback für alten Code
    if (t->wahrheit != WAHR_NONE && t->condition) {
        mcs_semantic_op_t op = mcs_wahrheit_to_semantic(t->wahrheit);
        mcs_truth_t cond = mcs_evaluate_condition(t->condition);

        if (op == SEMANTIC_WHEN_NOT) {  // °
            // Fallback: nur erstes Protein als THEN, zweites als ELSE nutzen
            if (t->protein_count >= 2) {
                mcs_protein_t* p_if = t->proteine[0];     // ¶ = THEN
                mcs_protein_t* p_else = t->proteine[1];   // ¶¶ = ELSE
                if (cond == MCS_FALSE && p_if && p_if->wahrheit == WAHR_IF && p_if->then_branch) {
                    int r = mcs_run_protein(p_if->then_branch);
                    if (r < 0) res = r;
                } else if (cond == MCS_TRUE && p_else && p_else->wahrheit == WAHR_ELSE && p_else->else_branch) {
                    int r = mcs_run_protein(p_else->else_branch);
                    if (r < 0) res = r;
                }
            }
        } else {
            // Altes Backup-Verhalten
            if (t->protein_count > 0 && t->proteine && t->proteine[0]) {
                mcs_protein_t* p0 = t->proteine[0];
                if (cond == MCS_FALSE && p0->then_branch) {
                    int r = mcs_run_protein(p0->then_branch);
                    if (r < 0) res = r;
                } else if (cond == MCS_TRUE && p0->else_branch) {
                    int r = mcs_run_protein(p0->else_branch);
                    if (r < 0) res = r;
                }
            }
        }
    }

    // 🔹 3. Alle Proteine sequenziell ausführen — mit Wahrheits-Dispatch
    for (int i = 0; i < t->protein_count; i++) {
        mcs_protein_t* p = (i < t->protein_count) ? t->proteine[i] : NULL;
        if (!p) continue;

        if (p->wahrheit != WAHR_NONE) {
            mcs_semantic_op_t op = mcs_wahrheit_to_semantic(p->wahrheit);
            switch (op) {
                case SEMANTIC_IF:
                    if (p->then_branch) {
                        int r = mcs_run_protein(p->then_branch);
                        if (r < 0) res = r;
                    }
                    break;
                case SEMANTIC_ELSE:
                    if (p->else_branch) {
                        int r = mcs_run_protein(p->else_branch);
                        if (r < 0) res = r;
                    }
                    break;
                case SEMANTIC_PARALLEL_TRANSPORT:  // ;;
                    if (p->action) {
                        int r = mcs_run_action(p->action);
                        if (r < 0) res = r;
                    }
                    break;
                case SEMANTIC_PARALLEL:  // ¶=
                    if (p->action) {
                        pid_t pid = fork();
                        if (pid == 0) {
                            mcs_run_action(p->action);
                            exit(0);
                        } else if (pid > 0) {
                            printf("[PARALLEL] Gestartet: PID %d\n", (int)pid);
                        } else {
                            fprintf(stderr, "[ERROR] fork() fehlgeschlagen\n");
                            res = MCS_ERR_SYSTEM;
                        }
                    }
                    break;
                default:
                    int r = mcs_run_protein(p);
                    if (r < 0) res = r;
            }
        } else {
            int r = mcs_run_protein(p);
            if (r < 0) res = r;
        }
    }

    // 🔹 Epilog: Fehlerbehandlung — ^ als error_handler
    if (res < 0 && t->error_handler) {
        mcs_run_protein(t->error_handler);
    }

    return res;
}

int mcs_run_protein(mcs_protein_t* p) {
    if (!p) return MCS_ERR_SYNTAX;

    if (p->wahrheit == WAHR_IF && p->then_branch) {
        return mcs_run_protein(p->then_branch);
    }
    if (p->wahrheit == WAHR_ELSE && p->else_branch) {
        return mcs_run_protein(p->else_branch);
    }
    if (p->action) {
        return mcs_run_action(p->action);
    }
    return MCS_OK;
}

int mcs_run_action(mcs_action_t* a) {
    if (!a || !a->cmd) {
        fprintf(stderr, "[RUN] Action NULL\n");
        return MCS_ERR_SYNTAX;
    }

    // 🔹 1. Wartezeit ausführen, falls ARG_TIME gesetzt
    if (a->arg.type == ARG_TIME && a->arg.value > 0) {
        double delay_sec = mcs_resolve_value(&a->arg, get_cpu_load());
        if (delay_sec > 0) {
            usleep((useconds_t)(delay_sec * 1e6)); // präziser als sleep()
        }
    }

    printf("[RUN] cmd='%s', blank='%s', feed.id=%d, arg.type=%d, arg.value=%.2f\n",
           a->cmd, a->blankernenner ?: "(null)", a->feed.id, a->arg.type, a->arg.value);

    // 🔹 2. Blankernenner → Feed setzen
    if (a->blankernenner && a->blankernenner[0] == '$' && a->feed.id > 0) {
        FILE* p = popen(a->blankernenner + 1, "r");
        if (p) {
            char buf[64] = {0};
            if (fgets(buf, sizeof(buf), p)) {
                buf[strcspn(buf, "\n")] = 0;
                mcs_feed_set(a->feed.id, buf, a->blankernenner);
                printf("[FEED] (%d) = '%s'\n", a->feed.id, buf);
            }
            pclose(p);
        }
    }
    // 🔹 3. MCS-CMD oder System-CMD ausführen
    if (a->cmd) {
        // ✅ MCS-CMD: 'feed cache' oder ’feed cache’ → aus Register
        // Prüfe auf Anführungszeichen durch Lexer-Typ — aber hier vereinfacht:
        if (a->cmd[0] == '\'') {  // ← nur ASCII-Apostroph: 'feed cache'
            char* cmd_clean = strdup(a->cmd + 1);
            if (cmd_clean) {
                // Entferne schließendes '
                char* last = strrchr(cmd_clean, '\'');
                if (last) *last = '\0';
                int err = mcs_cmd_execute(cmd_clean, a->blankernenner);
                free(cmd_clean);
                return (err == 0) ? MCS_OK : MCS_ERR_SYNTAX;
            }
        }

        // ❌ System-CMD: exec, run
        if ((strcmp(a->cmd, "exec") == 0 || strcmp(a->cmd, "run") == 0)) {
            const char* feed_val = a->feed.id > 0 ? mcs_feed_get(a->feed.id) : NULL;
            char cmd[256];
            if (a->blankernenner && feed_val) {
                snprintf(cmd, sizeof(cmd), "%s %s", a->cmd, feed_val);
            } else if (a->arg1) {
                snprintf(cmd, sizeof(cmd), "%s %s", a->cmd, a->arg1);
            } else {
                snprintf(cmd, sizeof(cmd), "%s", a->cmd);
            }
            printf("[EXEC] %s\n", cmd);
            return system(cmd);
        }
    }
}
