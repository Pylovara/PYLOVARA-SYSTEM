// include/mcs_cmd_register.h
#ifndef MCS_CMD_REGISTER_H
#define MCS_CMD_REGISTER_H

// Minimaler MCS-Befehl: cmd_name und Argument (String)
int mcs_cmd_execute(const char* cmd, const char* arg);
void mcs_register_init(void);  // optional: falls später dynamisch

#endif // MCS_CMD_REGISTER_H
