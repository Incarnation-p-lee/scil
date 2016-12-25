// Automaticaly generated by script/generate_external_declaration.pl.

#ifndef HAVE_DEFINED_EXTERNAL_H_tokenizer_
#define HAVE_DEFINED_EXTERNAL_H_tokenizer_

extern bool tk_char_comment_p(char *buf, e_tkz_lang_type_t tkz_type);
extern bool tk_char_double_quote_p(char c);
extern bool tk_char_multiple_comment_head_p(char *buf, e_tkz_lang_type_t tkz_type);
extern bool tk_char_multiple_comment_tail_p(char *buf, e_tkz_lang_type_t tkz_type);
extern bool tk_char_single_comment_p(char *buf, e_tkz_lang_type_t tkz_type);
extern bool tk_structure_illegal_p(s_tk_t *token);
extern bool tk_structure_legal_p(s_tk_t *token);
extern char tk_char_single_comment_end(e_tkz_lang_type_t tkz_type);
extern s_tk_t * tk_list_node_next(s_tk_t *token);
extern s_tk_t * tk_list_node_previous(s_tk_t *token);
extern s_tkz_file_t * tkz_file_create(char *fname);
extern s_trie_tree_t * tk_lang_c_keyword_trie_create(void);
extern uint32 tk_lang_c_cnst_match(s_nfa_t *nfa, s_tk_t *tk_head, char *buf);
extern uint32 tk_lang_c_idtr_match(s_nfa_t *nfa, s_trie_tree_t *keyword_trie, s_tk_t *tk_head, char *buf);
extern uint32 tk_lang_c_optr_match(s_nfa_t *nfa, s_tk_t *tk_head, char *buf);
extern uint32 tk_lang_c_pctt_match(s_nfa_t *nfa, s_tk_t *tk_head, char *buf);
extern void tk_lang_c_keyword_trie_destroy(s_trie_tree_t **keyword_trie);
extern void tk_list_destroy(s_tk_t *tk_list);
extern void tk_log_print(s_tk_t *token);
extern void tkz_file_destroy(s_tkz_file_t *tkz_file);
extern void tkz_file_tk_process(s_tkz_file_t *tkz_file);
extern void tkz_lang_cache_cleanup(void);

#endif

