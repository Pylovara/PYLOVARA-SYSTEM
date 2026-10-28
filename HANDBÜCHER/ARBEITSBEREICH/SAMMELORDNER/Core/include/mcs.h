#ifndef MCS_H
#define MCS_H

// Public structs (opaque)
typedef struct mcs_ast mcs_ast_t;

// Public API
mcs_ast_t*  mcs_parse_file(const char* path);
mcs_ast_t*  mcs_parse_string(const char* src);
int         mcs_run_ast(mcs_ast_t* ast);
void        mcs_free_ast(mcs_ast_t* ast);

// Fehlercodes (öffentlich)
#define MCS_OK               0
#define MCS_ERR_SYNTAX      -1
#define MCS_ERR_IO          -2
#define MCS_ERR_SECURITY    -3

#endif
