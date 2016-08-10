#ifndef HAVE_DEFINED_REGULAR_H
#define HAVE_DEFINED_REGULAR_H

#define RE_EXPAND_MIN          8

enum regular_meta_opt {
    RE_M_OPT_BKT_L  = '(', // left bracket
    RE_M_OPT_BKT_R  = ')', // right bracket
    RE_M_OPT_AND    = '&',
    RE_M_OPT_OR     = '|',
    RE_M_OPT_STAR   = '*',
    RE_M_OPT_PLUS   = '+',
    RE_M_OPT_QUST   = '?',
    RE_M_OPT_MBKT_L = '[',
    RE_M_OPT_MBKT_R = ']',
    RE_M_OPT_CNNT   = '-',
};

typedef enum regular_meta_opt e_regular_meta_opt_t;

#endif

