#ifndef NFA_DECLARATION_H
#define NFA_DECLARATION_H


s_nfa_t * nfa_engine_create(char *re);
static inline bool nfa_engine_alpha_underline_p(char c);
static inline bool nfa_engine_bracket_p(char c);
static inline bool nfa_engine_graph_legal_p(s_nfa_t *nfa);
static inline bool nfa_engine_stack_opt_top_p(s_array_stack_t *stack, char expected);
static inline bool nfa_engine_structure_legal_p(s_nfa_t *nfa);
static inline bool nfa_status_dfs_reached_p(s_fa_status_t *start, s_fa_status_t *terminal, s_open_addressing_hash_t *hash);
static inline bool nfa_status_legal_p(s_fa_status_t *status);
static inline s_fa_status_t * nfa_status_create(void);
static inline s_nfa_edge_map_t * nfa_edge_map_create(char c, s_nfa_t *nfa);
static inline s_nfa_t * nfa_create(void);
static inline s_nfa_t * nfa_edge_map_nfa_obtain(s_nfa_edge_map_t *map);
static inline s_nfa_t * nfa_engine_create_i(char *rp);
static inline s_nfa_t * nfa_subset_rule_basic(char c);
static inline uint32 nfa_label_obtain(void);
static inline void nfa_edge_map_destroy(s_nfa_edge_map_t *map);
static inline void nfa_engine_re_pre_process(char *pre, uint32 size, char *re);
static inline void nfa_engine_regular_to_reverse_polish(char *rp, uint32 size, char *re);
static inline void nfa_engine_regular_to_reverse_polish_final(char *re, uint32 size, s_array_stack_t *stack);
static inline void nfa_engine_regular_to_reverse_polish_top_bkt(s_array_stack_t *stack, char *c);
static inline void nfa_engine_regular_to_reverse_polish_top_opt(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void nfa_label_cleanup(void);
static inline void nfa_status_destroy_dfs(s_fa_status_t *status, s_open_addressing_hash_t *hash);
static inline void nfa_status_edge_chain(s_fa_status_t *status, char c, s_fa_status_t *next);
static inline void nfa_subset_rule_induction_and(s_nfa_t *s, s_nfa_t *t);
static inline void nfa_subset_rule_induction_binary(s_array_stack_t *stack,  e_nfa_subset_opt_t opt);
static inline void nfa_subset_rule_induction_or(s_nfa_t *s, s_nfa_t *t);
static inline void nfa_subset_rule_induction_plus(s_nfa_t *s);
static inline void nfa_subset_rule_induction_question(s_nfa_t *s);
static inline void nfa_subset_rule_induction_star(s_nfa_t *s);
static inline void nfa_subset_rule_induction_unary(s_array_stack_t *stack, e_nfa_subset_opt_t opt);
void nfa_engine_destroy(s_nfa_t *nfa);

#endif
