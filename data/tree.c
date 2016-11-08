void
avl_tree_nice_set(struct avl_tree *tree, sint64 nice)
{
    tree->nice = nice;
}

sint64
avl_tree_nice(struct avl_tree *tree)
{
    return tree->nice;
}

struct avl_tree *
avl_tree_left(struct avl_tree *tree)
{
    return tree->left;
}

struct avl_tree *
avl_tree_right(struct avl_tree *tree)
{
    return tree->right;
}

void
avl_tree_height_set(struct avl_tree *tree, sint32 height)
{
    tree->height = height;
}

struct avl_tree *
avl_tree_create(void)
{
    struct avl_tree *tree;

    tree = memory_cache_allocate(sizeof(*tree));
    avl_tree_initial_internal(tree, 0);

    return tree;
}

static inline void
avl_tree_initial_internal(struct avl_tree *tree, sint64 nice)
{
    assert_exit(!NULL_PTR_P(tree));

    tree->height = 0;
    tree->nice = nice;
    tree->left = NULL;
    tree->right = NULL;
}

void
avl_tree_initial(struct avl_tree *tree, sint64 nice)
{
    if (avl_tree_structure_legal_p(tree)) {
        avl_tree_initial_internal(tree, nice);
    }
}

static inline bool
avl_tree_structure_legal_p(struct avl_tree *tree)
{
    if (NULL_PTR_P(tree)) {
        return false;
    } else if (tree->left == tree->right && NULL != tree->left) {
        return false;
    } else if (tree->left == tree || tree->right == tree) {
        return false;
    } else {
        return true;
    }
}

static inline void
avl_tree_node_destroy(struct avl_tree *node)
{
    assert_exit(avl_tree_structure_legal_p(node));

    memory_cache_free(node);
}

static inline void
avl_tree_destroy_internal(struct avl_tree *tree)
{
    /*
     * destroy node in post iterater order.
     * Warning: sometime if nested function call is too deepth,
     *     it may reach the default limitation of elf stack size, 8192KB.
     *     use ulimit -s unlimited or refine this function without nested.
     */
    if (tree) {
        avl_tree_destroy_internal(tree->left);
        tree->left = NULL;

        avl_tree_destroy_internal(tree->right);
        tree->right = NULL;

        avl_tree_node_destroy(tree);
    }
}

void
avl_tree_destroy(struct avl_tree **tree)
{
    if (!NULL_PTR_P(tree) && avl_tree_structure_legal_p(*tree)) {
        avl_tree_destroy_internal(*tree);
        *tree = NULL;
    }
}

static inline struct avl_tree *
avl_tree_find_internal(struct avl_tree *tree, sint64 nice)
{
    assert_exit(avl_tree_structure_legal_p(tree));

    while (tree) {
        if (nice < tree->nice) {
            tree = tree->left;
        } else if (nice > tree->nice) {
            tree = tree->right;
        } else {
            return tree;
        }
    }

    return NULL;
}

struct avl_tree *
avl_tree_find(struct avl_tree *tree, sint64 nice)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return NULL;
    } else {
        return avl_tree_find_internal(tree, nice);
    }
}

static inline struct avl_tree *
avl_tree_find_min_internal(struct avl_tree *tree)
{
    assert_exit(avl_tree_structure_legal_p(tree));

    while (tree->left) {
        tree = tree->left;
    }

    return tree;
}

struct avl_tree *
avl_tree_find_min(struct avl_tree *tree)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return NULL;
    } else {
        return avl_tree_find_min_internal(tree);
    }
}

static inline struct avl_tree *
avl_tree_find_max_internal(struct avl_tree *tree)
{
    assert_exit(avl_tree_structure_legal_p(tree));

    while (tree->right) {
        tree = tree->right;
    }

    return tree;
}

struct avl_tree *
avl_tree_find_max(struct avl_tree *tree)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return NULL;
    } else {
        return avl_tree_find_max_internal(tree);
    }
}

static inline bool
avl_tree_contains_p_internal(struct avl_tree *tree, struct avl_tree *node)
{
    bool retval;
    sint64 nice;
    struct avl_tree *left;
    struct avl_tree *right;

    assert_exit(avl_tree_structure_legal_p(tree));
    assert_exit(avl_tree_structure_legal_p(node));

    retval = false;
    nice = node->nice;

    while (tree) {
        if (node == tree) {
            return true;
        } else if (nice > tree->nice) {
            tree =  tree->right;
        } else if (nice < tree->nice) {
            tree = tree->left;
        } else {
            left = tree->left;
            right = tree->right;

            // Handle repeated nice
            if (left && nice == left->nice) {
                retval = avl_tree_contains_p_internal(left, node);
            }
            if (!retval && right && nice == right->nice) {
                retval = avl_tree_contains_p_internal(right, node);
            }

            return retval;
        }
    }

    return false;
}

bool
avl_tree_contains_p(struct avl_tree *tree, struct avl_tree *node)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return false;
    } else if (!avl_tree_structure_legal_p(node)) {
        return false;
    } else {
        return avl_tree_contains_p_internal(tree, node);
    }
}

static inline bool
avl_tree_node_balanced_p(struct avl_tree *node)
{
    assert_exit(avl_tree_structure_legal_p(node));

    return avl_tree_height_balanced_opt_p(node);
}

static inline bool
avl_tree_balanced_p_internal(struct avl_tree *tree)
{
    if (!tree) {
        return true;
    } else if (!avl_tree_node_balanced_p(tree)) {
        return false;
    } else if (!avl_tree_balanced_p_internal(tree->left)) {
        return false;
    } else if (!avl_tree_balanced_p_internal(tree->right)) {
        return false;
    } else {
        return true;
    }
}

bool
avl_tree_balanced_p(struct avl_tree *tree)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return true;
    } else {
        return avl_tree_balanced_p_internal(tree);
    }
}

static inline bool
avl_tree_doubly_child_p(struct avl_tree *tree)
{
    assert_exit(avl_tree_structure_legal_p(tree));

    if (tree->left && tree->right) {
        return true;
    } else {
        return false;
    }
}

sint32
avl_tree_height(struct avl_tree *tree)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return -1;
    } else {
        return avl_tree_height_opt(tree);
    }
}

static inline void
avl_tree_height_update(struct avl_tree *tree)
{
    sint32 left;
    sint32 right;

    assert_exit(avl_tree_structure_legal_p(tree));

    left = avl_tree_height_opt(tree->left);
    right = avl_tree_height_opt(tree->right);

    tree->height = MAX_S32(left, right) + 1;
}

/*
 *        k1                   k2
 *       /  \                 /  \
 *      k2   c               k3   k1
 *     /  \         ==>     /    /  \
 *    k3   b               a1   b    c
 *   /
 *  a1
 * perform the singe rotation, with left hand.
 */
static inline struct avl_tree *
avl_tree_single_rotate_left(struct avl_tree *node)
{
    struct avl_tree *k1;
    struct avl_tree *k2;

    assert_exit(avl_tree_structure_legal_p(node));

    k1 = node;
    k2 = k1->left;

    assert_exit(avl_tree_structure_legal_p(k2));
    assert_exit(avl_tree_structure_legal_p(k2->left));
    assert_exit(avl_tree_single_rotate_left_precondition_p(node));

    k1->left = k2->right;
    k2->right = k1;

    avl_tree_height_update(k1);
    avl_tree_height_update(k2);

    return k2;
}

/*
 *        k1                     k2
 *       /  \                   /  \
 *      c    k2                k1   k3
 *          /  \      ==>     /  \   \
 *         b    k3           c    b   a1
 *               \
 *                a1
 * perform the singe rotation, with right hand.
 */
static inline struct avl_tree *
avl_tree_single_rotate_right(struct avl_tree *node)
{
    struct avl_tree *k1;
    struct avl_tree *k2;

    assert_exit(avl_tree_structure_legal_p(node));

    k1 = node;
    k2 = k1->right;

    assert_exit(avl_tree_structure_legal_p(k2));
    assert_exit(avl_tree_structure_legal_p(k2->right));
    assert_exit(avl_tree_single_rotate_right_precondition_p(node));

    k1->right = k2->left;
    k2->left = k1;

    avl_tree_height_update(k1);
    avl_tree_height_update(k2);

    return k2;
}

/*
 *        k1                 k3
 *       /  \               /  \
 *      k2   b             k2   k1
 *     /  \       ==>     / \     \
 *    a    k3            a   a1    b
 *        /
 *       a1
 * perform the doubly rotation, with left hand.
 */
static inline struct avl_tree *
avl_tree_doubly_rotate_left(struct avl_tree *node)
{
    struct avl_tree *k1;
    struct avl_tree *k2;
    struct avl_tree *k3;

    assert_exit(avl_tree_structure_legal_p(node));

    k1 = node;
    k2 = k1->left;

    assert_exit(avl_tree_structure_legal_p(k2));
    assert_exit(avl_tree_structure_legal_p(k2->right));
    assert_exit(avl_tree_doubly_rotate_left_precondition_p(node));

    k3 = k2->right;
    k2->right = k3->left;
    k1->left = k3->right;

    k3->left = k2;
    k3->right = k1;

    avl_tree_height_update(k1);
    avl_tree_height_update(k2);
    avl_tree_height_update(k3);

    return k3;
}

/*
 *        k1                   k3
 *       /  \                 /  \
 *      a    k2              k1   k2
 *          / \     ==>     / \     \
 *         k3  b           a   a1    b
 *        /
 *       a1
 * perform the doubly rotation, with right hand.
 */
static inline struct avl_tree *
avl_tree_doubly_rotate_right(struct avl_tree *node)
{
    struct avl_tree *k1;
    struct avl_tree *k2;
    struct avl_tree *k3;

    assert_exit(avl_tree_structure_legal_p(node));

    k1 = node;
    k2 = k1->right;

    assert_exit(avl_tree_structure_legal_p(k2));
    assert_exit(avl_tree_structure_legal_p(k2->left));
    assert_exit(avl_tree_doubly_rotate_right_precondition_p(node));

    k3 = k2->left;
    k2->left = k3->right;
    k1->right = k3->left;

    k3->right = k2;
    k3->left = k1;

    avl_tree_height_update(k1);
    avl_tree_height_update(k2);
    avl_tree_height_update(k3);

    return k3;
}

static inline void
avl_tree_rotate_left(struct avl_tree **tree)
{
    sint32 l_ht;
    sint32 r_ht;
    struct avl_tree *left;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_structure_legal_p((*tree)->left));

    left = (*tree)->left;
    l_ht = avl_tree_height_opt(left->left);
    r_ht = avl_tree_height_opt(left->right);

    if (l_ht >= r_ht) {
        /*
         *      k1   <== *tree
         *     /  \
         *    k2   c
         *   /  \
         *  k3   b
         */
        *tree = avl_tree_single_rotate_left(*tree);
    } else {
        /*
         *     k1   <== *tree
         *    /  \
         *   k2   c
         *    \
         *     k3
        */
        *tree =  avl_tree_doubly_rotate_left(*tree);
    }
}

static inline void
avl_tree_rotate_right(struct avl_tree **tree)
{
    sint32 l_ht;
    sint32 r_ht;
    struct avl_tree *right;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_structure_legal_p((*tree)->right));

    right = (*tree)->right;
    l_ht = avl_tree_height_opt(right->left);
    r_ht = avl_tree_height_opt(right->right);

    if (r_ht >= l_ht) {
        /*
         * k1      <== *tree
         *   \
         *    k2
         *     \
         *      k3
         */
        *tree = avl_tree_single_rotate_right(*tree);
    } else {
        /*
         * k1
         *   \
         *    k2
         *   /
         * k3
         */
        *tree = avl_tree_doubly_rotate_right(*tree);
    }
}


static inline struct avl_tree *
avl_tree_insert_internal(struct avl_tree **tree, struct avl_tree *node)
{
    sint64 nice;
    struct avl_tree *avl;
    struct avl_tree *inserted;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_structure_legal_p(node));
    assert_exit(avl_tree_balanced_p(*tree));
    assert_exit(avl_tree_ordered_p(*tree));
    assert_exit(0 == node->height);

    avl = *tree;
    inserted = node;
    nice = node->nice;

    if (nice < avl->nice) {
        if (NULL == avl->left) {
            avl->left = node;
        } else {
            inserted = avl_tree_insert_internal(&avl->left, node);
            if (!avl_tree_node_balanced_p(avl)) {
                avl_tree_rotate_left(tree);
            }
        }
        avl_tree_height_update(avl);
    } else if (nice > avl->nice) {
        if (NULL == avl->right) {
            avl->right = node;
        } else {
            inserted = avl_tree_insert_internal(&avl->right, node);
            if (!avl_tree_node_balanced_p(avl)) {
                avl_tree_rotate_right(tree);
            }
        }
        avl_tree_height_update(avl);
    } else if (avl == node) {
        pr_log_info("Insert node exist, nothing will be done.\n");
        inserted = NULL;
    } else {
        if (NULL == avl->left) {
            avl->left = node;
        } else if (NULL == avl->right) {
            avl->right = node;
        } else {
            // Always inserted repeated value to left
            inserted = avl_tree_insert_internal(&avl->left, node);
            if (!avl_tree_node_balanced_p(avl)) {
                avl_tree_rotate_left(tree);
            }
        }
        avl_tree_height_update(avl);
    }

    assert_exit(avl_tree_ordered_p(*tree));
    assert_exit(avl_tree_balanced_p(*tree));

    return inserted;
}

struct avl_tree *
avl_tree_insert(struct avl_tree **tree, struct avl_tree *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!avl_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!avl_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        node->height = 0;
        return avl_tree_insert_internal(tree, node);
    }
}

static inline void
avl_tree_lt_doubly_child_strip(struct avl_tree **pre, struct avl_tree *node)
{
    assert_exit(!NULL_PTR_P(pre));
    assert_exit(avl_tree_structure_legal_p(*pre));
    assert_exit(avl_tree_structure_legal_p(node));
    assert_exit(*pre == node);

    if (NULL != node->left) {
        *pre = node->left;
    } else {
        *pre = node->right;
    }

    node->left = NULL;
    node->right = NULL;
}

static inline void
avl_tree_doubly_child_strip(struct avl_tree **node_pre)
{
    sint32 l_ht;
    sint32 r_ht;
    struct avl_tree *avl;

    assert_exit(!NULL_PTR_P(node_pre));
    assert_exit(avl_tree_structure_legal_p(*node_pre));

    avl = *node_pre;
    l_ht = avl_tree_height_opt(avl->left);
    r_ht = avl_tree_height_opt(avl->right);

    if (l_ht < r_ht) {
        avl_tree_doubly_child_strip_from_max(node_pre);
    } else {
        avl_tree_doubly_child_strip_from_min(node_pre);
    }
}

static inline void
avl_tree_swap_child(struct avl_tree *a, struct avl_tree *b)
{
    void *tmp;
    sint32 height;

    assert_exit(avl_tree_structure_legal_p(a));
    assert_exit(avl_tree_structure_legal_p(b));

    tmp = a->left;
    a->left = b->left;
    b->left = tmp;

    tmp = a->right;
    a->right = b->right;
    b->right = tmp;

    height = a->height;
    a->height = b->height;
    b->height = height;
}

static inline struct avl_tree **
avl_tree_find_ptr_to_max(struct avl_tree **tree)
{
    struct avl_tree *avl;
    struct avl_tree **max;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_doubly_child_p(*tree));

    max = tree;
    avl = *max;

    while (NULL != avl->right) {
        max = &avl->right;
        avl = *max;
    }

    return max;
}

static inline struct avl_tree **
avl_tree_find_ptr_to_min(struct avl_tree **tree)
{
    struct avl_tree *avl;
    struct avl_tree **min;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_doubly_child_p(*tree));

    min = tree;
    avl = *min;

    while (NULL != avl->left) {
        min = &avl->left;
        avl = *min;
    }

    return min;
}

static inline void
avl_tree_doubly_child_strip_from_max(struct avl_tree **node_pre)
{
    void *tmp;
    struct avl_tree *avl;
    struct avl_tree *max;
    struct avl_tree **max_pre;

    assert_exit(!NULL_PTR_P(node_pre));
    assert_exit(avl_tree_structure_legal_p(*node_pre));
    assert_exit(avl_tree_doubly_child_p(*node_pre));

    avl = *node_pre;

    if (!avl->left->right) {
         *node_pre = avl->left;
         tmp = avl->left->left;
         avl->left->right = avl->right;
         avl->left->left = avl;

         avl->left = tmp;
         avl->right = NULL;
    } else {
         max_pre = avl_tree_find_ptr_to_max(&avl->left);
         max = *max_pre;

         avl_tree_swap_child(avl, max);
         *max_pre = avl;
         *node_pre = max;
    }

    avl_tree_remove_internal(&max->left, avl);
}

static inline void
avl_tree_doubly_child_strip_from_min(struct avl_tree **node_pre)
{
    void *tmp;
    struct avl_tree *avl;
    struct avl_tree *min;
    struct avl_tree **min_pre;

    assert_exit(!NULL_PTR_P(node_pre));
    assert_exit(avl_tree_structure_legal_p(*node_pre));
    assert_exit(avl_tree_doubly_child_p(*node_pre));

    avl = *node_pre;

    if (!avl->right->left) {
         *node_pre = avl->right;
         tmp = avl->right->right;
         avl->right->left = avl->left;
         avl->right->right = avl;

         avl->right = tmp;
         avl->left = NULL;
    } else {
         min_pre = avl_tree_find_ptr_to_min(&avl->right);
         min = *min_pre;

         avl_tree_swap_child(avl, min);
         *min_pre = avl;
         *node_pre = min;
    }

    avl_tree_remove_internal(&min->right, avl);
}

static inline struct avl_tree *
avl_tree_remove_internal(struct avl_tree **tree, struct avl_tree *node)
{
    sint64 nice;
    struct avl_tree *avl;
    struct avl_tree *removed;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_balanced_p(*tree));
    assert_exit(avl_tree_ordered_p(*tree));
    assert_exit(avl_tree_structure_legal_p(node));

    avl = *tree;
    removed = NULL;
    nice = avl_tree_nice(node);

    if (nice < avl->nice) {
        if (NULL == avl->left) {
            pr_log_warn("Failed to find the node in given tree.\n");
            return NULL;
        } else {
            removed = avl_tree_remove_internal(&avl->left, node);
            if (!avl_tree_node_balanced_p(avl)) {
                avl_tree_rotate_right(tree);
            }
            avl_tree_height_update(avl);
        }
    } else if (nice > avl->nice) {
        if (NULL == avl->right) {
            pr_log_warn("Failed to find the node in given tree.\n");
            return NULL;
        } else {
            removed = avl_tree_remove_internal(&avl->right, node);
            if (!avl_tree_node_balanced_p(avl)) {
                 avl_tree_rotate_left(tree);
            }
            avl_tree_height_update(avl);
        }
    } else if (node != avl) {
        if (avl->left && nice == avl->left->nice) {
            removed = avl_tree_remove_internal(&avl->left, node);
            avl_tree_height_update(avl);
        }
        if (!removed && avl->right && nice == avl->right->nice) {
            removed = avl_tree_remove_internal(&avl->right, node);
            avl_tree_height_update(avl);
        }
        if (NULL == removed) {
            pr_log_warn("Failed to find the node in given tree.\n");
        }
    } else {
        removed = node;
        if (avl_tree_doubly_child_p(node)) {
            avl_tree_doubly_child_strip(tree);
        } else {
            avl_tree_lt_doubly_child_strip(tree, node);
        }
        avl_tree_height_update(avl);
    }

    assert_exit(avl_tree_balanced_p(*tree));
    assert_exit(avl_tree_ordered_p(*tree));

    return removed;
}

struct avl_tree *
avl_tree_remove(struct avl_tree **tree, struct avl_tree *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!avl_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!avl_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return avl_tree_remove_internal(tree, node);
    }
}

static inline void
avl_tree_iterate_internal(struct avl_tree *tree,
    void (*handle)(void *), enum ITER_ORDER order)
{
    assert_exit(LEGAL_ORDER_P(order));
    assert_exit(!NULL_PTR_P(handle));

    if (tree) {
        if (ORDER_PRE == order) {
            handle(tree);
        }

        avl_tree_iterate_internal(tree->left, handle, order);

        if (ORDER_IN == order) {
            handle(tree);
        }

        avl_tree_iterate_internal(tree->right, handle, order);

        if (ORDER_POST == order) {
            handle(tree);
        }
    }
}

void
avl_tree_iterate(struct avl_tree *tree,
    void (*handle)(void *), enum ITER_ORDER order)
{
    if (NULL_PTR_P(handle)) {
        return;
    } else if (!avl_tree_structure_legal_p(tree)) {
        return;
    } else if (!LEGAL_ORDER_P(order)) {
        pr_log_warn("Invalid value of enum ITER_ORDER.\n");
        return;
    } else {
        avl_tree_iterate_internal(tree, handle, order);
    }
}

static inline bool
avl_tree_ordered_p(struct avl_tree *tree)
{
    struct avl_tree *left;
    struct avl_tree *right;

    if (!tree) {
        return true;
    } else {
        left = tree->left;
        right = tree->right;

        if (left && tree->nice < left->nice) {
            return false;
        } else if (right && tree->nice > right->nice) {
            return false;
        } else {
            if (!avl_tree_ordered_p(left)) {
                return false;
            } else {
                return avl_tree_ordered_p(right);
            }
        }
    }
}

static inline sint32
avl_tree_height_calculate(struct avl_tree *tree)
{
    sint32 left;
    sint32 right;

    if (!tree) {
        return -1;
    } else {
        left = avl_tree_height_calculate(tree->left);
        right = avl_tree_height_calculate(tree->right);

        return MAX_S32(left, right) + 1;
    }
}

static inline bool
avl_tree_height_sync_with_calculated_p(struct avl_tree *tree,
    sint32 left, sint32 right)
{
    assert_exit(avl_tree_structure_legal_p(tree));

    if (left == avl_tree_height_calculate(tree->left)
        && right == avl_tree_height_calculate(tree->right)) {
        return true;
    } else {
        pr_log_debug("Illegal height of avl tree with calculated.\n");
        return false;
    }
}

static inline bool
avl_tree_single_rotate_left_precondition_p(struct avl_tree *node)
{
    sint32 left;
    sint32 right;
    struct avl_tree *k1;
    struct avl_tree *k2;

    assert_exit(avl_tree_structure_legal_p(node));
    assert_exit(avl_tree_structure_legal_p(node->left));

    k1 = node;
    k2 = k1->left;

    left = avl_tree_height_internal(k2->left);
    right = avl_tree_height_internal(k2->right);

    if (left < right) {
        return false;
    } else {
        return true;
    }
}

static inline bool
avl_tree_single_rotate_right_precondition_p(struct avl_tree *node)
{
    sint32 left;
    sint32 right;
    struct avl_tree *k1;
    struct avl_tree *k2;

    assert_exit(avl_tree_structure_legal_p(node));
    assert_exit(avl_tree_structure_legal_p(node->right));

    k1 = node;
    k2 = k1->right;

    left = avl_tree_height_internal(k2->left);
    right = avl_tree_height_internal(k2->right);

    if (right < left) {
        return false;
    } else {
        return true;
    }
}

static inline bool
avl_tree_doubly_rotate_left_precondition_p(struct avl_tree *node)
{
    sint32 left;
    sint32 right;
    struct avl_tree *k1;
    struct avl_tree *k2;

    assert_exit(avl_tree_structure_legal_p(node));
    assert_exit(avl_tree_structure_legal_p(node->left));

    k1 = node;
    k2 = k1->left;

    left = avl_tree_height_internal(k2->left);
    right = avl_tree_height_internal(k2->right);

    if (left < right) {
        return true;
    } else {
        return false;
    }
}

static inline bool
avl_tree_doubly_rotate_right_precondition_p(struct avl_tree *node)
{
    sint32 left;
    sint32 right;
    struct avl_tree *k1;
    struct avl_tree *k2;

    assert_exit(avl_tree_structure_legal_p(node));
    assert_exit(avl_tree_structure_legal_p(node->right));

    k1 = node;
    k2 = k1->right;

    left = avl_tree_height_internal(k2->left);
    right = avl_tree_height_internal(k2->right);

    if (left > right) {
        return true;
    } else {
        return false;
    }
}

static inline bool
avl_tree_left_optimize_validity_p(struct avl_tree *node,
    void *expected)
{
    void *computed;

    assert_exit(avl_tree_structure_legal_p(node));

    computed = node->left;

    if (computed == expected) {
        return true;
    } else {
        return false;
    }
}

static inline bool
avl_tree_right_optimize_validity_p(struct avl_tree *node,
    void *expected)
{
    void *computed;

    assert_exit(avl_tree_structure_legal_p(node));

    computed = node->right;

    if (computed == expected) {
        return true;
    } else {
        return false;
    }
}

static inline sint32
avl_tree_height_internal(struct avl_tree *tree)
{
    if (!tree) {
        return -1;
    } else {
        return tree->height;
    }
}

static inline bool
avl_tree_height_opt_validity_p(struct avl_tree *tree,
    sint32 expected)
{
    sint32 computed;

    computed = avl_tree_height_internal(tree);

    if (computed == expected) {
        return true;
    } else {
        return false;
    }
}

static inline bool
avl_tree_height_balanced_p(struct avl_tree *tree)
{
    sint32 left;
    sint32 right;

    assert_exit(avl_tree_structure_legal_p(tree));

    left = avl_tree_height_opt(tree->left);
    right = avl_tree_height_opt(tree->right);
    assert_exit(avl_tree_height_sync_with_calculated_p(tree, left, right));

    if (abs_sint32(left - right) > 1) {
        return false;
    } else {
        return true;
    }
}

static inline bool
avl_tree_balanced_optimize_validity_p(struct avl_tree *tree, bool expected)
{
    bool computed;

    assert_exit(avl_tree_structure_legal_p(tree));

    computed = avl_tree_height_balanced_p(tree);

    if (computed == expected) {
        return true;
    } else {
        return false;
    }
}

uint32
binary_indexed_tree_size(struct binary_indexed_tree *tree)
{
    return tree->size;
}

sint64
binary_indexed_tree_value(struct binary_indexed_tree *tree, uint32 number)
{
    return tree->data[number];
}

/*
 * Generate tree->data with DP.
 * For example: number 24 = 11000B
 *     24 = 11000B
 *        = 10111B
 *        + 10110B
 *        + 10100B
 * Iteration:
 * msk 1111110B
 * raw XXX1000B
 * b   XXX0111B
 * ...
 * e   XXX0100B
 */
struct binary_indexed_tree *
binary_indexed_tree_create(sint64 *data, uint32 size)
{
    uint32 i;
    uint32 base;
    uint32 mask;
    struct binary_indexed_tree *tree;

    if (NULL_PTR_P(data)) {
        return NULL;
    } else if (complain_zero_size_p(size)) {
        return NULL;
    } else {
        tree = memory_cache_allocate(sizeof(*tree));
        tree->size = size;
        tree->data = memory_cache_allocate(sizeof(*tree->data) * (size + 1));
        memset(tree->data, 0, sizeof(*tree->data) * (size + 1));

        i = 2;
        tree->data[1] = data[0];

        while (i <= size) {
            tree->data[i] = data[i - 1];
            if (0 == i % 2) {
                base = i - 1;
                mask = 0xfffffffe;

                while (0 == (i & ~mask)) {
                    tree->data[i] += tree->data[base];
                    base = base & mask;
                    mask = mask << 1;
                }
            }
            i++;
        }

        return tree;
    }
}

void
binary_indexed_tree_destroy(struct binary_indexed_tree **tree)
{
    struct binary_indexed_tree *tmp;

    if (NULL_PTR_P(tree)) {
        return;
    } else if (binary_indexed_tree_structure_legal_p(*tree)) {
        tmp = *tree;
        *tree = NULL;

        memory_cache_free(tmp->data);
        memory_cache_free(tmp);
    }
}

static inline bool
binary_indexed_tree_number_legal_p(struct binary_indexed_tree *tree, uint32 number)
{
    assert_exit(!NULL_PTR_P(tree));

    if (number == BIN_IDXED_NMBR_INVALID) {
        return false;
    } else if (number > tree->size) {
        return false;
    } else {
        return true;
    }
}

static inline bool
binary_indexed_tree_structure_legal_p(struct binary_indexed_tree *tree)
{
    if (NULL_PTR_P(tree)) {
        return false;
    } else if (NULL_PTR_P(tree->data)) {
        return false;
    } else if (complain_zero_size_p(tree->size)) {
        return false;
    } else {
        return true;
    }
}

static inline void
binary_indexed_tree_add_internal(struct binary_indexed_tree *tree,
    uint32 number, sint64 val)
{
    uint32 base;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(binary_indexed_tree_number_legal_p(tree, number));
    assert_exit(binary_indexed_tree_structure_legal_p(tree));

    base = ((number - 1) ^ number) & number;
    tree->data[number] += val;

    /*
     * Update the all the parent node of tree.
     * For example: number 10 = 1010B
     *     Should update from 
     *     1. 01010B
     *     2. 01100B = 1010B + 10B
     *     3. 10000B = 1100B + 100B ...
     *     to the limit of size. 
     */
    while (number + base <= tree->size) {
        number = number + base;
        tree->data[number] += val;
        base = base << 1;
    }
}


void
binary_indexed_tree_add(struct binary_indexed_tree *tree,
    uint32 number, sint64 val)
{

    if (!binary_indexed_tree_structure_legal_p(tree)) {
        return;
    } else if (!binary_indexed_tree_number_legal_p(tree, number)) {
        return;
    } else {
        binary_indexed_tree_add_internal(tree, number, val);
    }
}

void
binary_indexed_tree_sub(struct binary_indexed_tree *tree,
    uint32 number, sint64 val)
{

    if (!binary_indexed_tree_structure_legal_p(tree)) {
        return;
    } else if (!binary_indexed_tree_number_legal_p(tree, number)) {
        return;
    } else {
        binary_indexed_tree_add_internal(tree, number, -val);
    }
}

static inline sint64
binary_indexed_tree_sum_internal(struct binary_indexed_tree *tree,
    uint32 number)
{
    sint64 retval;
    uint32 base;

    assert_exit(binary_indexed_tree_structure_legal_p(tree));
    assert_exit(binary_indexed_tree_number_legal_p(tree, number));

    retval = 0;
    base = number;

    while (base) {
        retval += tree->data[base];
        base = base & (base - 1);
    }

    return retval;
}

sint64
binary_indexed_tree_sum(struct binary_indexed_tree *tree, uint32 number)
{
    if (!binary_indexed_tree_structure_legal_p(tree)) {
        return BIN_IDXED_SUM_INVALID;
    } else if (!binary_indexed_tree_number_legal_p(tree, number)) {
        return 0;
    } else {
        return binary_indexed_tree_sum_internal(tree, number);
    }
}

sint64
binary_indexed_tree_range_sum(struct binary_indexed_tree *tree,
    uint32 nmbr_s, uint32 nmbr_e)
{
    sint64 retval;
    sint64 tmp;

    if (!binary_indexed_tree_structure_legal_p(tree)) {
        return BIN_IDXED_SUM_INVALID;
    } else if (!binary_indexed_tree_number_legal_p(tree, nmbr_s)) {
        return BIN_IDXED_SUM_INVALID;
    } else if (!binary_indexed_tree_number_legal_p(tree, nmbr_e)) {
        return BIN_IDXED_SUM_INVALID;
    } else if (nmbr_s > nmbr_e) {
        pr_log_warn("Invalid start number and end number of range.\n");
        return BIN_IDXED_SUM_INVALID;
    } else {
        retval = binary_indexed_tree_sum_internal(tree, nmbr_e);
        if (nmbr_s == 1) {
            tmp = 0;
        } else {
            tmp = binary_indexed_tree_sum_internal(tree, nmbr_s - 1);
        }

        return retval - tmp;
    }
}

void
binary_search_tree_node_nice_set(struct binary_search_tree *tree, sint64 nice)
{
    tree->nice = nice;
}

sint64
binary_search_tree_nice(struct binary_search_tree *tree)
{
    return tree->nice;
}

struct binary_search_tree *
binary_search_tree_left(struct binary_search_tree *tree)
{
    return tree->left;
}

struct binary_search_tree *
binary_search_tree_right(struct binary_search_tree *tree)
{
    return tree->right;
}

void
binary_search_tree_left_set(struct binary_search_tree *tree,
    struct binary_search_tree *left)
{
    tree->left = left;
}

void
binary_search_tree_right_set(struct binary_search_tree *tree,
    struct binary_search_tree *right)
{
    tree->right = right;
}

struct binary_search_tree *
binary_search_tree_create(void)
{
    struct binary_search_tree *tree;

    tree = memory_cache_allocate(sizeof(*tree));
    binary_search_tree_initial_internal(tree, 0);

    return tree;
}

static inline void
binary_search_tree_initial_internal(struct binary_search_tree *tree,
    sint64 nice)
{
    assert_exit(!NULL_PTR_P(tree));

    tree->left = NULL;
    tree->right = NULL;
    tree->nice = nice;
}

void
binary_search_tree_initial(struct binary_search_tree *tree, sint64 nice)
{
    if (binary_search_tree_structure_legal_p(tree)) {
        binary_search_tree_initial_internal(tree, nice);
    }
}

static inline bool
binary_search_tree_structure_legal_p(struct binary_search_tree *tree)
{
    if (NULL_PTR_P(tree)) {
        return false;
    } else if (tree->left == tree->right && NULL != tree->left) {
        return false;
    } else if (tree->left == tree || tree->right == tree) {
        return false;
    } else {
        return true;
    }
}

static inline void
binary_search_tree_node_destroy(struct binary_search_tree *node)
{
    assert_exit(binary_search_tree_structure_legal_p(node));

    memory_cache_free(node);
}

static inline void
binary_search_tree_destroy_internal(struct binary_search_tree **tree)
{
    if (*tree && tree) {
        /*
         * destroy node in post iterater order.
         * Warning: sometime if nested function call is too deepth,
         *     it may reach the default limitation of elf stack size, 8192KB.
         *     use ulimit -s unlimited or refine this function without nested.
         */
        binary_search_tree_destroy_internal(&(*tree)->left);
        binary_search_tree_destroy_internal(&(*tree)->right);
        binary_search_tree_node_destroy(*tree);
        *tree = NULL;
    }
}

void
binary_search_tree_destroy(struct binary_search_tree **tree)
{
    if (NULL_PTR_P(tree)) {
        return;
    } else if (binary_search_tree_structure_legal_p(*tree)) {
        binary_search_tree_destroy_internal(tree);
    }
}

static inline struct binary_search_tree *
binary_search_tree_find_internal(struct binary_search_tree *tree,
    sint64 nice)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return NULL;
    } else {
        while (tree) {
            if (nice > binary_search_tree_nice(tree)) {
                tree = tree->right;
            } else if (nice < binary_search_tree_nice(tree)) {
                tree = tree->left;
            } else {
                return tree;
            }
        }

        return NULL;
    }
}

struct binary_search_tree *
binary_search_tree_find(struct binary_search_tree *tree, sint64 nice)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return NULL;
    } else {
        return binary_search_tree_find_internal(tree, nice);
    }
}

static inline struct binary_search_tree *
binary_search_tree_find_min_internal(struct binary_search_tree *tree)
{
    assert_exit(binary_search_tree_structure_legal_p(tree));

    while (tree->left) {
        tree = tree->left;
    }

    return tree;
}

struct binary_search_tree  *
binary_search_tree_find_min(struct binary_search_tree *tree)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return NULL;
    } else {
        return binary_search_tree_find_min_internal(tree);
    }
}

static inline struct binary_search_tree *
binary_search_tree_find_max_internal(struct binary_search_tree *tree)
{
    assert_exit(binary_search_tree_structure_legal_p(tree));

    while (tree->right) {
        tree = tree->right;
    }

    return tree;
}

struct binary_search_tree *
binary_search_tree_find_max(struct binary_search_tree *tree)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return NULL;
    } else {
        return binary_search_tree_find_max_internal(tree);
    }
}

static inline sint32
binary_search_tree_height_internal(struct binary_search_tree *tree)
{
    sint32 left;
    sint32 right;

    if (!tree) {
        return -1;
    } else {
        left = binary_search_tree_height_internal(tree->left);
        right = binary_search_tree_height_internal(tree->right);

        return MAX_S32(left, right) + 1;
    }
}

sint32
binary_search_tree_height(struct binary_search_tree *tree)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return -1;
    } else {
        return binary_search_tree_height_internal(tree);
    }
}

static inline bool
binary_search_tree_contains_p_internal(struct binary_search_tree *tree,
    struct binary_search_tree *node)
{
    bool retval;
    sint64 nice;
    struct binary_search_tree *left;
    struct binary_search_tree *right;

    assert_exit(binary_search_tree_structure_legal_p(tree));
    assert_exit(binary_search_tree_structure_legal_p(node));

    retval = false;
    nice = node->nice;

    while (tree) {
        if (node == tree) {
            return true;
        } else if (nice > tree->nice) {
            tree = tree->right;
        } else if (nice < tree->nice) {
            tree = tree->left;
        } else {
            left = tree->left;
            right = tree->right;

            // Handle repeated nice
            if (left && nice == left->nice) {
                retval = binary_search_tree_contains_p_internal(left, node);
            }
            if (!retval && right && nice == right->nice) {
                retval = binary_search_tree_contains_p_internal(right, node);
            }

            return retval;
        }
    }

    return false;
}

bool
binary_search_tree_contains_p(struct binary_search_tree *tree,
    struct binary_search_tree *node)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return false;
    } else if (!binary_search_tree_structure_legal_p(node)) {
        return false;
    } else {
        return binary_search_tree_contains_p_internal(tree, node);
    }
}

static inline struct binary_search_tree *
binary_search_tree_insert_internal(struct binary_search_tree **tree,
    struct binary_search_tree *node)
{
    sint64 nice;
    struct binary_search_tree **iter;
    struct binary_search_tree *binary;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(binary_search_tree_ordered_p(*tree));
    assert_exit(binary_search_tree_structure_legal_p(*tree));
    assert_exit(binary_search_tree_structure_legal_p(node));

    iter = tree;
    nice = node->nice;

    while (*iter) {
        if (nice > (*iter)->nice) {
            iter = &(*iter)->right;
            direct++;
        } else if (nice < (*iter)->nice) {
            iter = &(*iter)->left;
            direct--;
        } else if (node != *iter) {
            binary = *iter;

            if (direct > 0) { // If right is heavy, inserted to left.
                node->left = binary;
                node->right = binary->right;
                binary->right = NULL;
                *iter = node;
            } else {
                node->left = binary->left;
                node->right = binary;
                binary->left = NULL;
                *iter = node;
            }

            direct = 0;
            break;
        } else {
            pr_log_warn("Insert node exist, nothing will be done.\n");
            return NULL;
        }
    }

    if (!*iter) {
        *iter = node;
    }

    assert_exit(binary_search_tree_ordered_p(*tree));
    return node;
}

struct binary_search_tree *
binary_search_tree_insert(struct binary_search_tree **tree,
    struct binary_search_tree *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!binary_search_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!binary_search_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return binary_search_tree_insert_internal(tree, node);
    }
}

static inline bool
binary_search_tree_doubly_child_p(struct binary_search_tree *node)
{
    assert_exit(binary_search_tree_structure_legal_p(node));

    if (node->left && node->right) {
        return true;
    } else {
        return false;
    }
}

static inline struct binary_search_tree **
binary_search_tree_find_ptr_to_max(struct binary_search_tree **tree)
{
    struct binary_search_tree **max;
    struct binary_search_tree *node;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(binary_search_tree_structure_legal_p(*tree));

    max = tree;
    node = *max;

    while (NULL != node->right) {
        max = &node->right;
        node = *max;
    }

    return max;
}

static inline struct binary_search_tree **
binary_search_tree_find_ptr_to_min(struct binary_search_tree **tree)
{
    struct binary_search_tree **min;
    struct binary_search_tree *node;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(binary_search_tree_structure_legal_p(*tree));

    min = tree;
    node = *min;

    while (NULL != node->left) {
        min = &node->left;
        node = *min;
    }

    return min;
}

static inline void
binary_search_tree_swap_child(struct binary_search_tree *a,
    struct binary_search_tree *b)
{
    void *tmp;

    assert_exit(binary_search_tree_structure_legal_p(a));
    assert_exit(binary_search_tree_structure_legal_p(b));

    tmp = a->left;
    a->left = b->left;
    b->left = tmp;

    tmp = a->right;
    a->right = b->right;
    b->right = tmp;
}

static inline void
binary_search_tree_lt_doubly_child_strip(struct binary_search_tree **pre,
    struct binary_search_tree *node)
{
    assert_exit(!NULL_PTR_P(pre));
    assert_exit(binary_search_tree_structure_legal_p(node));
    assert_exit(binary_search_tree_structure_legal_p(*pre));
    assert_exit(!binary_search_tree_doubly_child_p(node));
    assert_exit(*pre == node);

    if (NULL != node->left) {
        *pre = node->left;
    } else {
        *pre = node->right;
    }

    node->left = NULL;
    node->right = NULL;
}

static inline void
binary_search_tree_child_strip_from_max(struct binary_search_tree **node_pre)
{
    struct binary_search_tree *binary;
    struct binary_search_tree *max;
    struct binary_search_tree **max_pre;

    assert_exit(!NULL_PTR_P(node_pre));
    assert_exit(binary_search_tree_structure_legal_p(*node_pre));
    assert_exit(binary_search_tree_doubly_child_p(*node_pre));

    binary = *node_pre;

    if (!binary->left->right) {
        // short cut here
        *node_pre = binary->left;
        binary->left->right = binary->right;
        binary->left = binary->right = NULL;
    } else {
        max_pre = binary_search_tree_find_ptr_to_max(&binary->left);
        max = *max_pre;

        binary_search_tree_swap_child(binary, max);
        *max_pre = binary;
        *node_pre = max;

        binary_search_tree_lt_doubly_child_strip(max_pre, binary);
    }
}

static inline void
binary_search_tree_child_strip_from_min(struct binary_search_tree **node_pre)
{
    struct binary_search_tree *binary;
    struct binary_search_tree *min;
    struct binary_search_tree **min_pre;

    assert_exit(!NULL_PTR_P(node_pre));
    assert_exit(binary_search_tree_structure_legal_p(*node_pre));
    assert_exit(binary_search_tree_doubly_child_p(*node_pre));

    binary = *node_pre;

    if (!binary->right->left) {
        // short cut here
        *node_pre = binary->right;
        binary->right->left = binary->left;
        binary->left = binary->right = NULL;
    } else {
        min_pre = binary_search_tree_find_ptr_to_min(&binary->right);
        min = *min_pre;

        binary_search_tree_swap_child(binary, min);
        *min_pre = binary;
        *node_pre = min;

        binary_search_tree_lt_doubly_child_strip(min_pre, binary);
    }
}

static inline void
binary_search_tree_doubly_child_strip(struct binary_search_tree **node_pre)
{
    assert_exit(!NULL_PTR_P(node_pre));
    assert_exit(binary_search_tree_structure_legal_p(*node_pre));
    assert_exit(binary_search_tree_doubly_child_p(*node_pre));

    if (direct > 0) {
        binary_search_tree_child_strip_from_max(node_pre);
    } else {
        binary_search_tree_child_strip_from_min(node_pre);
    }

    direct = 0;
}

static inline struct binary_search_tree *
binary_search_tree_remove_internal(struct binary_search_tree **tree,
    struct binary_search_tree *node)
{
    sint64 nice;
    struct binary_search_tree *n;
    struct binary_search_tree *removed;
    struct binary_search_tree **pre;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(binary_search_tree_structure_legal_p(*tree));
    assert_exit(binary_search_tree_ordered_p(*tree));
    assert_exit(binary_search_tree_ordered_p(*tree));
    assert_exit(binary_search_tree_structure_legal_p(node));

    pre = tree;
    n = *pre;
    nice = node->nice;
    removed = NULL;

    while (n) {
        if (nice > n->nice) {
            pre = &n->right;
            direct++;
        } else if (nice < n->nice) {
            pre = &n->left;
            direct--;
        } else if (node != n) {
            if (n->left && nice == n->left->nice) {
                removed = binary_search_tree_remove_internal(&n->left, node);
            }
            if (NULL == removed && n->right && nice == n->right->nice) {
                removed = binary_search_tree_remove_internal(&n->right, node);
            }
            break;
        } else if (binary_search_tree_doubly_child_p(n)) {
            binary_search_tree_doubly_child_strip(pre);
            return n;
        } else {
            binary_search_tree_lt_doubly_child_strip(pre, n);
            return n;
        }
        n = *pre;
    }

    if (NULL == removed) {
        pr_log_warn("Failed to find the node in given tree.\n");
    }

    assert_exit(binary_search_tree_ordered_p(*tree));
    return removed;
}

struct binary_search_tree *
binary_search_tree_remove(struct binary_search_tree **tree,
    struct binary_search_tree *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!binary_search_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!binary_search_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return binary_search_tree_remove_internal(tree, node);
    }
}

static inline void
binary_search_tree_iterate_internal(struct binary_search_tree *tree,
    void (*handle)(void *), enum ITER_ORDER order)
{
    assert_exit(LEGAL_ORDER_P(order));
    assert_exit(!NULL_PTR_P(handle));

    if (tree) {
        if (ORDER_PRE == order) {
            handle(tree);
        }

        binary_search_tree_iterate_internal(tree->left, handle, order);

        if (ORDER_IN == order) {
            handle(tree);
        }

        binary_search_tree_iterate_internal(tree->right, handle, order);

        if (ORDER_POST == order) {
            handle(tree);
        }
    }
}

void
binary_search_tree_iterate(struct binary_search_tree *tree,
    void (*handle)(void *), enum ITER_ORDER order)
{
    if (NULL_PTR_P(handle)) {
        return;
    } else if (!binary_search_tree_structure_legal_p(tree)) {
        return;
    } else if (!LEGAL_ORDER_P(order)) {
        pr_log_warn("Invalid value of enum ITER_ORDER.\n");
        return;
    } else {
        binary_search_tree_iterate_internal(tree, handle, order);
    }
}

static inline bool
binary_search_tree_ordered_p(struct binary_search_tree *tree)
{
    struct binary_search_tree *left;
    struct binary_search_tree *right;

    if (!tree) {
        return true;
    } else {
        left = tree->left;
        right = tree->right;

        if (left && tree->nice < left->nice) {
            return false;
        } else if (right && tree->nice > right->nice) {
            return false;
        } else {
            if (!binary_search_tree_ordered_p(left)) {
                return false;
            } else {
                return binary_search_tree_ordered_p(right);
            }
        }
    }
}

sint64
splay_tree_nice(struct splay_tree *tree)
{
    if (!splay_tree_structure_legal_p(tree)) {
        return (sint64)1 << 63;
    } else {
        return tree->nice;
    }
}

void
splay_tree_nice_set(struct splay_tree *tree, sint64 nice)
{
    tree->nice = nice;
}

struct splay_tree *
splay_tree_left(struct splay_tree *tree)
{
    if (!splay_tree_structure_legal_p(tree)) {
        return NULL;
    } else {
        return tree->left;
    }
}

struct splay_tree *
splay_tree_right(struct splay_tree *tree)
{
    if (!splay_tree_structure_legal_p(tree)) {
        return NULL;
    } else {
        return tree->right;
    }
}

static inline bool
splay_tree_structure_legal_p(struct splay_tree *tree)
{
    if (NULL_PTR_P(tree)) {
        return false;
    } else if (tree->left == tree->right && NULL != tree->left) {
        return false;
    } else if (tree->left == tree || tree->right == tree) {
        return false;
    } else {
        return true;
    }
}

static inline void
splay_tree_initial_internal(struct splay_tree *tree, sint64 nice)
{
     assert_exit(splay_tree_structure_legal_p(tree));

     tree->left = NULL;
     tree->right = NULL;
     tree->nice = nice;
}

struct splay_tree *
splay_tree_create(void)
{
    struct splay_tree *tree;

    tree = memory_cache_allocate(sizeof(*tree));
    splay_tree_initial_internal(tree, (sint64)0);

    return tree;
}

void
splay_tree_initial(struct splay_tree *tree, sint64 nice)
{
    if (splay_tree_structure_legal_p(tree)) {
        splay_tree_initial_internal(tree, nice);
    }
}

static inline void
splay_tree_node_destroy(struct splay_tree *node)
{
    assert_exit(splay_tree_structure_legal_p(node));
    assert_exit(NULL_PTR_P(node->left));
    assert_exit(NULL_PTR_P(node->right));

    memory_cache_free(node);
}

static inline void
splay_tree_destroy_internal(struct splay_tree *tree)
{
    if (tree) {
        /*
         * destroy node in post iterater order.
         * Warning: sometime if nested function call is too deepth,
         *     it may reach the default limitation of elf stack size, 8192KB.
         *     use ulimit -s unlimited or refine this function without nested.
         */
        splay_tree_destroy_internal(tree->left);
        splay_tree_destroy_internal(tree->right);

        tree->left = NULL;
        tree->right = NULL;

        splay_tree_node_destroy(tree);
    }
}

void
splay_tree_destroy(struct splay_tree **tree)
{
    if (NULL_PTR_P(tree)) {
        return;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return;
    } else {
        splay_tree_destroy_internal(*tree);
        *tree = NULL;
    }
}

static inline struct splay_tree *
splay_tree_find_internal(struct splay_tree **tree, sint64 nice,
    struct splay_tree *root)
{
    struct splay_tree *splay;
    struct splay_tree *found;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(root));

    found = NULL;
    splay = *tree;

    if (NULL_PTR_P(splay)) {
        return NULL;
    } else {
        if (nice < splay->nice) {
            found = splay_tree_find_internal(&splay->left, nice, root);
            if (NULL != found) {
                splay_tree_balance_splaying_left(tree, root, found);
            }
        } else if (nice > splay->nice) {
            found = splay_tree_find_internal(&splay->right, nice, root);
            if (NULL != found) {
                splay_tree_balance_splaying_right(tree, root, found);
            }
        } else {
            found = splay;
        }

        return found;
    }
}

struct splay_tree *
splay_tree_find(struct splay_tree **tree, sint64 nice)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else {
        return splay_tree_find_internal(tree, nice, *tree);
    }
}

static inline struct splay_tree *
splay_tree_find_min_internal(struct splay_tree **tree,
    struct splay_tree *root)
{
    struct splay_tree *min;
    struct splay_tree *node;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p(root));

    node = *tree;

    if (NULL == node->left) {
        return node;
    } else {
        min = splay_tree_find_min_internal(&node->left, root);

        if (node->left->left) {
            splay_tree_balance_single_splaying_left(tree);
        } else if (node == root) {
            splay_tree_balance_root_splaying_left(tree);
        } else {
            assert_exit(min == node->left);
            /*
             *       / 
             *     node
             *     /
             *   min
             *   /
             */
            return min;
        }

        assert_exit(*tree == min);
        return min;
    }
}

struct splay_tree *
splay_tree_find_min(struct splay_tree **tree)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else {
        return splay_tree_find_min_internal(tree, *tree);
    }
}

static inline struct splay_tree *
splay_tree_find_max_internal(struct splay_tree **tree,
    struct splay_tree *root)
{
    struct splay_tree *max;
    struct splay_tree *node;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p(root));

    node = *tree;

    if (NULL == node->right) {
        return node;
    } else {
        max = splay_tree_find_max_internal(&node->right, root);

        if (node->right->right) {
            splay_tree_balance_single_splaying_right(tree);
        } else if (node == root) {
            splay_tree_balance_root_splaying_right(tree);
        } else {
            assert_exit(max == node->right);
            /*
             *       / 
             *     node
             *     /
             *   min
             *   /
             */
            return max;
        }

        assert_exit(*tree == max);
        return max;
    }
}

struct splay_tree *
splay_tree_find_max(struct splay_tree **tree)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else {
        return splay_tree_find_max_internal(tree, *tree);
    }
}

static inline void
splay_tree_balance_splaying_left(struct splay_tree **tree,
    struct splay_tree *root, struct splay_tree *target)
{
    struct splay_tree *node;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p(root));
    assert_exit(splay_tree_structure_legal_p(target));
    assert_exit(splay_tree_structure_legal_p((*tree)->left));

    node = *tree;

    if (node == root && node->left == target) {
        /*
         *    root  <- node
         *     /
         *  target
         */
        splay_tree_balance_root_splaying_left(tree);
    } else if (node->left->left == target) {
        splay_tree_balance_single_splaying_left(tree);
    } else if (node->left->right == target) {
        splay_tree_balance_doubly_splaying_left(tree);
    } else {
        assert_exit(target == node->left);
        return;
    }

    assert_exit(target == *tree);
}

static inline void
splay_tree_balance_splaying_right(struct splay_tree **tree,
    struct splay_tree *root, struct splay_tree *target)
{
    struct splay_tree *node;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p(root));
    assert_exit(splay_tree_structure_legal_p(target));
    assert_exit(splay_tree_structure_legal_p((*tree)->right));

    node = *tree;

    if (node == root && node->right == target) {
        /*
         *  root  <- node
         *     \
         *    target
         */
        splay_tree_balance_root_splaying_right(tree);
    } else if (node->right->left == target) {
        splay_tree_balance_doubly_splaying_right(tree);
    } else if (node->right->right == target) {
        splay_tree_balance_single_splaying_right(tree);
    } else {
        assert_exit(target == node->right);
        return;
    }

    assert_exit(target == *tree);
}

/*
 *     k1          k2
 *    /  \        /  \
 *   k2   c  =>  a    k1
 *  /  \             /  \
 * a    b           b    c
 */
static void inline
splay_tree_balance_root_splaying_left(struct splay_tree **tree)
{
    struct splay_tree *k1;
    struct splay_tree *k2;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p((*tree)->left));

    k1 = *tree;
    k2 = k1->left;

    k1->left = k2->right;
    k2->right = k1;

    *tree = k2;
}

/*
 *   k1               k2
 *  /  \             /  \
 * a    k2    =>    k1   c
 *     /  \        /  \
 *    b    c      a    b
 */
static void inline
splay_tree_balance_root_splaying_right(struct splay_tree **tree)
{
    struct splay_tree *k1;
    struct splay_tree *k2;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p((*tree)->right));

    k1 = *tree;
    k2 = k1->right;

    k1->right = k2->left;
    k2->left = k1;

    *tree = k2;
}

/*
 *        k1            k3
 *       /  \          /  \
 *      k2   d   =>   a    k2
 *     /  \               /  \
 *    k3   c             b    k1
 *   /  \                    /  \
 *  a    b                  c    d
 */
static void inline
splay_tree_balance_single_splaying_left(struct splay_tree **tree)
{
    struct splay_tree *k1;
    struct splay_tree *k2;
    struct splay_tree *k3;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p((*tree)->left));
    assert_exit(splay_tree_structure_legal_p((*tree)->left->left));

    k1 = *tree;
    k2 = k1->left;
    k3 = k2->left;

    k1->left = k2->right;
    k2->left = k3->right;
    k2->right = k1;
    k3->right = k2;

    *tree = k3;
}

/*
 *     k1                 k3
 *    / \                /  \
 *   a   k2      =>     k2   d
 *      /  \           /  \
 *     b    k3        k1   c
 *         /  \      /  \
 *        c    d    a    b
 */
static void inline
splay_tree_balance_single_splaying_right(struct splay_tree **tree)
{
    struct splay_tree *k1;
    struct splay_tree *k2;
    struct splay_tree *k3;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p((*tree)->right));
    assert_exit(splay_tree_structure_legal_p((*tree)->right->right));

    k1 = *tree;
    k2 = k1->right;
    k3 = k2->right;

    k1->right = k2->left;
    k2->right = k3->left;
    k2->left = k1;
    k3->left = k2;

    *tree = k3;
}

/*
 *     k1            k3
 *    /  \          /  \
 *   k2   d  =>    k2   k1
 *  / \           / \   / \
 * a   k3        a   b c   d
 *    /  \
 *   b    c
 */
static void inline
splay_tree_balance_doubly_splaying_left(struct splay_tree **tree)
{
    struct splay_tree *k1;
    struct splay_tree *k2;
    struct splay_tree *k3;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p((*tree)->left));
    assert_exit(splay_tree_structure_legal_p((*tree)->left->right));

    k1 = *tree;
    k2 = k1->left;
    k3 = k2->right;

    k1->left = k3->right;
    k2->right = k3->left;
    k3->left = k2;
    k3->right = k1;

    *tree = k3;
}

/*
 *    k1              k3
 *   / \             /  \
 *  a   k2    =>    k1   k2
 *     /  \        / \   / \
 *    k3   c      a   b c   d
 *   /  \
 *  b    c
 */
static void inline
splay_tree_balance_doubly_splaying_right(struct splay_tree **tree)
{
    struct splay_tree *k1;
    struct splay_tree *k2;
    struct splay_tree *k3;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p((*tree)->right));
    assert_exit(splay_tree_structure_legal_p((*tree)->right->left));

    k1 = *tree;
    k2 = k1->right;
    k3 = k2->left;

    k1->right = k3->left;
    k2->left = k3->right;
    k3->left = k1;
    k3->right = k2;

    *tree = k3;
}

static inline sint32
splay_tree_height_internal(struct splay_tree *tree)
{
    sint32 left;
    sint32 right;

    if (!tree) {
        return -1;
    } else {
        left = splay_tree_height_internal(tree->left);
        right = splay_tree_height_internal(tree->right);

        return MAX_S32(left, right) + 1;
    }
}

sint32
splay_tree_height(struct splay_tree *tree)
{
    if (!splay_tree_structure_legal_p(tree)) {
        return -1;
    } else {
        return splay_tree_height_internal(tree);
    }
}

static inline bool
splay_tree_contains_p_internal(struct splay_tree *tree, struct splay_tree *node)
{
    bool retval;
    sint64 nice;
    struct splay_tree *left;
    struct splay_tree *right;

    assert_exit(splay_tree_structure_legal_p(tree));
    assert_exit(splay_tree_structure_legal_p(node));

    retval = false;
    nice = node->nice;

    while (tree) {
        if (node == tree) {
            return true;
        } else if (nice > tree->nice) {
            tree = tree->right;
        } else if (nice < tree->nice) {
            tree = tree->left;
        } else {
            left = tree->left;
            right = tree->right;

            // Handle repeated nice
            if (left && nice == left->nice) {
                retval = splay_tree_contains_p_internal(left, node);
            }
            if (!retval && right && nice == right->nice) {
                retval = splay_tree_contains_p_internal(right, node);
            }

            return retval;
        }
    }

    return false;
}

bool
splay_tree_contains_p(struct splay_tree *tree, struct splay_tree *node)
{
    if (!splay_tree_structure_legal_p(tree)) {
        return false;
    } else if (!splay_tree_structure_legal_p(node)) {
        return false;
    } else {
        return splay_tree_contains_p_internal(tree, node);
    }
}

static inline struct splay_tree *
splay_tree_insert_internal(struct splay_tree **tree,
    struct splay_tree *node, struct splay_tree *root)
{
    struct splay_tree *splay;
    struct splay_tree *inserted;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_ordered_p(*tree));
    assert_exit(splay_tree_structure_legal_p(node));
    assert_exit(splay_tree_structure_legal_p(root));

    if (!*tree) {
        return *tree = node;
    } else {
        splay = *tree;
        if (node->nice < splay->nice) {
            inserted = splay_tree_insert_internal(&splay->left, node, root);
            splay_tree_balance_splaying_left(tree, root, inserted);
        } else if (node->nice > splay->nice) {
            inserted = splay_tree_insert_internal(&splay->right, node, root);
            splay_tree_balance_splaying_right(tree, root, inserted);
        } else if (splay != node) {
            // Repeated nice, insert it to right
            inserted = splay_tree_insert_internal(&splay->right, node, root);
            splay_tree_balance_splaying_right(tree, root, inserted);
        } else {
            pr_log_warn("Insert node exist, nothing will be done.\n");
            return NULL;
        }

        assert_exit(splay_tree_ordered_p(*tree));
        return inserted;
    }
}

struct splay_tree *
splay_tree_insert(struct splay_tree **tree, struct splay_tree *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return splay_tree_insert_internal(tree, node, *tree);
    }
}

static inline bool
splay_tree_doubly_child_p(struct splay_tree *node)
{
    assert_exit(splay_tree_structure_legal_p(node));

    if (node->left && node->right) {
        return true;
    } else {
        return false;
    }
}

static inline void
splay_tree_lt_doubly_child_strip(struct splay_tree **pre,
    struct splay_tree *node)
{
    assert_exit(!NULL_PTR_P(pre));
    assert_exit(splay_tree_structure_legal_p(node));
    assert_exit(splay_tree_structure_legal_p(*pre));
    assert_exit(!splay_tree_doubly_child_p(node));
    assert_exit(*pre == node);

    if (NULL != node->left) {
        *pre = node->left;
    } else {
        *pre = node->right;
    }

    node->left = NULL;
    node->right = NULL;
}

static inline struct splay_tree **
splay_tree_find_ptr_to_min(struct splay_tree **tree)
{
    struct splay_tree **min;
    struct splay_tree *splay;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));

    min = tree;
    splay = *min;

    while (splay->left) {
        min = &splay->left;
        splay = *min;
    }

    return min;
}

static inline void
splay_tree_swap_child(struct splay_tree *a, struct splay_tree *b)
{
    void *tmp;

    assert_exit(splay_tree_structure_legal_p(a));
    assert_exit(splay_tree_structure_legal_p(b));

    tmp = a->left;
    a->left = b->left;
    b->left = tmp;

    tmp = a->right;
    a->right = b->right;
    b->right = tmp;
}

static inline void
splay_tree_doubly_child_strip(struct splay_tree **pre)
{
    struct splay_tree *splay;
    struct splay_tree *min;
    struct splay_tree **min_pre;

    assert_exit(!NULL_PTR_P(pre));
    assert_exit(splay_tree_structure_legal_p(*pre));
    assert_exit(splay_tree_doubly_child_p(*pre));

    splay = *pre;

    if (!splay->right->left) {
        // short cut here
        *pre = splay->right;
        splay->right->left = splay->left;
        splay->left = splay->right = NULL;
    } else {
        min_pre = splay_tree_find_ptr_to_min(&splay->right);
        min = *min_pre;

        splay_tree_swap_child(splay, min);
        *min_pre = splay;
        *pre = min;

        splay_tree_lt_doubly_child_strip(min_pre, splay);
    }
}

static inline struct splay_tree *
splay_tree_remove_internal(struct splay_tree **tree,
    struct splay_tree *node)
{
    struct splay_tree *splay;
    struct splay_tree *removed;
    struct splay_tree **pre;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_ordered_p(*tree));
    assert_exit(splay_tree_structure_legal_p(node));

    pre = tree;
    splay = *pre;
    removed = NULL;

    while (splay) {
        if (node->nice > splay->nice) {
            pre = &splay->right;
        } else if (node->nice < splay->nice) {
            pre = &splay->left;
        } else if (splay != node) {
            if (splay->left && node->nice == splay->left->nice) {
                removed = splay_tree_remove_internal(&splay->left, node);
            }
            if (!removed && splay->right && node->nice == splay->right->nice) {
                removed = splay_tree_remove_internal(&splay->right, node);
            }
            break;
        } else if (splay_tree_doubly_child_p(splay)) {
            splay_tree_doubly_child_strip(pre);
            return splay;
        } else {
            splay_tree_lt_doubly_child_strip(pre, splay);
            return splay;
        }
        splay = *pre;
    }

    if (NULL == removed) {
        pr_log_warn("Failed to find the node in given tree.\n");
    }

    assert_exit(splay_tree_ordered_p(*tree));
    return removed;
}

struct splay_tree *
splay_tree_remove(struct splay_tree **tree, struct splay_tree *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return splay_tree_remove_internal(tree, node);
    }
}

static inline void
splay_tree_iterate_internal(struct splay_tree *tree,
    void (*handle)(void *), enum ITER_ORDER order)
{
    assert_exit(LEGAL_ORDER_P(order));
    assert_exit(!NULL_PTR_P(handle));

    if (tree) {
        if (ORDER_PRE == order) {
            handle(tree);
        }

        splay_tree_iterate_internal(tree->left, handle, order);

        if (ORDER_IN == order) {
            handle(tree);
        }

        splay_tree_iterate_internal(tree->right, handle, order);

        if (ORDER_POST == order) {
            handle(tree);
        }
    }
}

void
splay_tree_iterate(struct splay_tree *tree,
    void (*handle)(void *), enum ITER_ORDER order)
{
    if (NULL_PTR_P(tree) || NULL_PTR_P(handle)) {
        return;
    } else if (!LEGAL_ORDER_P(order)) {
        pr_log_warn("illegal oder type of iterate.\n");
    } else {
        splay_tree_iterate_internal(tree, handle, order);
    }
}

static inline bool
splay_tree_ordered_p(struct splay_tree *tree)
{
    struct splay_tree *left;
    struct splay_tree *right;

    if (!tree) {
        return true;
    } else {
        left = tree->left;
        right = tree->right;

        if (left && tree->nice < left->nice) {
            return false;
        } else if (right && tree->nice > right->nice) {
            return false;
        } else {
            if (!splay_tree_ordered_p(left)) {
                return false;
            } else {
                return splay_tree_ordered_p(right);
            }
        }
    }
}

s_trie_tree_t *
trie_tree_create(void)
{
    return trie_tree_node_create(TRIE_TREE_ROOT);
}

static inline s_trie_tree_t *
trie_tree_node_create(uint32 val)
{
    s_trie_tree_t *trie;

    trie = dp_malloc(sizeof(*trie));
    trie->sub_queue = array_queue_create();

    trie->val = val;
    trie->is_deleted = trie->is_terminal = false;

    return trie;
}

static inline void
trie_tree_node_destroy(s_trie_tree_t *trie)
{
    assert_exit(trie_tree_structure_legal_ip(trie));

    array_queue_destroy(&trie->sub_queue);
    dp_free(trie);
}

void
trie_tree_destroy(s_trie_tree_t **trie)
{
    s_trie_tree_t *trie_node;
    s_array_queue_t *sub_queue, *queue;

    if (NULL_PTR_P(trie)) {
        return;
    } else if (trie_tree_structure_legal_ip(*trie)) {
        trie_node = *trie;
        queue = array_queue_create();
        array_queue_enter(queue, trie_node);

        while (!array_queue_empty_p(queue)) {
            trie_node = array_queue_leave(queue);
            sub_queue = trie_node->sub_queue;

            while (!array_queue_empty_p(sub_queue)) {
                array_queue_enter(queue, array_queue_leave(sub_queue));
            }

            trie_tree_node_destroy(trie_node);
        }

        array_queue_destroy(&queue);
        *trie = NULL;
    }
}

bool
trie_tree_structure_legal_p(s_trie_tree_t *trie)
{
    return trie_tree_structure_legal_ip(trie);
}

static inline bool
trie_tree_structure_legal_ip(s_trie_tree_t *trie)
{
    if (NULL_PTR_P(trie)) {
        return false;
    } else {
        return array_queue_structure_legal_p(trie->sub_queue);
    }
}

static inline bool
trie_tree_root_node_p(s_trie_tree_t *trie)
{
    assert_exit(trie_tree_structure_legal_ip(trie));

    return trie->val == TRIE_TREE_ROOT ? true : false;
}

static inline s_trie_tree_t *
trie_tree_scalar_insert(s_trie_tree_t *trie, uint32 val)
{
    s_trie_tree_t *trie_node;

    assert_exit(val != TRIE_TREE_ROOT);
    assert_exit(trie_tree_structure_legal_p(trie));

    trie_node = trie_tree_sub_queue_find(trie, val);

    if (trie_node == NULL) {
        trie_node = trie_tree_node_create(val);
        array_queue_enter(trie->sub_queue, trie_node);
    }

    return trie_node;
}

static inline void
trie_tree_sequence_insert_i(s_trie_tree_t *trie, uint32 *sequence, uint32 len)
{
    uint32 *seq;
    s_trie_tree_t *trie_node;

    assert_exit(!complain_zero_size_p(len));
    assert_exit(trie_tree_root_node_p(trie));
    assert_exit(trie_tree_structure_legal_p(trie));
    assert_exit(!NULL_PTR_P(sequence));

    seq = sequence;
    trie_node = trie;

    while (seq < sequence + len) {
        trie_node = trie_tree_scalar_insert(trie_node, *seq);
        assert_exit(trie_node->val == *seq);
        seq++;
    }

    trie_node->is_terminal = true;
}

void
trie_tree_sequence_insert(s_trie_tree_t *trie, uint32 *sequence, uint32 len)
{
    if (!trie_tree_structure_legal_ip(trie)) {
        return;
    } else if (!trie_tree_root_node_p(trie)) {
        return;
    } else if (complain_zero_size_p(len)) {
        return;
    } else if (NULL_PTR_P(sequence)) {
        return;
    } else {
        trie_tree_sequence_insert_i(trie, sequence, len);
    }
}

void
trie_tree_string_insert(s_trie_tree_t *trie, char *string)
{
    uint32 len;
    uint32 *sequence;

    if (!trie_tree_structure_legal_ip(trie)) {
        return;
    } else if (!trie_tree_root_node_p(trie)) {
        return;
    } else if (complain_null_string_p(string)) {
        return;
    } else {
        sequence = convert_string_to_uint32_array(string, &len);
        assert_exit(sequence != PTR_INVALID);

        trie_tree_sequence_insert_i(trie, sequence, len);
        dp_free(sequence);
    }
}

static inline s_trie_tree_t *
trie_tree_sub_queue_find(s_trie_tree_t *trie, uint32 val)
{
    s_trie_tree_t *sub_node;
    s_array_queue_t *sub_queue;
    s_array_iterator_t *iterator;

    assert_exit(val != TRIE_TREE_ROOT);
    assert_exit(trie_tree_structure_legal_p(trie));

    sub_queue = trie->sub_queue;
    iterator = array_queue_iterator_obtain(sub_queue);
    assert_exit(iterator != PTR_INVALID);

    iterator->fp_index_initial(sub_queue);
    while (iterator->fp_next_exist_p(sub_queue)) {
        sub_node = iterator->fp_next_obtain(sub_queue);

        if (sub_node->val == val && !sub_node->is_deleted) {
            return sub_node;
        }
    }

    return NULL;
}

bool
trie_tree_sequence_matched_p(s_trie_tree_t *trie, uint32 *sequence, uint32 len)
{
    if (NULL_PTR_P(sequence)) {
        return false;
    } else if (complain_zero_size_p(len)) {
        return false;
    } else if (!trie_tree_structure_legal_ip(trie)) {
        return false;
    } else {
        return trie_tree_sequence_matched_ip(trie, sequence, len);
    }
}

bool
trie_tree_string_matched_p(s_trie_tree_t *trie, char *string)
{
    uint32 len;
    bool is_matched;
    uint32 *sequence;

    if (!trie_tree_structure_legal_ip(trie)) {
        return false;
    } else if (!trie_tree_root_node_p(trie)) {
        return false;
    } else if (NULL_PTR_P(string)) {
        return false;
    } else if (complain_null_string_p(string)) {
        return false;
    } else {
        sequence = convert_string_to_uint32_array(string, &len);
        assert_exit(sequence != PTR_INVALID);

        is_matched = trie_tree_sequence_matched_ip(trie, sequence, len);
        dp_free(sequence);

        return is_matched;
    }
}

static inline bool
trie_tree_sub_queue_empty_p(s_trie_tree_t *trie)
{
    s_trie_tree_t *trie_node;
    s_array_queue_t *sub_queue;
    s_array_iterator_t *iterator;

    assert_exit(trie_tree_structure_legal_p(trie));

    sub_queue = trie->sub_queue;
    iterator = array_queue_iterator_obtain(sub_queue);

    iterator->fp_index_initial(sub_queue);
    while (iterator->fp_next_exist_p(sub_queue)) {
        trie_node = iterator->fp_next_obtain(sub_queue);

        if (trie_node->is_deleted == false) {
            return false;
        }
    }

    return true;
}

static inline void
trie_tree_sequence_remove_i(s_trie_tree_t *trie, uint32 *sequence, uint32 len)
{
    uint32 *seq;
    s_array_stack_t *stack;
    s_trie_tree_t *trie_node;

    assert_exit(!complain_zero_size_p(len));
    assert_exit(trie_tree_root_node_p(trie));
    assert_exit(trie_tree_structure_legal_p(trie));
    assert_exit(!NULL_PTR_P(sequence));

    seq = sequence;
    trie_node = trie;
    stack = array_stack_create();

    while (seq < sequence + len) {
        trie_node = trie_tree_sub_queue_find(trie_node, *seq);
        if (trie_node == NULL) {
            array_stack_destroy(&stack);
            return;
        } else {
            array_stack_push(stack, trie_node);
            seq++;
        }
    }

    while (!array_stack_empty_p(stack)) {
        trie_node = array_stack_pop(stack);
        if (trie_tree_sub_queue_empty_p(trie_node)) {
            trie_node->is_deleted = true;
        }
    }

    array_stack_destroy(&stack);
}

void
trie_tree_sequence_remove(s_trie_tree_t *trie, uint32 *sequence, uint32 len)
{
    if (!trie_tree_structure_legal_ip(trie)) {
        return;
    } else if (!trie_tree_root_node_p(trie)) {
        return;
    } else if (complain_zero_size_p(len)) {
        return;
    } else if (NULL_PTR_P(sequence)) {
        return;
    } else {
        trie_tree_sequence_remove_i(trie, sequence, len);
    }
}

void
trie_tree_string_remove(s_trie_tree_t *trie, char *string)
{
    uint32 len;
    uint32 *sequence;

    if (!trie_tree_structure_legal_ip(trie)) {
        return;
    } else if (!trie_tree_root_node_p(trie)) {
        return;
    } else if (complain_null_string_p(string)) {
        return;
    } else {
        sequence = convert_string_to_uint32_array(string, &len);
        assert_exit(sequence != PTR_INVALID);

        trie_tree_sequence_remove_i(trie, sequence, len);
        dp_free(sequence);
    }
}

static inline bool
trie_tree_sequence_matched_ip(s_trie_tree_t *trie, uint32 *sequence, uint32 len)
{
    uint32 i;
    s_trie_tree_t *trie_node;

    assert_exit(!complain_zero_size_p(len));
    assert_exit(trie_tree_root_node_p(trie));
    assert_exit(trie_tree_structure_legal_p(trie));
    assert_exit(!NULL_PTR_P(sequence));

    trie_node = trie;

    i = 0;
    while (i < len) {
        trie_node = trie_tree_sub_queue_find(trie_node, sequence[i]);
        if (trie_node == NULL) {
            return false;
        }
        i++;
    }

    return trie_node->is_terminal;
}

