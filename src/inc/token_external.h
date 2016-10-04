// Automaticaly generated by script/generate_external_declaration.pl.

#ifndef HAVE_DEFINED_EXTERNAL_H_token_
#define HAVE_DEFINED_EXTERNAL_H_token_

extern bool token_char_double_quote_p(char c);
extern bool token_char_multiple_comment_head_p(char *buf, e_tokenizer_language_type_t tkz_type);
extern bool token_char_multiple_comment_tail_p(char *buf, e_tokenizer_language_type_t tkz_type);
extern bool token_char_single_comment_p(char *buf, e_tokenizer_language_type_t tkz_type);
extern bool token_structure_legal_p(s_token_t *token);
extern s_token_language_c_kywd_t * token_language_c_keyword_trie_create(void);
extern s_token_t * token_list_previous_node(s_token_t *token);
extern uint32 token_language_c_constant_match(s_nfa_t *nfa, s_token_t *token_head, char *buf);
extern uint32 token_language_c_identifier_match(s_nfa_t *nfa, s_token_t *token_head, char *buf);
extern uint32 token_language_c_operator_match(s_nfa_t *nfa, s_token_t *token_head, char *buf);
extern uint32 token_language_c_punctuation_match(s_nfa_t *nfa, s_token_t *token_head, char *buf);
extern void token_language_c_destroy(s_token_t *token_list);
extern void token_language_c_keyword_seek(s_token_language_c_kywd_t *keyword_trie, s_token_t *token);
extern void token_language_c_keyword_trie_destroy(s_token_language_c_kywd_t *keyword_trie);

#endif

