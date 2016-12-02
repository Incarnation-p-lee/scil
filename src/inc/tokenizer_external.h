// Automaticaly generated by script/generate_external_declaration.pl.

#ifndef HAVE_DEFINED_EXTERNAL_H_tokenizer_
#define HAVE_DEFINED_EXTERNAL_H_tokenizer_

extern bool token_char_comment_p(char *buf, e_tkz_lang_type_t tkz_type);
extern bool token_char_double_quote_p(char c);
extern bool token_char_multiple_comment_head_p(char *buf, e_tkz_lang_type_t tkz_type);
extern bool token_char_multiple_comment_tail_p(char *buf, e_tkz_lang_type_t tkz_type);
extern bool token_char_single_comment_p(char *buf, e_tkz_lang_type_t tkz_type);
extern bool token_structure_illegal_p(s_tk_t *token);
extern bool token_structure_legal_p(s_tk_t *token);
extern s_tk_t * token_list_node_next(s_tk_t *token);
extern s_tk_t * token_list_node_previous(s_tk_t *token);
extern s_trie_tree_t * token_language_c_keyword_trie_create(void);
extern sint32 main(int argc, char **argv);
extern uint32 token_language_c_constant_match(s_nfa_t *nfa, s_tk_t *token_head, char *buf);
extern uint32 token_language_c_identifier_match(s_nfa_t *nfa, s_trie_tree_t *keyword_trie, s_tk_t *token_head, char *buf);
extern uint32 token_language_c_operator_match(s_nfa_t *nfa, s_tk_t *token_head, char *buf);
extern uint32 token_language_c_punctuation_match(s_nfa_t *nfa, s_tk_t *token_head, char *buf);
extern void token_language_c_destroy(s_tk_t *token_list);
extern void token_language_c_keyword_trie_destroy(s_trie_tree_t **keyword_trie);
extern void token_print(s_tk_t *token, e_tkz_lang_type_t tkz_language_type);

#endif

