#ifndef HAVE_DEFINED_NFA_DEFINE_H
#define HAVE_DEFINED_NFA_DEFINE_H

#if defined DEBUG
    #define NFA_ENGINE_STRUCTURE_LEGAL_P(nfa)   nfa_engine_structure_legal_p(nfa)
    #define NFA_ENGINE_STRUCTURE_ILLEGAL_P(nfa) nfa_engine_structure_illegal_p(nfa)
    #define NFA_ENGINE_GRAPH_LEGAL_P(nfa)       nfa_engine_graph_legal_p(nfa)
    #define NFA_ENGINE_GRAPH_ILLEGAL_P(nfa)     nfa_engine_graph_illegal_p(nfa)

#else
    #define NFA_ENGINE_STRUCTURE_LEGAL_P(nfa)   NON_NULL_PTR_P(nfa)
    #define NFA_ENGINE_STRUCTURE_ILLEGAL_P(nfa) NULL_PTR_P(nfa)
    #define NFA_ENGINE_GRAPH_LEGAL_P(nfa)       NON_NULL_PTR_P(nfa)
    #define NFA_ENGINE_GRAPH_ILLEGAL_P(nfa)     NULL_PTR_P(nfa)

#endif

#endif

