// Automaticaly generated by script/generate_declaration.pl.

#ifndef NFA_DECLARATION_H
#define NFA_DECLARATION_H


bool nfa_engine_pattern_match_p(s_nfa_t *nfa, char *pn);
s_nfa_t * nfa_engine_create(char *regular);
static inline bool nfa_engine_graph_dfs_reached_p(s_nfa_t *nfa, s_open_addressing_hash_t *hash, s_fa_status_t *status);
static inline bool nfa_engine_graph_legal_p(s_nfa_t *nfa);
static inline bool nfa_engine_pattern_match_ip(s_nfa_t *nfa, char *pn);
static inline bool nfa_engine_structure_legal_p(s_nfa_t *nfa);
static inline bool nfa_engine_terminal_reached_p(s_array_queue_t *master);
static inline bool nfa_status_structure_legal_p(s_fa_status_t *status);
static inline bool nfa_status_terminal_p(s_fa_status_t *status);
static inline s_fa_edge_t * nfa_edge_next(s_fa_edge_t *edge);
static inline s_fa_status_t * nfa_status_create(void);
static inline s_nfa_edge_map_t * nfa_edge_map_create(char c);
static inline s_nfa_t * nfa_edge_map_nfa_obtain(s_nfa_edge_map_t *map);
static inline s_nfa_t * nfa_engine_create_i(char *polish);
static inline s_nfa_t * nfa_subset_rule_basic(char c);
static inline uint32 nfa_engine_token_match_i(s_nfa_t *nfa, char *pn);
static inline uint32 nfa_label_obtain(void);
static inline void nfa_edge_map_destroy(s_nfa_edge_map_t *map);
static inline void nfa_engine_array_queue_swap(s_array_queue_t **a, s_array_queue_t **b);
static inline void nfa_engine_create_operator(s_array_stack_t *stack, char c);
static inline void nfa_engine_destroy_final(s_nfa_t *nfa);
static inline void nfa_engine_destroy_i(s_nfa_t *nfa);
static inline void nfa_engine_destroy_print(s_nfa_t *nfa);
static inline void nfa_engine_graph_dfs_print(s_fa_status_t *status, s_open_addressing_hash_t *hash);
static inline void nfa_engine_graph_print(s_nfa_t *nfa);
static inline void nfa_engine_graph_print_status(s_fa_status_t *status);
static inline void nfa_engine_pattern_match_setup(s_array_queue_t *master, s_nfa_t *nfa);
static inline void nfa_engine_re_copy(s_nfa_t *nfa, char *re);
static inline void nfa_label_cleanup(void);
static inline void nfa_simplify(s_nfa_t *nfa);
static inline void nfa_status_destroy_dfs(s_fa_status_t *status, s_open_addressing_hash_t *hash);
static inline void nfa_status_edge_chain(s_fa_status_t *status, char c, s_fa_status_t *succ);
static inline void nfa_status_terminal_merge(s_fa_status_t *t, s_fa_status_t *m);
static inline void nfa_subset_rule_induction_and(s_nfa_t *s, s_nfa_t *t);
static inline void nfa_subset_rule_induction_binary(s_array_stack_t *stack,  e_regular_meta_opt_t opt);
static inline void nfa_subset_rule_induction_or(s_nfa_t *s, s_nfa_t *t);
static inline void nfa_subset_rule_induction_plus(s_nfa_t *s);
static inline void nfa_subset_rule_induction_question(s_nfa_t *s);
static inline void nfa_subset_rule_induction_star(s_nfa_t *s);
static inline void nfa_subset_rule_induction_unary(s_array_stack_t *stack, e_regular_meta_opt_t opt);
uint32 nfa_engine_token_match(s_nfa_t *nfa, char *pn);
void nfa_engine_destroy(s_nfa_t *nfa);

#endif
