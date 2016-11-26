// Automaticaly generated by script/generate_declaration.pl.

#ifndef NFA_DECLARATION_H
#define NFA_DECLARATION_H


bool nfa_engine_pattern_match_p(s_nfa_t *nfa, char *pn);
bool nfa_engine_structure_legal_p(s_nfa_t *nfa);
s_nfa_t * nfa_engine_create(char *regular);
static inline bool nfa_closure_match_dp_structure_legal_p(s_fa_match_dp_t *match_dp);
static inline bool nfa_closure_structure_legal_p(s_fa_closure_t *closure);
static inline bool nfa_engine_closure_match_p(s_nfa_t *nfa, s_fa_closure_t *closure);
static inline bool nfa_engine_graph_dfs_reached_p(s_nfa_t *nfa, s_open_addressing_hash_t *hash, s_fa_status_t *status);
static inline bool nfa_engine_graph_legal_p(s_nfa_t *nfa);
static inline bool nfa_status_structure_legal_p(s_fa_status_t *status);
static inline bool nfa_status_terminal_p(s_fa_status_t *status);
static inline s_fa_closure_t * nfa_closure_create(s_range_uint32_t *range);
static inline s_fa_edge_t * nfa_edge_next(s_fa_edge_t *edge);
static inline s_fa_match_dp_t * nfa_closure_match_dp_create(uint32 size);
static inline s_fa_status_t * nfa_status_create(void);
static inline s_nfa_edge_map_t * nfa_edge_map_create(char c);
static inline s_nfa_t * nfa_edge_map_nfa_obtain(s_nfa_edge_map_t *map);
static inline s_nfa_t * nfa_engine_create_i(char *polish);
static inline s_nfa_t * nfa_subset_rule_basic(char c);
static inline uint32 nfa_closure_match_dp_backtrack(s_fa_closure_t *closure);
static inline uint32 nfa_engine_pattern_match_i(s_nfa_t *nfa, char *pn);
static inline uint32 nfa_engine_pattern_match_ip(s_nfa_t *nfa, char *pn);
static inline uint32 nfa_label_obtain(void);
static inline void nfa_closure_char_seek(s_fa_closure_t *closure, char c);
static inline void nfa_closure_char_status_seek(s_fa_closure_t *closure, s_fa_status_t *status, char c);
static inline void nfa_closure_destroy(s_fa_closure_t **closure);
static inline void nfa_closure_init(s_nfa_t *nfa, s_fa_closure_t *closure);
static inline void nfa_closure_match_dp_append(s_nfa_t *nfa, s_fa_closure_t *closure);
static inline void nfa_closure_match_dp_destroy(s_fa_match_dp_t **match_dp);
static inline void nfa_closure_null_seek(s_fa_closure_t *closure);
static inline void nfa_closure_null_status_seek(s_fa_closure_t *closure, s_fa_status_t *status);
static inline void nfa_closure_print(s_fa_closure_t *closure, char *buf);
static inline void nfa_closure_queue_copy_to_path(s_fa_closure_t *closure);
static inline void nfa_closure_seek(s_fa_closure_t *closure, char c);
static inline void nfa_closure_title_print(char c, char *buf);
static inline void nfa_edge_map_destroy(s_nfa_edge_map_t *map);
static inline void nfa_engine_closure_match_print(s_nfa_t *nfa, s_fa_closure_t *closure);
static inline void nfa_engine_create_operator(s_array_stack_t *stack, char c);
static inline void nfa_engine_destroy_final(s_nfa_t *nfa);
static inline void nfa_engine_destroy_i(s_nfa_t *nfa);
static inline void nfa_engine_destroy_print(s_nfa_t *nfa);
static inline void nfa_engine_graph_dfs_print(s_fa_status_t *status, s_open_addressing_hash_t *hash);
static inline void nfa_engine_graph_print(s_nfa_t *nfa);
static inline void nfa_engine_graph_print_status(s_fa_status_t *status);
static inline void nfa_engine_patern_match_char_mov(s_fa_closure_t *closure, char c);
static inline void nfa_engine_re_copy(s_nfa_t *nfa, char *re);
static inline void nfa_engine_status_destroy_dfs(s_fa_status_t *status, s_open_addressing_hash_t *hash);
static inline void nfa_label_cleanup(void);
static inline void nfa_label_range_set(s_nfa_t *nfa);
static inline void nfa_simplify(s_nfa_t *nfa);
static inline void nfa_status_edge_chain(s_fa_status_t *status, char c, s_fa_status_t *succ);
static inline void nfa_status_terminal_merge(s_fa_status_t *t, s_fa_status_t *m);
static inline void nfa_subset_rule_induction_and(s_nfa_t *s, s_nfa_t *t);
static inline void nfa_subset_rule_induction_binary(s_array_stack_t *stack,  e_regular_wildcard_t opt);
static inline void nfa_subset_rule_induction_or(s_nfa_t *s, s_nfa_t *t);
static inline void nfa_subset_rule_induction_plus(s_nfa_t *s);
static inline void nfa_subset_rule_induction_question(s_nfa_t *s);
static inline void nfa_subset_rule_induction_star(s_nfa_t *s);
static inline void nfa_subset_rule_induction_unary(s_array_stack_t *stack, e_regular_wildcard_t opt);
uint32 nfa_engine_pattern_match(s_nfa_t *nfa, char *pn);
void nfa_engine_destroy(s_nfa_t *nfa);

#endif
