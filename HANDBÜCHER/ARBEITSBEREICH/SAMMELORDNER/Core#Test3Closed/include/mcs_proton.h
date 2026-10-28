// include/mcs_proton.h
#ifndef MCS_PROTON_H
#define MCS_PROTON_H

typedef struct {
    char* name;   // z.B. "Watt", "Temp", "Wait-s"
    char* value;  // z.B. "200", "25", "3"
} mcs_proton_field_t;

typedef struct {
    mcs_proton_field_t* fields;  // dynamisch
    int field_count;
} mcs_proton_t;

mcs_proton_t* mcs_parse_proton(const char* input);
void mcs_free_proton(mcs_proton_t* p);

#endif
