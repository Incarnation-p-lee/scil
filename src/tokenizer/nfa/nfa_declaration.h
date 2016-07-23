// Automaticaly generated by script/generate_declaration.pl.

#ifndef NFA_DECLARATION_H
#define NFA_DECLARATION_H


bool nfa_engine_pattern_match_p(s_nfa_t *nfa, char *pn);
s_nfa_t * nfa_engine_create(char *re);
static inline bool nfa_char_alnum_underline_p(char c);
static inline bool nfa_char_alpha_underline_p(char c);
static inline bool nfa_char_binary_opt_p(char c);
static inline bool nfa_char_bracket_left_p(char *c);
static inline bool nfa_char_bracket_p(char c);
static inline bool nfa_char_unary_opt_p(char c);
static inline bool nfa_engine_graph_dfs_reached_p(s_fa_status_t *start, s_fa_status_t *terminal, s_open_addressing_hash_t *hash);
static inline bool nfa_engine_graph_legal_p(s_nfa_t *nfa);
static inline bool nfa_engine_pattern_match_ip(s_nfa_t *nfa, char *pn);
static inline bool nfa_engine_re_and_needed_p(char last, char c);
static inline bool nfa_engine_reverse_polish_legal_p(char *rp);
static inline bool nfa_engine_stack_top_p(s_array_stack_t *stack, char expected);
static inline bool nfa_engine_structure_legal_p(s_nfa_t *nfa);
static inline bool nfa_status_structure_legal_p(s_fa_status_t *status);
static inline s_fa_status_t * nfa_status_create(void);
static inline s_nfa_edge_map_t * nfa_edge_map_create(char c, s_nfa_t *nfa);
static inline s_nfa_t * nfa_create(void);
static inline s_nfa_t * nfa_edge_map_nfa_obtain(s_nfa_edge_map_t *map);
static inline s_nfa_t * nfa_engine_create_i(char *rp);
static inline s_nfa_t * nfa_subset_rule_basic(char c);
static inline uint32 nfa_label_obtain(void);
static inline void nfa_edge_map_destroy(s_nfa_edge_map_t *map);
static inline void nfa_engine_re_complete(char *pre, uint32 size, char *re);
static inline void nfa_engine_re_to_rp(char *rp, uint32 size, char *re);
static inline void nfa_engine_re_to_rp_final(char *re, uint32 size, s_array_stack_t *stack);
static inline void nfa_engine_re_to_rp_top_priority_0(s_array_stack_t *stack, char *c);
static inline void nfa_engine_re_to_rp_top_priority_1(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void nfa_engine_re_to_rp_top_priority_2(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void nfa_engine_re_to_rp_top_priority_3(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void nfa_label_cleanup(void);
static inline void nfa_simplify(s_nfa_t *nfa);
static inline void nfa_status_destroy_dfs(s_fa_status_t *status, s_open_addressing_hash_t *hash);
static inline void nfa_status_edge_chain(s_fa_status_t *status, char c, s_fa_status_t *next);
static inline void nfa_status_terminal_merge(s_fa_status_t *t, s_fa_status_t *m);
static inline void nfa_subset_rule_induction_and(s_nfa_t *s, s_nfa_t *t);
static inline void nfa_subset_rule_induction_binary(s_array_stack_t *stack,  e_nfa_subset_opt_t opt);
static inline void nfa_subset_rule_induction_or(s_nfa_t *s, s_nfa_t *t);
static inline void nfa_subset_rule_induction_plus(s_nfa_t *s);
static inline void nfa_subset_rule_induction_question(s_nfa_t *s);
static inline void nfa_subset_rule_induction_star(s_nfa_t *s);
static inline void nfa_subset_rule_induction_unary(s_array_stack_t *stack, e_nfa_subset_opt_t opt);
void nfa_engine_destroy(s_nfa_t *nfa);

#endif
