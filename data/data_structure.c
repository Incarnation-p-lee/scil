bool
sort_data_sorted_p(void *data, uint32 size, uint32 csize,
    sint32 (*compare)(const void *, const void *))
{
    void *a;
    void *b;
    uint32 i;

    assert_exit(!NULL_PTR_P(data));
    assert_exit(!NULL_PTR_P(compare));
    assert_exit(!complain_zero_size_p(size));
    assert_exit(!complain_zero_size_p(csize));

    i = 1;
    while (i < size) {
        a = data + (i - 1) * csize;
        b = data + i * csize;

        if (compare(a, b) > 0) {
            return false;
        }
        i++;
    }

    return true;
}

static inline void
sort_cell_copy(void *to, void *from, uint32 size)
{
    assert_exit(!NULL_PTR_P(to));
    assert_exit(!NULL_PTR_P(from));
    assert_exit(!complain_zero_size_p(size));

    if (sizeof(uint64) == size) {
        *(uint64 *)to = *(uint64 *)from;
    } else if (sizeof(uint32) == size) {
        *(uint32 *)to = *(uint32 *)from;
    } else if (sizeof(uint16) == size) {
        *(uint16 *)to = *(uint16 *)from;
    } else if (sizeof(uint8) == size) {
        *(uint8 *)to = *(uint8 *)from;
    } else {
        dp_memcpy(to, from, size);
    }
}

static inline void
sort_cell_swap(void *a, void *b, uint32 size)
{
    void *tmp;

    assert_exit(!NULL_PTR_P(a));
    assert_exit(!NULL_PTR_P(b));
    assert_exit(!complain_zero_size_p(size));

    tmp = memory_cache_allocate(size);

    sort_cell_copy(tmp, a, size);
    sort_cell_copy(a, b, size);
    sort_cell_copy(b, tmp, size);

    memory_cache_free(tmp);
}

static inline bool
sort_parameters_legal_p(void *base, uint32 size, uint32 csize,
    sint32 (*compare)(const void *, const void *))
{
    if (NULL_PTR_P(base)) {
        return false;
    } else if (NULL_PTR_P(compare)) {
        return false;
    } else if (complain_zero_size_p(size)) {
        return false;
    } else if (complain_zero_size_p(csize)) {
        return false;
    } else {
        return true;
    }
}

/*
 *
 * 1,2,4,6,7,[5]
 * tmp = [5]
 * 1,2,4,6,7,7
 * 1,2,4,6,6,7
 * [5] = tmp
 * 1,2,4,5,6,7
 *
 */
void
insertion_sort(void *base, uint32 size, uint32 csize,
    sint32 (*compare)(const void *, const void *))
{
    uint32 i;
    uint32 j;
    void *tmp;

    if (sort_parameters_legal_p(base, size, csize, compare)) {
        i = 1;
        tmp = memory_cache_allocate(csize);

        while (i < size) {
            sort_cell_copy(tmp, base + i * csize, csize);
            j = i;

            while (j > 0 && compare(base + (j - 1) * csize, tmp) > 0) {
                sort_cell_copy(base + j * csize, base + (j - 1) * csize, csize);
                j--;
            }

            sort_cell_copy(base + j * csize, tmp, csize);
            i++;
        }

        memory_cache_free(tmp);
        assert_exit(sort_data_sorted_p(base, size, csize, compare));
    }
}

void
shell_sort(void *base, uint32 size, uint32 csize,
    sint32 (*compare)(const void *, const void *))
{
    uint32 i;
    uint32 j;
    uint32 k;
    void *tmp;
    uint32 inc;

    if (sort_parameters_legal_p(base, size, csize, compare)) {
        tmp = memory_cache_allocate(csize);
        k = shell_sort_exponet_get(size);

        while (k > 0) {
            inc = shell_sort_hibbard_increment_seq(k);
            i = inc;

            while (i < size) {
                sort_cell_copy(tmp, base + i * csize, csize);
                j = i;

                while (j >= inc && compare(base + (j - inc) * csize, tmp) > 0) {
                    sort_cell_copy(base + j * csize, base + (j - inc) * csize, csize);
                    j -= inc;
                }

                sort_cell_copy(base + j * csize, tmp, csize);
                i++;
            }

            k--;
        }

        memory_cache_free(tmp);
        assert_exit(sort_data_sorted_p(base, size, csize, compare));
    }
}

/*
 * Here shell sort use Hibbard increment sequence.
 * H[k] = 2^k - 1.
 * with worst time O(N^(3/2))
 */
static inline uint32
shell_sort_exponet_get(uint32 size)
{
    uint32 retval;

    assert_exit(size > 1);

    retval = 0;
    while (size) {
        retval += 1;
        size = size >> 1;
    }

    return retval - 1;
}

/*
 * For shell sort H[k] = 2^k - 1.
 */
static const uint32 hibbard_sequence[] = {
    0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191,
    16383, 32767, 65535, 131071, 262143, 524287, 1048575, 2097151,
    4194303, 8388607, 16777215, 33554431, 67108863, 134217727,
    268435455, 536870911, 1073741823, 2147483647, 4294967295u,
};

static inline uint32
shell_sort_hibbard_increment_seq(uint32 k)
{
    assert_exit(!complain_zero_size_p(k));
    assert_exit(k < array_sizeof(hibbard_sequence));

    return hibbard_sequence[k];
}

void
heap_sort(void *base, uint32 size, uint32 csize,
    sint32 (*compare)(const void *, const void *))
{
    uint32 index;

    if (sort_parameters_legal_p(base, size, csize, compare)) {
        heap_sort_build_heap(base, size, csize, compare);

        // Put the max one from heap top to tail of array
        index = size - 1;
        while (index > 0) {
            sort_cell_swap(base, base + index * csize, csize);
            heap_sort_percolate_down(base, index, csize, 0, compare);
            index--;
        }

        assert_exit(sort_data_sorted_p(base, size, csize, compare));
    }
}

static inline void
heap_sort_build_heap(void *base, uint32 size, uint32 csize,
    sint32 (*compare)(const void *, const void *))
{
    sint32 index;

    assert_exit(sort_parameters_legal_p(base, size, csize, compare));

    index = (sint32)(size >> 1);

    while (index >= 0) {
        heap_sort_percolate_down(base, size, csize, index, compare);
        index--;
    }
}

static inline void
heap_sort_percolate_down(void *base, uint32 size, uint32 csize, uint32 index,
    sint32 (*compare)(const void *, const void *))
{
    uint32 i;
    void *tmp;
    uint32 child;

    assert_exit(sort_parameters_legal_p(base, size, csize, compare));
    assert_exit(index < size);

    tmp = memory_cache_allocate(csize);
    sort_cell_copy(tmp, base + index * csize, csize);

    i = index;
    while (HEAP_LEFT(i) < size) {
        child = HEAP_LEFT(i);
        if (child != size - 1
            && compare(base + child * csize, base + (child + 1) * csize) < 0) {
            child++;
        }

        if (compare(tmp, base + child * csize) < 0) {
            sort_cell_copy(base + i * csize, base + child * csize, csize);
        } else {
            break;
        }
        i = child;
    }

    sort_cell_copy(base + i * csize, tmp, csize);
    memory_cache_free(tmp);
}

void
quick_sort(void *base, uint32 size, uint32 csize,
    sint32 (*compare)(const void *, const void *))
{
    if (sort_parameters_legal_p(base, size, csize, compare)) {
        quick_sort_recursive(base, 0, size - 1, csize, compare);
        assert_exit(sort_data_sorted_p(base, size, csize, compare));
    }
}

static inline void
quick_sort_recursive(void *base, uint32 left, uint32 right,
    uint32 csize, sint32 (*compare)(const void *, const void *))
{
    void *ptr_l;
    void *ptr_r;
    void *ptr_m;
    uint32 median;

    assert_exit(sort_parameters_legal_p(base, csize, csize, compare));

    ptr_l = base + left * csize;
    ptr_r = base + right * csize;

    if (left + 2 < right) {
        ptr_m = quick_sort_obtain_median(base, left, right, csize, compare);
        sort_cell_swap(ptr_m, ptr_r - csize, csize);

        ptr_m = ptr_r - csize;
        ptr_r = ptr_m;

        while (true) {
            do {
                ptr_l += csize;
            } while (compare(ptr_m, ptr_l) > 0);
            do {
                ptr_r -= csize;
            } while (compare(ptr_m, ptr_r) < 0);

            if (ptr_l < ptr_r) {
                sort_cell_swap(ptr_l, ptr_r, csize);
            } else {
                break;
            }
        }

        sort_cell_swap(ptr_l, ptr_m, csize);
        median = (ptr_l - base) / csize;

        quick_sort_recursive(base, left, median - 1, csize, compare);
        quick_sort_recursive(base, median + 1, right, csize, compare);
    } else if (left + 2 == right) {
        quick_sort_obtain_median(base, left, right, csize, compare);
    } else if (left + 1 == right && compare(ptr_l, ptr_r) > 0) {
        sort_cell_swap(ptr_l, ptr_r, csize);
    }
}

static inline void *
quick_sort_obtain_median(void *base, uint32 left, uint32 right, uint32 csize,
    sint32 (*compare)(const void *, const void *))
{
    void *ptr_l;
    void *ptr_r;
    void *ptr_m;

    assert_exit(sort_parameters_legal_p(base, csize, csize, compare));

    ptr_l = base + left * csize;
    ptr_r = base + right * csize;
    ptr_m = base + ((left + right) / 2) * csize;

    if (compare(ptr_l, ptr_m) > 0) {
        sort_cell_swap(ptr_l, ptr_m, csize);
    }

    if (compare(ptr_l, ptr_r) > 0) {
        sort_cell_swap(ptr_l, ptr_r, csize);
    }

    if (compare(ptr_m, ptr_r) > 0) {
        sort_cell_swap(ptr_m, ptr_r, csize);
    }

    return ptr_m;
}

void
merge_sort(void *base, uint32 size, uint32 csize,
    sint32 (*compare)(const void *, const void *))
{
    void *tmp;

    if (sort_parameters_legal_p(base, size, csize, compare)) {
        tmp = memory_cache_allocate(size * csize);

        merge_sort_recursive(base, tmp, 0, size - 1, csize, compare);

        memory_cache_free(tmp);
        assert_exit(sort_data_sorted_p(base, size, csize, compare));
    }
}

static inline void
merge_sort_recursive(void *base, void *tmp, uint32 left, uint32 right,
    uint32 csize, sint32 (*compare)(const void *, const void *))
{
    uint32 med;

    assert_exit(!NULL_PTR_P(base));
    assert_exit(!NULL_PTR_P(tmp));
    assert_exit(!NULL_PTR_P(compare));
    assert_exit(!complain_zero_size_p(csize));

    if (left < right) {
        med = (left + right) / 2;
        merge_sort_recursive(base, tmp, left, med, csize, compare);
        merge_sort_recursive(base, tmp, med + 1, right, csize, compare);
        merge_sort_divide_merge(base, tmp, left, med, right, csize, compare);
    }
}

static inline void
merge_sort_divide_merge(void *base, void *tmp, uint32 left, uint32 med, uint32 right,
    uint32 csize, sint32 (*compare)(const void *, const void *))
{
    void *start;
    void *limit;
    void *p1, *p1_lmt;
    void *p2, *p2_lmt;

    assert_exit(!NULL_PTR_P(base));
    assert_exit(!NULL_PTR_P(compare));
    assert_exit(!NULL_PTR_P(tmp));
    assert_exit(!complain_zero_size_p(csize));
    assert_exit(left <= med && med <= right);

    p1 = base + left * csize;
    p1_lmt = base + med * csize;
    p2 = p1_lmt + csize;
    p2_lmt = base + right * csize;

    start = tmp + left * csize;
    while (p1 <= p1_lmt && p2 <= p2_lmt) {
        if (compare(p1, p2) < 0) {
            sort_cell_copy(start, p1, csize);
            p1 += csize;
        } else {
            sort_cell_copy(start, p2, csize);
            p2 += csize;
        }
        start += csize;
    }

    while (p1 <= p1_lmt) {
        sort_cell_copy(start, p1, csize);
        p1 += csize;
        start += csize;
    }

    while (p2 <= p2_lmt) {
        sort_cell_copy(start, p2, csize);
        p2 += csize;
        start += csize;
    }

    limit = tmp + right * csize;
    assert_exit(start == limit + csize);
    start = tmp + left * csize;
    p1 = base + left * csize;

    while (start <= limit) {
        sort_cell_copy(p1, start, csize);
        p1 += csize;
        start += csize;
    }
}

uint32
array_stack_size(s_array_stack_t *stack)
{
    if (!array_stack_structure_legal_ip(stack)) {
        return STACK_SIZE_INVALID;
    } else {
        return (uint32)(stack->space.sp - stack->space.bp);
    }
}

static inline uint32
array_stack_size_i(s_array_stack_t *stack)
{
    assert_exit(array_stack_structure_legal_ip(stack));

    return (uint32)(stack->space.sp - stack->space.bp);
}

bool
array_stack_space_structure_legal_p(s_array_stack_space_t *space)
{
    if (NULL_PTR_P(space)) {
        return false;
    } else if (NULL_PTR_P(space->sp)) {
        return false;
    } else if (NULL_PTR_P(space->bp)) {
        return false;
    } else if (complain_zero_size_p(space->dim)) {
        return false;
    } else if ((uint32)(space->sp - space->bp) > space->dim) {
        return false;
    } else {
        return true;
    }
}

bool
array_stack_structure_legal_p(s_array_stack_t *stack)
{
    return array_stack_structure_legal_ip(stack);
}

static inline bool
array_stack_structure_legal_ip(s_array_stack_t *stack)
{
    if (NULL_PTR_P(stack)) {
        return false;
    } else {
        return array_stack_space_structure_legal_p(&stack->space);
    }
}

s_array_stack_t *
array_stack_create(void)
{
    s_array_stack_t *stack;

    stack = memory_cache_allocate(sizeof(*stack));
    stack->space.bp = memory_cache_allocate(sizeof(void *) * STACK_SIZE_DFT);
    stack->space.sp = stack->space.bp;
    stack->space.dim = STACK_SIZE_DFT;

    return stack;
}

void
array_stack_destroy(s_array_stack_t **stack)
{
    if (NULL_PTR_P(stack)) {
        return;
    } else if (array_stack_structure_legal_ip(*stack)) {
        memory_cache_free((*stack)->space.bp);
        memory_cache_free(*stack);
        *stack = NULL;
    }
}

static inline void
array_stack_resize_i(s_array_stack_t *stack, uint32 dim)
{
    uint32 size;

    assert_exit(!complain_zero_size_p(dim));
    assert_exit(array_stack_structure_legal_ip(stack));

    size = array_stack_size_i(stack);
    if (size > dim) {
        pr_log_warn("Stack overflow, will truncate to the top of stack.\n");
        size = dim;
    }

    stack->space.bp = memory_cache_re_allocate(stack->space.bp,
        sizeof(void *) * dim);
    stack->space.sp = stack->space.bp + size;
    stack->space.dim = dim;
}

void
array_stack_resize(s_array_stack_t *stack, uint32 dim)
{
    if (array_stack_structure_legal_ip(stack)) {
        if (0 == dim) {
            pr_log_info("Expanding size not specified, use default.\n");
            dim = stack->space.dim * 2 + STACK_EXPD_SIZE_MIN;
        }
        array_stack_resize_i(stack, dim);
    }
}

static inline bool
array_stack_full_ip(s_array_stack_t *stack)
{
    assert_exit(array_stack_structure_legal_ip(stack));

    return stack->space.dim == array_stack_size_i(stack) ? true : false;
}

bool
array_stack_full_p(s_array_stack_t *stack)
{
    if (!array_stack_structure_legal_ip(stack)) {
        return true;
    } else {
        return array_stack_full_ip(stack);
    }
}

uint32
array_stack_capacity(s_array_stack_t *stack)
{
    if (!array_stack_structure_legal_ip(stack)) {
        return STACK_SIZE_INVALID;
    } else {
        return stack->space.dim;
    }
}

static inline uint32
array_stack_rest_i(s_array_stack_t *stack)
{
    assert_exit(array_stack_structure_legal_ip(stack));

    return stack->space.dim - array_stack_size_i(stack);
}

uint32
array_stack_rest(s_array_stack_t *stack)
{
    if (!array_stack_structure_legal_ip(stack)) {
        return STACK_SIZE_INVALID;
    } else {
        return array_stack_rest_i(stack);
    }
}

void
array_stack_push(s_array_stack_t *stack, void *member)
{
    uint32 dim;

    if (array_stack_structure_legal_ip(stack)) {
        if (array_stack_full_ip(stack)) {
            pr_log_info("Stack is full, expand stack with default size.\n");
            dim = stack->space.dim * 2 + STACK_EXPD_SIZE_MIN;
            array_stack_resize_i(stack, dim);
        }

        *stack->space.sp++ = member;
    }
}

void *
array_stack_top(s_array_stack_t *stack)
{
    if (!array_stack_structure_legal_ip(stack)) {
        return PTR_INVALID;
    } else if (array_stack_empty_ip(stack)) {
        pr_log_warn("Attempt to pop from _EMPTY_ stack.\n");
        return PTR_INVALID;
    } else {
        return *(stack->space.sp - 1);
    }
}

void *
array_stack_pop(s_array_stack_t *stack)
{
    if (!array_stack_structure_legal_ip(stack)) {
        return PTR_INVALID;
    } else if (array_stack_empty_ip(stack)) {
        pr_log_warn("Attempt to pop from _EMPTY_ stack.\n");
        return PTR_INVALID;
    } else {
        return *(--stack->space.sp);
    }
}

static inline bool
array_stack_empty_ip(s_array_stack_t *stack)
{
    assert_exit(array_stack_structure_legal_ip(stack));

    return stack->space.bp == stack->space.sp ? true : false;
}

bool
array_stack_empty_p(s_array_stack_t *stack)
{
    if (!array_stack_structure_legal_ip(stack)) {
        return false;
    } else {
        return array_stack_empty_ip(stack);
    }
}

void
array_stack_cleanup(s_array_stack_t *stack)
{
    if (array_stack_structure_legal_ip(stack)) {
        dp_memset(stack->space.bp, 0, sizeof(void *) * stack->space.dim);
        stack->space.sp = stack->space.bp;
    }
}

void
array_stack_iterate(s_array_stack_t *stack, void (*handler)(void *))
{
    void **i;

    if (array_stack_structure_legal_ip(stack)
        && !NULL_PTR_P(handler)) {
        /* iterate from sp to bp */
        i = stack->space.sp;
        while(i != stack->space.bp) {
            handler(*(--i));
        }
    }
}

static inline void
linked_stack_space_initial(s_linked_stack_space_t *space, uint32 dim)
{
    assert_exit(!NULL_PTR_P(space));
    assert_exit(!complain_zero_size_p(dim));

    space->space.bp = memory_cache_allocate(sizeof(void *) * dim);
    space->space.sp = space->space.bp;
    space->space.dim = dim;
}

static inline s_linked_stack_space_t *
linked_stack_space_create(uint32 dim)
{
    s_linked_stack_space_t *space;

    assert_exit(!complain_zero_size_p(dim));

    space = memory_cache_allocate(sizeof(*space));
    doubly_linked_list_initial(&space->link);
    linked_stack_space_initial(space, dim);

    return space;
}

static inline bool
linked_stack_space_structure_legal_p(s_linked_stack_space_t *space)
{
    if (NULL_PTR_P(space)) {
        return false;
    } else if (!doubly_linked_list_structure_legal_p(&space->link)) {
        return false;
    } else if (!array_stack_space_structure_legal_p(&space->space)) {
        return false;
    } else {
        return true;
    }
}

bool
linked_stack_structure_legal_p(s_linked_stack_t *stack)
{
    return linked_stack_structure_legal_ip(stack);
}

static inline bool
linked_stack_structure_legal_ip(s_linked_stack_t *stack)
{
    if (NULL_PTR_P(stack)) {
        return false;
    } else if (NULL_PTR_P(stack->base)) {
        return false;
    } else if (NULL_PTR_P(stack->top)) {
        return false;
    } else if (!linked_stack_space_structure_legal_p(stack->base)) {
        return false;
    } else if (!linked_stack_space_structure_legal_p(stack->top)) {
        return false;
    } else {
        return true;
    }
}

s_linked_stack_t *
linked_stack_create(void)
{
    s_linked_stack_t *stack;

    stack = memory_cache_allocate(sizeof(*stack));
    stack->base = linked_stack_space_create(STACK_SIZE_DFT);
    stack->top = stack->base;

    return stack;
}

void
linked_stack_destroy(s_linked_stack_t **stack)
{
    s_linked_stack_space_t *node;

    if (NULL_PTR_P(stack)) {
        return;
    } else if (linked_stack_structure_legal_ip(*stack)) {
        node = (*stack)->base;
        while (node) {
            node = linked_stack_space_remove(node);
        }

        memory_cache_free(*stack);
        *stack = NULL;
    }
}

static inline s_linked_stack_space_t *
linked_stack_ptr_to_space(s_doubly_linked_list_t *link)
{
    assert_exit(doubly_linked_list_structure_legal_p(link));

    return CONTAINER_OF(link, s_linked_stack_space_t, link);
}

static inline s_linked_stack_space_t *
linked_stack_space_next(s_linked_stack_space_t *node)
{
    assert_exit(linked_stack_space_structure_legal_p(node));

    return linked_stack_ptr_to_space(node->link.next);
}

static inline s_linked_stack_space_t *
linked_stack_space_previous(s_linked_stack_space_t *node)
{
    assert_exit(linked_stack_space_structure_legal_p(node));

    return linked_stack_ptr_to_space(node->link.previous);
}

static inline s_linked_stack_space_t *
linked_stack_space_remove(s_linked_stack_space_t *node)
{
    s_doubly_linked_list_t *link;

    assert_exit(linked_stack_space_structure_legal_p(node));

    link = &node->link;

    doubly_linked_list_remove(&link);
    memory_cache_free(node->space.bp);
    memory_cache_free(node);

    if (NULL == link) {
        /* last node of linked stack */
        return NULL;
    } else {
        return linked_stack_ptr_to_space(link);
    }
}

static inline void
linked_stack_resize_i(s_linked_stack_t *stack, uint32 dim)
{
    uint32 capacity;
    uint32 node_capacity;
    s_linked_stack_space_t *node;
    s_linked_stack_space_t *last;

    assert_exit(!complain_zero_size_p(dim));
    assert_exit(linked_stack_structure_legal_ip(stack));

    capacity = linked_stack_capacity_i(stack);
    last = linked_stack_space_previous(stack->base);

    if (capacity < dim) {
        dim = dim - capacity;
        node = linked_stack_space_create(dim);
        doubly_linked_list_insert_after(&last->link, &node->link);
    } else if (capacity > dim) {
        node_capacity = last->space.dim;
        while (capacity - node_capacity >= dim) {
            capacity -= node_capacity;
            node = linked_stack_space_previous(last);
            node_capacity = node->space.dim;

            linked_stack_space_remove(last);
            last = node;
        }

        dim = dim - (capacity - node_capacity);
        last->space.bp = memory_cache_re_allocate(last->space.bp,
            sizeof(void *) * dim);
        last->space.sp = last->space.bp;
        last->space.dim = dim;
    }
}

void
linked_stack_resize(s_linked_stack_t *stack, uint32 dim)
{
    if (linked_stack_structure_legal_ip(stack)) {
        if (0 == dim) {
            pr_log_info("Expanding size not specified, use default.\n");
            dim = linked_stack_capacity_i(stack) * 2 + STACK_EXPD_SIZE_MIN;
        }

        linked_stack_resize_i(stack, dim);
    }
}

static inline bool
linked_stack_full_ip(s_linked_stack_t *stack)
{
    assert_exit(linked_stack_structure_legal_ip(stack));

    if (0 == linked_stack_rest_i(stack)) {
        return true;
    } else {
        return false;
    }
}

bool
linked_stack_full_p(s_linked_stack_t *stack)
{
    if (!linked_stack_structure_legal_ip(stack)) {
        return true;
    } else {
        return linked_stack_full_ip(stack);
    }
}

static inline uint32
linked_stack_rest_i(s_linked_stack_t *stack)
{
    uint32 rest;
    struct linked_stack_space *next;

    assert_exit(linked_stack_structure_legal_ip(stack));

    rest = linked_stack_space_rest(stack->top);
    next = linked_stack_space_next(stack->top);

    while (stack->base != next) {
        rest += next->space.dim;
        next = linked_stack_space_next(next);
    }

    return rest;
}

uint32
linked_stack_rest(s_linked_stack_t *stack)
{
    if (!linked_stack_structure_legal_ip(stack)) {
        return STACK_SIZE_INVALID;
    } else {
        return linked_stack_rest_i(stack);
    }
}

static inline uint32
linked_stack_capacity_i(s_linked_stack_t *stack)
{
    uint32 capacity;
    s_linked_stack_space_t *space;

    assert_exit(linked_stack_structure_legal_ip(stack));

    capacity = 0u;
    space = stack->base;

    do {
        capacity += space->space.dim;
        space = linked_stack_space_next(space);
    } while (space != stack->base);

    return capacity;
}

uint32
linked_stack_capacity(s_linked_stack_t *stack)
{
    if (!linked_stack_structure_legal_ip(stack)) {
        return STACK_SIZE_INVALID;
    } else {
        return linked_stack_capacity_i(stack);
    }
}

static inline bool
linked_stack_space_empty_p(s_linked_stack_space_t *space)
{
    assert_exit(linked_stack_space_structure_legal_p(space));

    return space->space.dim == linked_stack_space_rest(space) ? true : false;
}

static inline bool
linked_stack_space_full_p(s_linked_stack_space_t *space)
{
    assert_exit(linked_stack_space_structure_legal_p(space));

    return 0u == linked_stack_space_rest(space) ? true : false;
}

static inline uint32
linked_stack_space_rest(s_linked_stack_space_t *space)
{
    void **tmp;
    void **limit;

    assert_exit(linked_stack_space_structure_legal_p(space));

    tmp = space->space.sp;
    limit = space->space.bp + space->space.dim;
    if ((ptr_t)tmp > (ptr_t)limit) {
        pr_log_err("Array stack overflow.");
    }

    return (uint32)(limit - tmp);
}

void
linked_stack_push(s_linked_stack_t *stack, void *member)
{
    uint32 dim;

    if (linked_stack_structure_legal_ip(stack)) {
        if (linked_stack_full_ip(stack)) {
            pr_log_info("Stack is full, expand stack with default size.\n");
            dim = linked_stack_capacity_i(stack) * 2 + STACK_EXPD_SIZE_MIN;
            linked_stack_resize_i(stack, dim);
        }

        if (linked_stack_space_full_p(stack->top)) {
            pr_log_info("Stack node is full, will move to next node.\n");
            stack->top = linked_stack_space_next(stack->top);
        }

        *stack->top->space.sp++ = member;
    }
}

void *
linked_stack_pop(s_linked_stack_t *stack)
{
    if (!linked_stack_structure_legal_ip(stack)) {
        return PTR_INVALID;
    } else if (linked_stack_empty_ip(stack)) {
        pr_log_warn("Attempt to pop from _EMPTY_ stack.\n");
        return PTR_INVALID;
    } else {
        if (linked_stack_space_empty_p(stack->top)) {
            pr_log_info("Stack node is empty, will move to previous node.\n");
            stack->top = linked_stack_space_previous(stack->top);
        }

        return *(--stack->top->space.sp);
    }
}

void *
linked_stack_top(s_linked_stack_t *stack)
{
    if (!linked_stack_structure_legal_ip(stack)) {
        return PTR_INVALID;
    } else if (linked_stack_empty_ip(stack)) {
        pr_log_warn("Attempt to pop from _EMPTY_ stack.\n");
        return PTR_INVALID;
    } else {
        if (linked_stack_space_empty_p(stack->top)) {
            pr_log_info("Stack node is empty, will move to previous node.\n");
            stack->top = linked_stack_space_previous(stack->top);
        }

        return *(stack->top->space.sp);
    }
}

static inline bool
linked_stack_empty_ip(s_linked_stack_t *stack)
{
    assert_exit(linked_stack_structure_legal_ip(stack));

    if (stack->base != stack->top) {
        return false;
    } else if (stack->top->space.dim == linked_stack_space_rest(stack->top)) {
        return true;
    } else {
        return false;
    }
}

bool
linked_stack_empty_p(s_linked_stack_t *stack)
{
    if (!linked_stack_structure_legal_ip(stack)) {
        return false;
    } else {
        return linked_stack_empty_ip(stack);
    }
}

void
linked_stack_cleanup(s_linked_stack_t *stack)
{
    s_linked_stack_space_t *space;

    if (linked_stack_structure_legal_ip(stack)) {
        stack->top = stack->base;
        space = stack->base;

        do {
            memset(space->space.bp, 0, sizeof(void *) * space->space.dim);
            space->space.sp = space->space.bp;
            space = linked_stack_space_next(space);
        } while (space != stack->base);
    }
}

void
linked_stack_iterate(s_linked_stack_t *stack, void (*handler)(void *))
{
    s_linked_stack_space_t *space;
    s_linked_stack_space_t *space_head;

    if (NULL_PTR_P(handler)) {
        return;
    } else if (linked_stack_structure_legal_ip(stack)) {
        space = space_head = stack->top;

        do {
            linked_stack_space_iterate(space, handler);
            space = linked_stack_space_next(space);
        } while (space_head != space);
    }
}

static inline void
linked_stack_space_iterate(s_linked_stack_space_t *space,
    void (*handler)(void *))
{
    void **ptr;

    assert_exit(!NULL_PTR_P(handler));
    assert_exit(linked_stack_space_structure_legal_p(space));

    ptr = space->space.sp;
    while(ptr != space->space.bp) {
        handler(*(--ptr));
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

void
binary_search_tree_nice_set(s_binary_search_tree_t *tree, sint64 nice)
{
    if (binary_search_tree_structure_legal_p(tree)) {
        tree->nice = nice;
    }
}

sint64
binary_search_tree_nice(s_binary_search_tree_t *tree)
{
    if (binary_search_tree_structure_legal_p(tree)) {
        return tree->nice;
    } else {
        return TREE_NICE_INVALID;
    }
}

s_binary_search_tree_t *
binary_search_tree_left(s_binary_search_tree_t *tree)
{
    if (binary_search_tree_structure_legal_p(tree)) {
        return tree->left;
    } else {
        return PTR_INVALID;
    }
}

s_binary_search_tree_t *
binary_search_tree_right(s_binary_search_tree_t *tree)
{
    if (binary_search_tree_structure_legal_p(tree)) {
        return tree->right;
    } else {
        return PTR_INVALID;
    }
}

s_binary_search_tree_t *
binary_search_tree_create(void)
{
    s_binary_search_tree_t *tree;

    tree = memory_cache_allocate(sizeof(*tree));
    binary_search_tree_initial_i(tree, 0);

    return tree;
}

static inline void
binary_search_tree_initial_i(s_binary_search_tree_t *tree, sint64 nice)
{
    assert_exit(!NULL_PTR_P(tree));

    tree->nice = nice;
    tree->left = tree->right = NULL;

    assert_exit(binary_search_tree_structure_legal_p(tree));
}

void
binary_search_tree_initial(s_binary_search_tree_t *tree, sint64 nice)
{
    if (!complain_null_pointer_p(tree)) {
        binary_search_tree_initial_i(tree, nice);
    }
}

static inline bool
binary_search_tree_structure_legal_p(s_binary_search_tree_t *tree)
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
binary_search_tree_destroy_i(s_binary_search_tree_t *tree)
{
    s_array_queue_t *queue;
    s_binary_search_tree_t *binary_node;

    assert_exit(binary_search_tree_structure_legal_p(tree));

    queue = array_queue_create();
    array_queue_enter(queue, tree);

    while (!array_queue_empty_p(queue)) {
        binary_node = array_queue_leave(queue);

        if (binary_node->left != NULL) {
            array_queue_enter(queue, binary_node->left);
        }

        if (binary_node->right != NULL) {
            array_queue_enter(queue, binary_node->right);
        }

        memory_cache_free(binary_node);
    }

    array_queue_destroy(&queue);
}

void
binary_search_tree_destroy(s_binary_search_tree_t **tree)
{
    if (NULL_PTR_P(tree)) {
        return;
    } else if (binary_search_tree_structure_legal_p(*tree)) {
        binary_search_tree_destroy_i(*tree);
        *tree = NULL;
    }
}

static inline s_binary_search_tree_t *
binary_search_tree_find_i(s_binary_search_tree_t *tree, sint64 nice)
{
    sint64 node_nice;

    assert_exit(binary_search_tree_structure_legal_p(tree));

    while (tree) {
        node_nice = tree->nice;
        if (nice > node_nice) {
            tree = tree->right;
        } else if (nice < node_nice) {
            tree = tree->left;
        } else {
            return tree;
        }
    }

    return NULL;
}

s_binary_search_tree_t *
binary_search_tree_find(s_binary_search_tree_t *tree, sint64 nice)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return PTR_INVALID;
    } else {
        return binary_search_tree_find_i(tree, nice);
    }
}

static inline s_binary_search_tree_t *
binary_search_tree_find_min_i(s_binary_search_tree_t *tree)
{
    assert_exit(binary_search_tree_structure_legal_p(tree));

    while (tree->left) {
        tree = tree->left;
    }

    return tree;
}

s_binary_search_tree_t  *
binary_search_tree_find_min(s_binary_search_tree_t *tree)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return PTR_INVALID;
    } else {
        return binary_search_tree_find_min_i(tree);
    }
}

static inline s_binary_search_tree_t *
binary_search_tree_find_max_i(s_binary_search_tree_t *tree)
{
    assert_exit(binary_search_tree_structure_legal_p(tree));

    while (tree->right) {
        tree = tree->right;
    }

    return tree;
}

s_binary_search_tree_t *
binary_search_tree_find_max(s_binary_search_tree_t *tree)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return PTR_INVALID;
    } else {
        return binary_search_tree_find_max_i(tree);
    }
}

static inline sint32
binary_search_tree_height_i(s_binary_search_tree_t *tree)
{
    sint32 left;
    sint32 right;

    if (!tree) {
        return -1;
    } else {
        left = binary_search_tree_height_i(tree->left);
        right = binary_search_tree_height_i(tree->right);

        return MAX_S32(left, right) + 1;
    }
}

sint32
binary_search_tree_height(s_binary_search_tree_t *tree)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return -1;
    } else {
        return binary_search_tree_height_i(tree);
    }
}

static inline bool
binary_search_tree_contains_p_i(s_binary_search_tree_t *tree,
    s_binary_search_tree_t *node)
{
    bool retval;
    s_binary_search_tree_t *left, *right;

    assert_exit(binary_search_tree_structure_legal_p(tree));
    assert_exit(binary_search_tree_structure_legal_p(node));

    retval = false;

    while (tree) {
        if (node == tree) {
            return true;
        } else if (node->nice > tree->nice) {
            tree = tree->right;
        } else if (node->nice < tree->nice) {
            tree = tree->left;
        } else {
            left = tree->left;
            right = tree->right;

            if (left && node->nice == left->nice) { // Handle repeated nice
                retval = binary_search_tree_contains_p_i(left, node);
            }
            if (!retval && right && node->nice == right->nice) {
                retval = binary_search_tree_contains_p_i(right, node);
            }

            return retval;
        }
    }

    return false;
}

bool
binary_search_tree_contains_p(s_binary_search_tree_t *tree,
    s_binary_search_tree_t *node)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return false;
    } else if (!binary_search_tree_structure_legal_p(node)) {
        return false;
    } else {
        return binary_search_tree_contains_p_i(tree, node);
    }
}

static inline s_binary_search_tree_t *
binary_search_tree_insert_i(s_binary_search_tree_t *tree,
    s_binary_search_tree_t *node)
{
    sint64 nice;
    sint32 path_direction;
    s_binary_search_tree_t *binary_node;
    s_binary_search_tree_t **iterator_node;

    assert_exit(binary_search_tree_structure_legal_p(tree));
    assert_exit(binary_search_tree_ordered_p(tree));
    assert_exit(binary_search_tree_structure_legal_p(node));

    nice = node->nice;
    path_direction = 0;
    iterator_node = &tree;

    while (*iterator_node) {
        binary_node = *iterator_node;
        if (nice < binary_node->nice) {
            path_direction--;
            iterator_node = &binary_node->left;
        } else if (nice > binary_node->nice) {
            path_direction++;
            iterator_node = &binary_node->right;
        } else if (binary_node == node) {
            pr_log_warn("Insert node exist, nothing will be done.\n");
            return NULL;
        } else {
            if (path_direction > 0) { // If right is heavy, inserted to left.
                node->left = binary_node->left;
                binary_node->left = node;
                node->right = NULL;
            } else {
                node->right = binary_node->right;
                binary_node->right = node;
                node->left = NULL;
            }

            assert_exit(binary_search_tree_ordered_p(tree));
            return node;
        }
    }

    *iterator_node = node;

    assert_exit(binary_search_tree_ordered_p(tree));
    return node;
}

s_binary_search_tree_t *
binary_search_tree_insert(s_binary_search_tree_t *tree,
    s_binary_search_tree_t *node)
{
    if (!binary_search_tree_structure_legal_p(tree)) {
        return PTR_INVALID;
    } else if (!binary_search_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return binary_search_tree_insert_i(tree, node);
    }
}

static inline bool
binary_search_tree_doubly_child_p(s_binary_search_tree_t *node)
{
    assert_exit(binary_search_tree_structure_legal_p(node));

    if (node->left && node->right) {
        return true;
    } else {
        return false;
    }
}

static inline s_binary_search_tree_t **
binary_search_tree_find_ptr_to_max(s_binary_search_tree_t **tree)
{
    s_binary_search_tree_t **max;
    s_binary_search_tree_t *node;

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

static inline s_binary_search_tree_t **
binary_search_tree_find_ptr_to_min(s_binary_search_tree_t **tree)
{
    s_binary_search_tree_t **min;
    s_binary_search_tree_t *node;

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
binary_search_tree_swap_child(s_binary_search_tree_t *a,
    s_binary_search_tree_t *b)
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
binary_search_tree_lt_doubly_child_strip(s_binary_search_tree_t **binary_node)
{
    s_binary_search_tree_t *node_strip;

    assert_exit(!complain_null_pointer_p(binary_node));
    assert_exit(binary_search_tree_structure_legal_p(*binary_node));
    assert_exit(!binary_search_tree_doubly_child_p(*binary_node));

    node_strip = *binary_node;

    if (node_strip->left != NULL) {
        *binary_node = node_strip->left;
    } else {
        *binary_node = node_strip->right;
    }

    node_strip->left = node_strip->right = NULL;
}

static inline void
binary_search_tree_child_strip_from_max(s_binary_search_tree_t **binary_node)
{
    s_binary_search_tree_t *binary;
    s_binary_search_tree_t *max;
    s_binary_search_tree_t **max_node;

    assert_exit(!complain_null_pointer_p(binary_node));
    assert_exit(binary_search_tree_structure_legal_p(*binary_node));
    assert_exit(binary_search_tree_doubly_child_p(*binary_node));

    binary = *binary_node;

    if (!binary->left->right) { // short cut here
        *binary_node = binary->left;
        binary->left->right = binary->right;
        binary->left = binary->right = NULL;
    } else {
        max_node = binary_search_tree_find_ptr_to_max(&binary->left);
        max = *max_node;

        binary_search_tree_swap_child(binary, max);
        *max_node = binary;
        *binary_node = max;
        binary_search_tree_lt_doubly_child_strip(max_node);
    }
}

static inline void
binary_search_tree_child_strip_from_min(s_binary_search_tree_t **binary_node)
{
    s_binary_search_tree_t *binary;
    s_binary_search_tree_t *min;
    s_binary_search_tree_t **min_node;

    assert_exit(!complain_null_pointer_p(binary_node));
    assert_exit(binary_search_tree_structure_legal_p(*binary_node));
    assert_exit(binary_search_tree_doubly_child_p(*binary_node));

    binary = *binary_node;

    if (!binary->right->left) { // short cut here
        *binary_node = binary->right;
        binary->right->left = binary->left;
        binary->left = binary->right = NULL;
    } else {
        min_node = binary_search_tree_find_ptr_to_min(&binary->right);
        min = *min_node;

        binary_search_tree_swap_child(binary, min);
        *min_node = binary;
        *binary_node = min;

        binary_search_tree_lt_doubly_child_strip(min_node);
    }
}

static inline void
binary_search_tree_doubly_child_strip(s_binary_search_tree_t **binary_node,
    sint32 direction)
{
    assert_exit(!complain_null_pointer_p(binary_node));
    assert_exit(binary_search_tree_structure_legal_p(*binary_node));
    assert_exit(binary_search_tree_doubly_child_p(*binary_node));

    if (direction > 0) {
        binary_search_tree_child_strip_from_max(binary_node);
    } else {
        binary_search_tree_child_strip_from_min(binary_node);
    }
}

static inline void
binary_search_tree_child_strip(s_binary_search_tree_t **binary_node,
    sint32 direction)
{
    assert_exit(!complain_null_pointer_p(binary_node));
    assert_exit(binary_search_tree_structure_legal_p(*binary_node));

    if (binary_search_tree_doubly_child_p(*binary_node)) {
        binary_search_tree_doubly_child_strip(binary_node, direction);
    } else {
        binary_search_tree_lt_doubly_child_strip(binary_node);
    }
}

static inline s_binary_search_tree_t *
binary_search_tree_repeated_remove(s_binary_search_tree_t **tree,
    s_binary_search_tree_t *node, sint32 direction)
{
    sint64 nice;
    s_array_queue_t *queue;
    s_binary_search_tree_t **iterator;
    s_binary_search_tree_t *binary_node, *removed_node;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(binary_search_tree_structure_legal_p(node));
    assert_exit(binary_search_tree_structure_legal_p(*tree));
    assert_exit(binary_search_tree_ordered_p(*tree));
    assert_exit((*tree)->nice == node->nice);
    assert_exit(*tree != node);

    iterator = tree;
    removed_node = NULL;

    nice = node->nice;
    queue = array_queue_create();
    array_queue_enter(queue, iterator);

    while (!array_queue_empty_p(queue)) {
         iterator = array_queue_leave(queue);
         binary_node = *iterator;
         if (binary_node == node) {
             removed_node = node;
             binary_search_tree_child_strip(iterator, direction);
             break;
         } else if (binary_node->left && binary_node->left->nice == nice) {
             direction--;
             array_queue_enter(queue, &binary_node->left);
         } else if (binary_node->right && binary_node->right->nice == nice) {
             direction++;
             array_queue_enter(queue, &binary_node->right);
         }
    }

    array_queue_destroy(&queue);
    return removed_node;
}

static inline s_binary_search_tree_t *
binary_search_tree_remove_i(s_binary_search_tree_t **tree,
    s_binary_search_tree_t *node)
{
    sint32 direction;
    s_binary_search_tree_t **iterator;
    s_binary_search_tree_t *binary_node, *removed_node;

    assert_exit(!complain_null_pointer_p(tree));
    assert_exit(binary_search_tree_structure_legal_p(*tree));
    assert_exit(binary_search_tree_ordered_p(*tree));
    assert_exit(binary_search_tree_structure_legal_p(node));

    direction = 0;
    removed_node = NULL;
    iterator = tree;
    binary_node = *tree;

    while (binary_node) {
        if (node->nice > binary_node->nice) {
            direction++;
            iterator = &binary_node->right;
        } else if (node->nice < binary_node->nice) {
            direction--;
            iterator = &binary_node->left;
        } else if (node == binary_node) {
            removed_node = node;
            binary_search_tree_child_strip(iterator, direction);
            break;
        } else {
            removed_node = binary_search_tree_repeated_remove(iterator, node, direction);
            break;
        }
        binary_node = *iterator;
    }

    if (removed_node == NULL) {
        pr_log_warn("Failed to find the node in given tree.\n");
    }

    assert_exit(binary_search_tree_ordered_p(*tree));
    return removed_node;
}

s_binary_search_tree_t *
binary_search_tree_remove(s_binary_search_tree_t **tree,
    s_binary_search_tree_t *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!binary_search_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!binary_search_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return binary_search_tree_remove_i(tree, node);
    }
}

static inline void
binary_search_tree_iterate_i(s_binary_search_tree_t *tree, void (*handler)(void *))
{
    s_array_queue_t *queue_slave;
    s_array_queue_t *queue_master;
    s_binary_search_tree_t *binary_node;

    assert_exit(!complain_null_pointer_p(handler));
    assert_exit(binary_search_tree_structure_legal_p(tree));

    queue_slave = array_queue_create();
    queue_master = array_queue_create();
    array_queue_enter(queue_master, tree);

    while (!array_queue_empty_p(queue_master)) {
        while (!array_queue_empty_p(queue_master)) {
            binary_node = array_queue_leave(queue_master);
            handler(binary_node);

            if (binary_node->left) {
                array_queue_enter(queue_slave, binary_node->left);
            }
            if (binary_node->right) {
                array_queue_enter(queue_slave, binary_node->right);
            }
        }
        swap_pointer((void **)&queue_master, (void **)&queue_slave);
    }

    array_queue_destroy(&queue_slave);
    array_queue_destroy(&queue_master);
}

void
binary_search_tree_iterate(s_binary_search_tree_t *tree, void (*handler)(void *))
{
    if (NULL_PTR_P(handler)) {
        return;
    } else if (!binary_search_tree_structure_legal_p(tree)) {
        return;
    } else {
        binary_search_tree_iterate_i(tree, handler);
    }
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
s_binary_indexed_tree_t *
binary_indexed_tree_create(sint64 *data, uint32 size)
{
    uint32 i;
    uint32 mask;
    uint32 iterator;
    s_binary_indexed_tree_t *tree;

    if (NULL_PTR_P(data)) {
        return NULL;
    } else if (complain_zero_size_p(size)) {
        return NULL;
    } else {
        tree = memory_cache_allocate(sizeof(*tree));

        tree->size = size;
        tree->data = memory_cache_allocate(sizeof(*tree->data) * (size + 1));
        dp_memset(tree->data, 0, sizeof(*tree->data) * (size + 1));

        tree->data[1] = data[0];

        i = 2;
        while (i <= size) {
            tree->data[i] = data[i - 1];
            if (0 == i % 2) {
                iterator = i - 1;
                mask = 0xfffffffe;

                while (0 == (i & ~mask)) {
                    tree->data[i] += tree->data[iterator];
                    iterator = iterator & mask;
                    mask = mask << 1;
                }
            }
            i++;
        }

        return tree;
    }
}

void
binary_indexed_tree_destroy(s_binary_indexed_tree_t **tree)
{
    if (NULL_PTR_P(tree)) {
        return;
    } else if (binary_indexed_tree_structure_legal_p(*tree)) {
        memory_cache_free((*tree)->data);
        memory_cache_free(*tree);
        *tree = NULL;
    }
}

static inline bool
binary_indexed_tree_number_legal_p(s_binary_indexed_tree_t *tree, uint32 number)
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
binary_indexed_tree_structure_legal_p(s_binary_indexed_tree_t *tree)
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
binary_indexed_tree_add_i(s_binary_indexed_tree_t *tree,
    uint32 number, sint64 val)
{
    uint32 iterator;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(binary_indexed_tree_number_legal_p(tree, number));
    assert_exit(binary_indexed_tree_structure_legal_p(tree));

    iterator = ((number - 1) ^ number) & number;
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
    while (number + iterator <= tree->size) {
        number = number + iterator;
        tree->data[number] += val;
        iterator = iterator << 1;
    }
}


void
binary_indexed_tree_add(s_binary_indexed_tree_t *tree,
    uint32 number, sint64 val)
{

    if (!binary_indexed_tree_structure_legal_p(tree)) {
        return;
    } else if (!binary_indexed_tree_number_legal_p(tree, number)) {
        return;
    } else {
        binary_indexed_tree_add_i(tree, number, val);
    }
}

void
binary_indexed_tree_sub(s_binary_indexed_tree_t *tree,
    uint32 number, sint64 val)
{
    if (!binary_indexed_tree_structure_legal_p(tree)) {
        return;
    } else if (!binary_indexed_tree_number_legal_p(tree, number)) {
        return;
    } else {
        binary_indexed_tree_add_i(tree, number, -val);
    }
}

static inline sint64
binary_indexed_tree_sum_i(s_binary_indexed_tree_t *tree,
    uint32 number)
{
    sint64 sum;
    uint32 iterator;

    assert_exit(binary_indexed_tree_structure_legal_p(tree));
    assert_exit(binary_indexed_tree_number_legal_p(tree, number));

    sum = 0;
    iterator = number;

    while (iterator) {
        sum += tree->data[iterator];
        iterator = iterator & (iterator - 1);
    }

    return sum;
}

sint64
binary_indexed_tree_sum(s_binary_indexed_tree_t *tree, uint32 number)
{
    if (!binary_indexed_tree_structure_legal_p(tree)) {
        return BIN_IDXED_SUM_INVALID;
    } else if (!binary_indexed_tree_number_legal_p(tree, number)) {
        return BIN_IDXED_SUM_INVALID;
    } else {
        return binary_indexed_tree_sum_i(tree, number);
    }
}

sint64
binary_indexed_tree_range_sum(s_binary_indexed_tree_t *tree,
    uint32 nmbr_s, uint32 nmbr_e)
{
    sint64 sum_head;
    sint64 sum_range;

    if (!binary_indexed_tree_structure_legal_p(tree)) {
        return BIN_IDXED_SUM_INVALID;
    } else if (!binary_indexed_tree_number_legal_p(tree, nmbr_s)) {
        return BIN_IDXED_SUM_INVALID;
    } else if (!binary_indexed_tree_number_legal_p(tree, nmbr_e)) {
        return BIN_IDXED_SUM_INVALID;
    } else if (nmbr_s > nmbr_e) {
        pr_log_warn("Invalid start number and end number of range.\n");
        return BIN_IDXED_SUM_INVALID;
    } else if (nmbr_s == 1) {
        return binary_indexed_tree_sum_i(tree, nmbr_e);
    } else {
        sum_head = binary_indexed_tree_sum_i(tree, nmbr_s - 1);
        sum_range = binary_indexed_tree_sum_i(tree, nmbr_e) - sum_head;
        return sum_range;
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

sint64
splay_tree_nice(s_splay_tree_t *tree)
{
    if (splay_tree_structure_legal_p(tree)) {
        return tree->nice;
    } else {
        return TREE_NICE_INVALID;
    }
}

s_splay_tree_t *
splay_tree_left(s_splay_tree_t *tree)
{
    if (splay_tree_structure_legal_p(tree)) {
        return tree->left;
    } else {
        return PTR_INVALID;
    }
}

s_splay_tree_t *
splay_tree_right(s_splay_tree_t *tree)
{
    if (splay_tree_structure_legal_p(tree)) {
        return tree->right;
    } else {
        return PTR_INVALID;
    }
}

static inline bool
splay_tree_structure_legal_p(s_splay_tree_t *tree)
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
splay_tree_initial_i(s_splay_tree_t *tree, sint64 nice)
{
    assert_exit(!complain_null_pointer_p(tree));

    tree->nice = nice;
    tree->left = tree->right = NULL;

    assert_exit(splay_tree_structure_legal_p(tree));
}

s_splay_tree_t *
splay_tree_create(void)
{
    s_splay_tree_t *tree;

    tree = memory_cache_allocate(sizeof(*tree));
    splay_tree_initial_i(tree, 0);

    return tree;
}

void
splay_tree_initial(s_splay_tree_t *tree, sint64 nice)
{
    if (!complain_null_pointer_p(tree)) {
        splay_tree_initial_i(tree, nice);
    }
}

static inline void
splay_tree_destroy_i(s_splay_tree_t *tree)
{
    s_array_queue_t *queue;
    s_splay_tree_t *splay_node;

    assert_exit(splay_tree_structure_legal_p(tree));

    queue = array_queue_create();
    array_queue_enter(queue, tree);

    while (!array_queue_empty_p(queue)) {
        splay_node = array_queue_leave(queue);

        if (splay_node->left != NULL) {
            array_queue_enter(queue, splay_node->left);
        }
        if (splay_node->right != NULL) {
            array_queue_enter(queue, splay_node->right);
        }

        memory_cache_free(splay_node);
    }

    array_queue_destroy(&queue);
}

void
splay_tree_destroy(s_splay_tree_t **tree)
{
    if (NULL_PTR_P(tree)) {
        return;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return;
    } else {
        splay_tree_destroy_i(*tree);
        *tree = NULL;
    }
}

static inline s_splay_tree_t *
splay_tree_find_i(s_splay_tree_t **tree, sint64 nice)
{
    s_splay_tree_t **iterator;
    s_splay_tree_t *splay_node;
    s_array_stack_t *path_stack;

    assert_exit(!complain_null_pointer_p(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));

    iterator = tree;
    splay_node = *tree;
    path_stack = array_stack_create();

    while (splay_node) {
        if (nice < splay_node->nice) {
            array_stack_push(path_stack, TREE_PATH_L_ENCODE(iterator));
            iterator = &splay_node->left;
        } else if (nice > splay_node->nice) {
            array_stack_push(path_stack, TREE_PATH_R_ENCODE(iterator));
            iterator = &splay_node->right;
        } else {
            splay_tree_balance_splaying(path_stack);
            array_stack_destroy(&path_stack);

            assert_exit(*tree = splay_node);
            return splay_node;
        }
        splay_node = *iterator;
    }

    array_stack_destroy(&path_stack);
    return NULL;
}

s_splay_tree_t *
splay_tree_find(s_splay_tree_t **tree, sint64 nice)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else {
        return splay_tree_find_i(tree, nice);
    }
}

static inline s_splay_tree_t *
splay_tree_find_min_i(s_splay_tree_t **tree)
{
    s_splay_tree_t *min_node;
    s_splay_tree_t **iterator;
    s_array_stack_t *path_stack;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));

    iterator = tree;
    min_node = *tree;
    path_stack = array_stack_create();

    while (min_node->left) {
        array_stack_push(path_stack, TREE_PATH_L_ENCODE(iterator));
        iterator = &min_node->left;
        min_node = *iterator;
    }

    splay_tree_balance_splaying(path_stack);
    array_stack_destroy(&path_stack);
    assert_exit(*tree == min_node);

    return min_node;
}

s_splay_tree_t *
splay_tree_find_min(s_splay_tree_t **tree)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else {
        return splay_tree_find_min_i(tree);
    }
}

static inline s_splay_tree_t *
splay_tree_find_max_i(s_splay_tree_t **tree)
{
    s_splay_tree_t *max_node;
    s_splay_tree_t **iterator;
    s_array_stack_t *path_stack;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));

    iterator = tree;
    max_node = *tree;
    path_stack = array_stack_create();

    while (max_node->right) {
        array_stack_push(path_stack, TREE_PATH_R_ENCODE(iterator));
        iterator = &max_node->right;
        max_node = *iterator;
    }

    splay_tree_balance_splaying(path_stack);
    array_stack_destroy(&path_stack);
    assert_exit(*tree == max_node);

    return max_node;
}

s_splay_tree_t *
splay_tree_find_max(s_splay_tree_t **tree)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else {
        return splay_tree_find_max_i(tree);
    }
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
static inline void
splay_tree_balance_splaying_left_to_left(s_splay_tree_t **tree)
{
    s_splay_tree_t *k1;
    s_splay_tree_t *k2;
    s_splay_tree_t *k3;

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
 *     k1            k3
 *    /  \          /  \
 *   k2   d  =>    k2   k1
 *  / \           / \   / \
 * a   k3        a   b c   d
 *    /  \
 *   b    c
 */
static inline void
splay_tree_balance_splaying_left_to_right(s_splay_tree_t **tree)
{
    s_splay_tree_t *k1;
    s_splay_tree_t *k2;
    s_splay_tree_t *k3;

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
static inline void
splay_tree_balance_splaying_right_to_left(s_splay_tree_t **tree)
{
    s_splay_tree_t *k1;
    s_splay_tree_t *k2;
    s_splay_tree_t *k3;

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

/*
 *     k1                 k3
 *    / \                /  \
 *   a   k2      =>     k2   d
 *      /  \           /  \
 *     b    k3        k1   c
 *         /  \      /  \
 *        c    d    a    b
 */
static inline void
splay_tree_balance_splaying_right_to_right(s_splay_tree_t **tree)
{
    s_splay_tree_t *k1;
    s_splay_tree_t *k2;
    s_splay_tree_t *k3;

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

static inline void
splay_tree_balance_splaying_i(s_splay_tree_t **tree, uint32 path_mask)
{
    assert_exit(!complain_null_pointer_p(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));

    switch (path_mask) {
        case PATH_LEFT_TO_LEFT:
            splay_tree_balance_splaying_left_to_left(tree);
            break;
        case PATH_LEFT_TO_RIGHT:
            splay_tree_balance_splaying_left_to_right(tree);
            break;
        case PATH_RIGHT_TO_LEFT:
            splay_tree_balance_splaying_right_to_left(tree);
            break;
        case PATH_RIGHT_TO_RIGHT:
            splay_tree_balance_splaying_right_to_right(tree);
            break;
        default:
            assert_not_reached("Unexpected value of enum tree_path_type.\n");
            break;
    }
}

/*
 *     k1          k2
 *    /  \        /  \
 *   k2   c  =>  a    k1
 *  /  \             /  \
 * a    b           b    c
 */
static inline void
splay_tree_balance_splaying_root_left(s_splay_tree_t **tree)
{
    s_splay_tree_t *k1;
    s_splay_tree_t *k2;

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
static inline void
splay_tree_balance_splaying_root_right(s_splay_tree_t **tree)
{
    s_splay_tree_t *k1;
    s_splay_tree_t *k2;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_structure_legal_p((*tree)->right));

    k1 = *tree;
    k2 = k1->right;

    k1->right = k2->left;
    k2->left = k1;

    *tree = k2;
}

static inline void
splay_tree_balance_splaying_root(s_splay_tree_t **tree, uint32 path_mask)
{

    assert_exit(!complain_null_pointer_p(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));

    switch (path_mask) {
        case PATH_LEFT:
            splay_tree_balance_splaying_root_left(tree);
            break;
        case PATH_RIGHT:
            splay_tree_balance_splaying_root_right(tree);
            break;
        default:
            assert_not_reached("Unexpected value of enum tree_path_type.\n");
            break;
    }
}

static inline void
splay_tree_balance_splaying(s_array_stack_t *path_stack)
{
    uint32 path_mask;
    s_splay_tree_t **iterator;

    assert_exit(array_stack_structure_legal_p(path_stack));

    while (array_stack_size(path_stack) >= 2) {
        iterator = array_stack_pop(path_stack);
        path_mask = TREE_PATH_MASK(iterator);

        iterator = array_stack_pop(path_stack);
        path_mask = path_mask + (TREE_PATH_MASK(iterator) << 1);

        iterator = TREE_PATH_DECODE(iterator);
        splay_tree_balance_splaying_i(iterator, path_mask);
    }

    if (array_stack_size(path_stack) == 1) {
        iterator = array_stack_pop(path_stack);
        path_mask = TREE_PATH_MASK(iterator);

        iterator = TREE_PATH_DECODE(iterator);
        splay_tree_balance_splaying_root(iterator, path_mask);
    }
}

static inline sint32
splay_tree_height_i(s_splay_tree_t *tree)
{
    sint32 height;
    s_splay_tree_t *splay_node;
    s_array_queue_t *queue_master, *queue_slave;

    if (!tree) {
        return -1;
    } else {
        height = -1;
        queue_master = array_queue_create();
        queue_slave = array_queue_create();

        array_queue_enter(queue_master, tree);
        while (!array_queue_empty_p(queue_master)) {
            splay_node = array_queue_leave(queue_master);
            if (splay_node->left) {
                array_queue_enter(queue_slave, splay_node->left);
            }
            if (splay_node->right) {
                array_queue_enter(queue_slave, splay_node->right);
            }

            if (array_queue_empty_p(queue_master)) {
                height++;
                swap_pointer((void **)&queue_master, (void **)&queue_slave);
            }
        }

        array_queue_destroy(&queue_master);
        array_queue_destroy(&queue_slave);

        return height;
    }
}

sint32
splay_tree_height(s_splay_tree_t *tree)
{
    if (!splay_tree_structure_legal_p(tree)) {
        return -1;
    } else {
        return splay_tree_height_i(tree);
    }
}

static inline bool
splay_tree_contains_ip(s_splay_tree_t *tree, s_splay_tree_t *node)
{
    sint64 nice;
    s_splay_tree_t *node_tmp;
    s_array_queue_t *repeated_queue;

    assert_exit(splay_tree_structure_legal_p(tree));
    assert_exit(splay_tree_structure_legal_p(node));

    node_tmp = tree;
    nice = node->nice;

    while (node_tmp) {
        if (node_tmp == node) {
            return true;
        } else if (node_tmp->nice < nice) {
            node_tmp = node_tmp->right;
        } else if (node_tmp->nice > nice) {
            node_tmp = node_tmp->left;
        } else { /* take care of repeated nice */
            repeated_queue = array_queue_create();
            array_queue_enter(repeated_queue, node_tmp);

            while (!array_queue_empty_p(repeated_queue)) {
                node_tmp = array_queue_leave(repeated_queue);
                if (node_tmp == node) {
                    array_queue_destroy(&repeated_queue);
                    return true;
                } else {
                    if (node_tmp->left && node_tmp->left->nice == nice) {
                        array_queue_enter(repeated_queue, node_tmp->left);
                    }
                    if (node_tmp->right && node_tmp->right->nice == nice) {
                        array_queue_enter(repeated_queue, node_tmp->right);
                    }
                }
            }

            array_queue_destroy(&repeated_queue);
            return false;
        }
    }

    return false;
}

bool
splay_tree_contains_p(s_splay_tree_t *tree, s_splay_tree_t *node)
{
    if (!splay_tree_structure_legal_p(tree)) {
        return false;
    } else if (!splay_tree_structure_legal_p(node)) {
        return false;
    } else {
        return splay_tree_contains_ip(tree, node);
    }
}

static inline void
splay_tree_repeated_insert(s_splay_tree_t *splay, s_splay_tree_t *inserted,
    sint32 path_direction)
{
    assert_exit(splay_tree_structure_legal_p(splay));
    assert_exit(splay_tree_structure_legal_p(inserted));
    assert_exit(splay != inserted);
    assert_exit(splay->nice == inserted->nice);

    if (path_direction > 0) { // If right is heavy, inserted to left.
        inserted->left = splay;
        inserted->right = splay->right;
        splay->right = NULL;
    } else {
        inserted->right = splay;
        inserted->left = splay->left;
        splay->left = NULL;
    }
}

static inline s_splay_tree_t *
splay_tree_insert_i(s_splay_tree_t **tree, s_splay_tree_t *node)
{
    sint32 path_direction;
    s_array_stack_t *path_stack;
    s_splay_tree_t *splay_node, **iterator;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(node));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_ordered_p(*tree));

    path_direction = 0;
    iterator = tree;
    path_stack = array_stack_create();
    splay_node = *iterator;

    while (splay_node) {
        if (node->nice < splay_node->nice) {
            path_direction--;
            array_stack_push(path_stack, TREE_PATH_L_ENCODE(iterator));
            iterator = &splay_node->left;
        } else if (node->nice > splay_node->nice) {
            path_direction++;
            array_stack_push(path_stack, TREE_PATH_R_ENCODE(iterator));
            iterator = &splay_node->right;
        } else if (splay_node == node) {
            pr_log_warn("Insert node exist, nothing will be done.\n");
            array_stack_destroy(&path_stack);
            return NULL;
        } else {
            splay_tree_repeated_insert(splay_node, node, path_direction);
            break;
        }
        splay_node = *iterator;
    }

    *iterator = node;
    splay_tree_balance_splaying(path_stack);
    array_stack_destroy(&path_stack);

    assert_exit(*tree == node);
    assert_exit(splay_tree_ordered_p(*tree));

    return node;
}

s_splay_tree_t *
splay_tree_insert(s_splay_tree_t **tree, s_splay_tree_t *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return splay_tree_insert_i(tree, node);
    }
}

static inline bool
splay_tree_doubly_child_p(s_splay_tree_t *node)
{
    assert_exit(splay_tree_structure_legal_p(node));

    if (node->left && node->right) {
        return true;
    } else {
        return false;
    }
}

static inline void
splay_tree_lt_doubly_child_strip(s_splay_tree_t **splay_node)
{
    s_splay_tree_t *node_strip;

    assert_exit(!complain_null_pointer_p(splay_node));
    assert_exit(splay_tree_structure_legal_p(*splay_node));

    node_strip = *splay_node;

    if (node_strip->left != NULL) {
        *splay_node = node_strip->left;
    } else {
        *splay_node = node_strip->right;
    }

    node_strip->left =  node_strip->right = NULL;
}

static inline s_splay_tree_t **
splay_tree_find_ptr_to_min(s_splay_tree_t **tree)
{
    s_splay_tree_t **min;
    s_splay_tree_t *splay;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));

    min = tree;
    splay = *tree;

    while (splay->left) {
        min = &splay->left;
        splay = *min;
    }

    return min;
}

static inline s_splay_tree_t **
splay_tree_find_ptr_to_max(s_splay_tree_t **tree)
{
    s_splay_tree_t **max;
    s_splay_tree_t *splay;

    assert_exit(!complain_null_pointer_p(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));

    max = tree;
    splay = *tree;

    while (splay->right) {
        max = &splay->right;
        splay = *max;
    }

    return max;
}

static inline void
splay_tree_swap_child(s_splay_tree_t *a, s_splay_tree_t *b)
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
splay_tree_doubly_child_strip_from_min(s_splay_tree_t **splay_node)
{
    s_splay_tree_t *splay;
    s_splay_tree_t *min;
    s_splay_tree_t **min_node;

    assert_exit(!complain_null_pointer_p(splay_node));
    assert_exit(splay_tree_structure_legal_p(*splay_node));
    assert_exit(splay_tree_doubly_child_p(*splay_node));

    splay = *splay_node;

    if (!splay->right->left) { // short cut here
        *splay_node = splay->right;
        splay->right->left = splay->left;
        splay->left = splay->right = NULL;
    } else {
        min_node = splay_tree_find_ptr_to_min(&splay->right);
        min = *min_node;

        splay_tree_swap_child(splay, min);
        *min_node = splay;
        *splay_node = min;

        splay_tree_lt_doubly_child_strip(min_node);
    }
}

static inline void
splay_tree_doubly_child_strip_from_max(s_splay_tree_t **splay_node)
{
    s_splay_tree_t *splay;
    s_splay_tree_t *max;
    s_splay_tree_t **max_node;

    assert_exit(!complain_null_pointer_p(splay_node));
    assert_exit(splay_tree_structure_legal_p(*splay_node));
    assert_exit(splay_tree_doubly_child_p(*splay_node));

    splay = *splay_node;

    if (!splay->left->right) { // short cut here
        *splay_node = splay->left;
        splay->left->right = splay->right;
        splay->left = splay->right = NULL;
    } else {
        max_node = splay_tree_find_ptr_to_max(&splay->left);
        max = *max_node;

        splay_tree_swap_child(splay, max);
        *max_node = splay;
        *splay_node = max;

        splay_tree_lt_doubly_child_strip(max_node);
    }
}

static inline void
splay_tree_doubly_child_strip(s_splay_tree_t **splay_node, sint32 direction)
{
    assert_exit(!complain_null_pointer_p(splay_node));
    assert_exit(splay_tree_structure_legal_p(*splay_node));
    assert_exit(splay_tree_doubly_child_p(*splay_node));

    if (direction > 0) {
        splay_tree_doubly_child_strip_from_max(splay_node);
    } else {
        splay_tree_doubly_child_strip_from_min(splay_node);
    }
}

static inline void
splay_tree_child_strip(s_splay_tree_t **splay_node, sint32 direction)
{
    assert_exit(!complain_null_pointer_p(splay_node));
    assert_exit(splay_tree_structure_legal_p(*splay_node));

    if (splay_tree_doubly_child_p(*splay_node)) {
        splay_tree_doubly_child_strip(splay_node, direction);
    } else {
        splay_tree_lt_doubly_child_strip(splay_node);
    }
}

static inline s_splay_tree_t *
splay_tree_repeated_remove(s_splay_tree_t **tree,
    s_splay_tree_t *node, sint32 direction)
{
    sint64 nice;
    s_array_queue_t *queue;
    s_splay_tree_t **iterator;
    s_splay_tree_t *removed_node, *splay_node;

    assert_exit(!complain_null_pointer_p(tree));
    assert_exit(splay_tree_structure_legal_p(node));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_ordered_p(*tree));
    assert_exit((*tree)->nice == node->nice);
    assert_exit(*tree != node);

    iterator = tree;
    removed_node = NULL;

    nice = node->nice;
    queue = array_queue_create();
    array_queue_enter(queue, iterator);

    while (!array_queue_empty_p(queue)) {
        iterator = array_queue_leave(queue);
        splay_node = *iterator;

        if (splay_node == node) {
            removed_node = node;
            splay_tree_child_strip(iterator, direction);
        } else if (splay_node->left && splay_node->left->nice == nice) {
            direction--;
            array_queue_enter(queue, &splay_node->left);
        } else if (splay_node->right && splay_node->right->nice == nice) {
            direction++;
            array_queue_enter(queue, &splay_node->right);
        }
    }

    array_queue_destroy(&queue);
    return removed_node;
}

static inline s_splay_tree_t *
splay_tree_remove_i(s_splay_tree_t **tree, s_splay_tree_t *node)
{
    sint32 direction;
    s_splay_tree_t **iterator;
    s_splay_tree_t *removed_node, *splay_node;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(splay_tree_structure_legal_p(*tree));
    assert_exit(splay_tree_ordered_p(*tree));
    assert_exit(splay_tree_structure_legal_p(node));

    direction = 0;
    removed_node = NULL;
    iterator = tree;
    splay_node = *tree;

    while (splay_node) {
        if (node->nice > splay_node->nice) {
            direction++;
            iterator = &splay_node->right;
        } else if (node->nice < splay_node->nice) {
            direction--;
            iterator = &splay_node->left;
        } else if (node == splay_node) {
            removed_node = node;
            splay_tree_child_strip(iterator, direction);
            break;
        } else {
            removed_node = splay_tree_repeated_remove(iterator, node, direction);
            break;
        }
        splay_node = *iterator;
    }

    if (removed_node == NULL) {
        pr_log_warn("Failed to find the node in given tree.\n");
    }

    assert_exit(splay_tree_ordered_p(*tree));
    return removed_node;
}

s_splay_tree_t *
splay_tree_remove(s_splay_tree_t **tree, s_splay_tree_t *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!splay_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return splay_tree_remove_i(tree, node);
    }
}

static inline void
splay_tree_iterate_i(s_splay_tree_t *tree, void (*handler)(void *))
{
    s_array_queue_t *queue;
    s_splay_tree_t *splay_node;

    assert_exit(!complain_null_pointer_p(handler));
    assert_exit(splay_tree_structure_legal_p(tree));

    queue = array_queue_create();
    array_queue_enter(queue, tree);

    while (!array_queue_empty_p(queue)) {
        splay_node = array_queue_leave(queue);
        handler(splay_node);

        if (splay_node->left) {
            array_queue_enter(queue, splay_node->left);
        }
        if (splay_node->right) {
            array_queue_enter(queue, splay_node->right);
        }
    }

    array_queue_destroy(&queue);
}

void
splay_tree_iterate(s_splay_tree_t *tree, void (*handler)(void *))
{
    if (complain_null_pointer_p(handler)) {
        return;
    } else if (!splay_tree_structure_legal_p(tree)) {
        return;
    } else {
        splay_tree_iterate_i(tree, handler);
    }
}

s_avl_tree_t *
avl_tree_left(s_avl_tree_t *tree)
{
    if (avl_tree_structure_legal_p(tree)) {
        return tree->left;
    } else {
        return PTR_INVALID;
    }
}

s_avl_tree_t *
avl_tree_right(s_avl_tree_t *tree)
{
    if (avl_tree_structure_legal_p(tree)) {
        return tree->right;
    } else {
        return PTR_INVALID;
    }
}

s_avl_tree_t *
avl_tree_create(void)
{
    s_avl_tree_t *tree;

    tree = memory_cache_allocate(sizeof(*tree));
    avl_tree_initial_i(tree, 0);

    return tree;
}

sint64
avl_tree_nice(s_avl_tree_t *tree)
{
    if (avl_tree_structure_legal_p(tree)) {
        return tree->nice;
    } else {
        return TREE_NICE_INVALID;
    }
}

static inline void
avl_tree_initial_i(s_avl_tree_t *tree, sint64 nice)
{
    assert_exit(!NULL_PTR_P(tree));

    tree->height = 0;
    tree->nice = nice;
    tree->left = NULL;
    tree->right = NULL;
}

void
avl_tree_initial(s_avl_tree_t *tree, sint64 nice)
{
    if (!complain_null_pointer_p(tree)) {
        avl_tree_initial_i(tree, nice);
    }
}

static inline bool
avl_tree_structure_legal_p(s_avl_tree_t *tree)
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
avl_tree_destroy_i(s_avl_tree_t *tree)
{
    s_array_queue_t *queue;
    s_avl_tree_t *avl_node;

    assert_exit(avl_tree_structure_legal_p(tree));

    queue = array_queue_create();
    array_queue_enter(queue, tree);

    while (!array_queue_empty_p(queue)) {
        avl_node = array_queue_leave(queue);

        if (avl_node->left != NULL) {
            array_queue_enter(queue, avl_node->left);
        }

        if (avl_node->right != NULL) {
            array_queue_enter(queue, avl_node->right);
        }

        memory_cache_free(avl_node);
    }

    array_queue_destroy(&queue);
}

void
avl_tree_destroy(s_avl_tree_t **tree)
{
    if (!NULL_PTR_P(tree) && avl_tree_structure_legal_p(*tree)) {
        avl_tree_destroy_i(*tree);
        *tree = NULL;
    }
}

static inline s_avl_tree_t *
avl_tree_find_i(s_avl_tree_t *tree, sint64 nice)
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

s_avl_tree_t *
avl_tree_find(s_avl_tree_t *tree, sint64 nice)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return PTR_INVALID;
    } else {
        return avl_tree_find_i(tree, nice);
    }
}

static inline s_avl_tree_t *
avl_tree_find_min_i(s_avl_tree_t *tree)
{
    assert_exit(avl_tree_structure_legal_p(tree));

    while (tree->left) {
        tree = tree->left;
    }

    return tree;
}

s_avl_tree_t *
avl_tree_find_min(s_avl_tree_t *tree)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return PTR_INVALID;
    } else {
        return avl_tree_find_min_i(tree);
    }
}

static inline s_avl_tree_t *
avl_tree_find_max_i(s_avl_tree_t *tree)
{
    assert_exit(avl_tree_structure_legal_p(tree));

    while (tree->right) {
        tree = tree->right;
    }

    return tree;
}

s_avl_tree_t *
avl_tree_find_max(s_avl_tree_t *tree)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return PTR_INVALID;
    } else {
        return avl_tree_find_max_i(tree);
    }
}

static inline bool
avl_tree_contains_ip(s_avl_tree_t *tree, s_avl_tree_t *node)
{
    bool retval;
    s_avl_tree_t *left;
    s_avl_tree_t *right;

    assert_exit(avl_tree_structure_legal_p(tree));
    assert_exit(avl_tree_structure_legal_p(node));

    retval = false;

    while (tree) {
        if (node == tree) {
            return true;
        } else if (node->nice > tree->nice) {
            tree =  tree->right;
        } else if (node->nice < tree->nice) {
            tree = tree->left;
        } else {
            left = tree->left;
            right = tree->right;

            // Handle nice repeated node
            if (left && node->nice == left->nice) {
                retval = avl_tree_contains_ip(left, node);
            }
            if (!retval && right && node->nice == right->nice) {
                retval = avl_tree_contains_ip(right, node);
            }

            return retval;
        }
    }

    return false;
}

bool
avl_tree_contains_p(s_avl_tree_t *tree, s_avl_tree_t *node)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return false;
    } else if (!avl_tree_structure_legal_p(node)) {
        return false;
    } else {
        return avl_tree_contains_ip(tree, node);
    }
}

static inline bool
avl_tree_node_balanced_p(s_avl_tree_t *node)
{
    assert_exit(avl_tree_structure_legal_p(node));

    return avl_tree_height_balanced_opt_p(node);
}

static inline bool
avl_tree_balanced_ip(s_avl_tree_t *tree)
{
    if (!tree) {
        return true;
    } else if (!avl_tree_node_balanced_p(tree)) {
        return false;
    } else if (!avl_tree_balanced_ip(tree->left)) {
        return false;
    } else if (!avl_tree_balanced_ip(tree->right)) {
        return false;
    } else {
        return true;
    }
}

bool
avl_tree_balanced_p(s_avl_tree_t *tree)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return true;
    } else {
        return avl_tree_balanced_ip(tree);
    }
}

static inline bool
avl_tree_doubly_child_p(s_avl_tree_t *tree)
{
    assert_exit(avl_tree_structure_legal_p(tree));

    if (tree->left && tree->right) {
        return true;
    } else {
        return false;
    }
}

sint32
avl_tree_height(s_avl_tree_t *tree)
{
    if (!avl_tree_structure_legal_p(tree)) {
        return -1;
    } else {
        return avl_tree_height_opt(tree);
    }
}

static inline void
avl_tree_height_update(s_avl_tree_t *tree)
{
    sint32 left;
    sint32 right;

    assert_exit(avl_tree_structure_legal_p(tree));
    // Also height_update_opt

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
static inline s_avl_tree_t *
avl_tree_single_rotate_left(s_avl_tree_t *node)
{
    s_avl_tree_t *k1;
    s_avl_tree_t *k2;

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
static inline s_avl_tree_t *
avl_tree_single_rotate_right(s_avl_tree_t *node)
{
    s_avl_tree_t *k1;
    s_avl_tree_t *k2;

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
static inline s_avl_tree_t *
avl_tree_doubly_rotate_left(s_avl_tree_t *node)
{
    s_avl_tree_t *k1;
    s_avl_tree_t *k2;
    s_avl_tree_t *k3;

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
static inline s_avl_tree_t *
avl_tree_doubly_rotate_right(s_avl_tree_t *node)
{
    s_avl_tree_t *k1;
    s_avl_tree_t *k2;
    s_avl_tree_t *k3;

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
avl_tree_rotate_left(s_avl_tree_t **tree)
{
    sint32 left_height;
    sint32 right_height;
    s_avl_tree_t *left;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_structure_legal_p((*tree)->left));

    left = (*tree)->left;
    left_height = avl_tree_height_opt(left->left);
    right_height = avl_tree_height_opt(left->right);

    if (left_height >= right_height) {
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
avl_tree_rotate_right(s_avl_tree_t **tree)
{
    sint32 left_height;
    sint32 right_height;
    s_avl_tree_t *right;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_structure_legal_p((*tree)->right));

    right = (*tree)->right;
    left_height = avl_tree_height_opt(right->left);
    right_height = avl_tree_height_opt(right->right);

    if (right_height >= left_height) {
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


static inline s_avl_tree_t *
avl_tree_insert_i(s_avl_tree_t **tree, s_avl_tree_t *node)
{
    s_avl_tree_t *avl;
    s_avl_tree_t *inserted;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_structure_legal_p(node));
    assert_exit(avl_tree_balanced_p(*tree));
    assert_exit(avl_tree_ordered_p(*tree));

    avl = *tree;
    inserted = node;

    if (avl == node) {
        pr_log_info("Insert node exist, nothing will be done.\n");
        return NULL;
    } else if (node->nice <= avl->nice) { // Always inserted repeated value to left
        if (NULL == avl->left) {
            avl->left = node;
        } else {
            inserted = avl_tree_insert_i(&avl->left, node);
            if (!avl_tree_node_balanced_p(avl)) {
                avl_tree_rotate_left(tree);
            }
        }
    } else { // node->nice > avl->nice
        if (NULL == avl->right) {
            avl->right = node;
        } else {
            inserted = avl_tree_insert_i(&avl->right, node);
            if (!avl_tree_node_balanced_p(avl)) {
                avl_tree_rotate_right(tree);
            }
        }
    }

    avl_tree_height_update(avl);
    assert_exit(avl_tree_ordered_p(*tree));
    assert_exit(avl_tree_balanced_p(*tree));
    return inserted;
}

s_avl_tree_t *
avl_tree_insert(s_avl_tree_t **tree, s_avl_tree_t *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!avl_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!avl_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return avl_tree_insert_i(tree, node);
    }
}

static inline void
avl_tree_lt_doubly_child_strip(s_avl_tree_t **pre, s_avl_tree_t *node)
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
avl_tree_doubly_child_strip(s_avl_tree_t **node_pre)
{
    sint32 left_height;
    sint32 right_height;
    s_avl_tree_t *avl;

    assert_exit(!NULL_PTR_P(node_pre));
    assert_exit(avl_tree_structure_legal_p(*node_pre));

    avl = *node_pre;
    left_height = avl_tree_height_opt(avl->left);
    right_height = avl_tree_height_opt(avl->right);

    if (left_height < right_height) {
        avl_tree_doubly_child_strip_from_max(node_pre);
    } else {
        avl_tree_doubly_child_strip_from_min(node_pre);
    }
}

// Fix me cmpxchange ?
static inline void
avl_tree_swap_child(s_avl_tree_t *a, s_avl_tree_t *b)
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

static inline s_avl_tree_t **
avl_tree_find_ptr_to_max(s_avl_tree_t **tree)
{
    s_avl_tree_t *avl;
    s_avl_tree_t **max;

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

static inline s_avl_tree_t **
avl_tree_find_ptr_to_min(s_avl_tree_t **tree)
{
    s_avl_tree_t *avl;
    s_avl_tree_t **min;

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
avl_tree_doubly_child_strip_from_max(s_avl_tree_t **node_pre)
{
    void *tmp;
    s_avl_tree_t *avl;
    s_avl_tree_t *max;
    s_avl_tree_t **max_pre;

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

    avl_tree_remove_i(&max->left, avl);
}

static inline void
avl_tree_doubly_child_strip_from_min(s_avl_tree_t **node_pre)
{
    void *tmp;
    s_avl_tree_t *avl;
    s_avl_tree_t *min;
    s_avl_tree_t **min_pre;

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

    avl_tree_remove_i(&min->right, avl);
}

static inline s_avl_tree_t *
avl_tree_remove_i(s_avl_tree_t **tree, s_avl_tree_t *node)
{
    s_avl_tree_t *avl;
    s_avl_tree_t *removed;

    assert_exit(!NULL_PTR_P(tree));
    assert_exit(avl_tree_structure_legal_p(*tree));
    assert_exit(avl_tree_balanced_p(*tree));
    assert_exit(avl_tree_ordered_p(*tree));
    assert_exit(avl_tree_structure_legal_p(node));

    avl = *tree;
    removed = NULL;

    if (node->nice < avl->nice) {
        if (NULL == avl->left) {
            pr_log_warn("Failed to find the node in given tree.\n");
            return NULL;
        } else {
            removed = avl_tree_remove_i(&avl->left, node);
            if (!avl_tree_node_balanced_p(avl)) {
                avl_tree_rotate_right(tree);
            }
            avl_tree_height_update(avl);
        }
    } else if (node->nice > avl->nice) {
        if (NULL == avl->right) {
            pr_log_warn("Failed to find the node in given tree.\n");
            return NULL;
        } else {
            removed = avl_tree_remove_i(&avl->right, node);
            if (!avl_tree_node_balanced_p(avl)) {
                 avl_tree_rotate_left(tree);
            }
            avl_tree_height_update(avl);
        }
    } else if (node != avl) {
        if (avl->left && node->nice == avl->left->nice) {
            removed = avl_tree_remove_i(&avl->left, node);
            avl_tree_height_update(avl);
        }
        if (!removed && avl->right && node->nice == avl->right->nice) {
            removed = avl_tree_remove_i(&avl->right, node);
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

s_avl_tree_t *
avl_tree_remove(s_avl_tree_t **tree, s_avl_tree_t *node)
{
    if (NULL_PTR_P(tree)) {
        return PTR_INVALID;
    } else if (!avl_tree_structure_legal_p(*tree)) {
        return PTR_INVALID;
    } else if (!avl_tree_structure_legal_p(node)) {
        return PTR_INVALID;
    } else {
        return avl_tree_remove_i(tree, node);
    }
}

static inline void
avl_tree_iterate_i(s_avl_tree_t *tree, void (*handler)(void *))
{
    s_avl_tree_t *avl_node;
    s_array_queue_t *queue_slave;
    s_array_queue_t *queue_master;

    assert_exit(!complain_null_pointer_p(handler));
    assert_exit(avl_tree_structure_legal_p(tree));

    queue_slave = array_queue_create();
    queue_master = array_queue_create();
    array_queue_enter(queue_master, tree);

    while (!array_queue_empty_p(queue_master)) {
        while (!array_queue_empty_p(queue_master)) {
            avl_node = array_queue_leave(queue_master);
            handler(avl_node);

            if (avl_node->left) {
                array_queue_enter(queue_slave, avl_node->left);
            }
            if (avl_node->right) {
                array_queue_enter(queue_slave, avl_node->right);
            }
        }
        swap_pointer((void **)&queue_master, (void **)&queue_slave);
    }

    array_queue_destroy(&queue_slave);
    array_queue_destroy(&queue_master);
}

void
avl_tree_iterate(s_avl_tree_t *tree, void (*handler)(void *))
{
    if (NULL_PTR_P(handler)) {
        return;
    } else if (!avl_tree_structure_legal_p(tree)) {
        return;
    } else {
        avl_tree_iterate_i(tree, handler);
    }
}

s_skip_linked_list_t *
skip_linked_list_next(s_skip_linked_list_t *list)
{
    if (!skip_linked_list_structure_legal_p(list)) {
        return PTR_INVALID;
    } else {
        return list->next;
    }
}

void
skip_linked_list_next_set(s_skip_linked_list_t *list,
    s_skip_linked_list_t *next)
{
    if (skip_linked_list_structure_legal_p(list)
        && skip_linked_list_structure_legal_p(next)) {
        list->next = next;
    }
}

sint32
skip_linked_list_key(s_skip_linked_list_t *list)
{
    if (!skip_linked_list_structure_legal_p(list)) {
        return SKIP_KEY_INVALID;
    } else {
        return list->key;
    }
}

void
skip_linked_list_key_set(s_skip_linked_list_t *list, sint32 key)
{
    if (skip_linked_list_structure_legal_p(list)) {
        list->key = key;
    }
}

bool
skip_linked_list_structure_legal_p(s_skip_linked_list_t *list)
{
    return skip_linked_list_structure_legal_ip(list);
}

static inline bool
skip_linked_list_structure_legal_ip(s_skip_linked_list_t *list)
{
    if (NULL_PTR_P(list)) {
        return false;
    } else if (list->next == list) {
        return false;
    } else if (list->key == SKIP_KEY_INVALID) {
        return false;
    } else {
        return true;
    }
}

s_skip_linked_list_t *
skip_linked_list_create(void)
{
    s_skip_linked_list_t *list;

    list = memory_cache_allocate(sizeof(*list));
    memset(list, 0, sizeof(*list));
    skip_linked_list_initial_i(list, 0);

    return list;
}

s_skip_linked_list_t *
skip_linked_list_create_with_key(sint32 key)
{
    s_skip_linked_list_t *list;

    list = memory_cache_allocate(sizeof(*list));
    memset(list, 0, sizeof(*list));
    skip_linked_list_initial_i(list, key);

    return list;
}

void
skip_linked_list_initial(s_skip_linked_list_t *list)
{
    if (!NULL_PTR_P(list)) {
        skip_linked_list_initial_i(list, 0);
    }
}

static inline void
skip_linked_list_initial_i(s_skip_linked_list_t *list, sint32 key)
{
    assert_exit(!NULL_PTR_P(list));

    list->key = key;
    list->next = NULL;
}

void
skip_linked_list_destroy(s_skip_linked_list_t **list)
{
    s_skip_linked_list_t *next;
    s_skip_linked_list_t *node;

    if (!NULL_PTR_P(list)
        && skip_linked_list_structure_legal_p(*list)) {
        node = *list;

        while (node) {
            next = node->next;
            memory_cache_free(node);
            node = next;
        }

        *list = NULL;
    }
}

static inline uint32
skip_linked_list_length_i(s_skip_linked_list_t *list)
{
    uint32 retval;
    s_skip_linked_list_t *node;

    assert_exit(skip_linked_list_structure_legal_p(list));

    retval = 0u;
    node = list;

    while (node) {
        retval++;
        node = node->next;
    }

    return retval;
}

uint32
skip_linked_list_length(s_skip_linked_list_t *list)
{
    if (!skip_linked_list_structure_legal_p(list)) {
        return LIST_SIZE_INVALID;
    } else {
        return skip_linked_list_length_i(list);
    }
}

s_skip_linked_list_t *
skip_linked_list_find_key(s_skip_linked_list_t *list, sint32 key)
{
    if (!skip_linked_list_structure_legal_p(list)) {
        return PTR_INVALID;
    } else {
        return skip_linked_list_find_key_i(list, key, SKIP_LIST_MAX_LVL_IDX);
    }
}

static inline s_skip_linked_list_t *
skip_linked_list_find_key_i(s_skip_linked_list_t *list, sint32 key, uint32 lv)
{
    s_skip_linked_list_t **head;

    assert_exit(skip_linked_list_structure_legal_p(list));
    assert_exit(SKIP_LIST_MAX_LVL > lv);

    while (true) {
        head = &list->layer[lv];

        if (list->key == key) {
            return list;
        } else if ((!*head || (*head)->key > key)) {
            if (SKIP_LIST_BOTTOM_IDX != lv) {
                lv--;
            } else {
                return NULL;
            }
        } else {
            list = list->layer[lv];
        }
    }
}

static inline bool
skip_linked_list_contains_ip(s_skip_linked_list_t *list,
    s_skip_linked_list_t *node)
{
    s_skip_linked_list_t *skip;

    assert_exit(skip_linked_list_structure_legal_p(list));
    assert_exit(skip_linked_list_structure_legal_p(node));

    skip = list;
    while (NULL != skip) {
        if (node == skip) {
            return true;
        }
        skip = skip->next;
    }

    return false;
}

bool
skip_linked_list_contains_p(s_skip_linked_list_t *list,
    s_skip_linked_list_t *node)
{
    if (!skip_linked_list_structure_legal_p(list)) {
        return false;
    } else if (!skip_linked_list_structure_legal_p(node)) {
        return false;
    } else {
        return skip_linked_list_contains_ip(list, node);
    }
}

static inline s_skip_linked_list_t *
skip_linked_list_insert_before_head(s_skip_linked_list_t *list,
    s_skip_linked_list_t *tgt)
{
    uint32 lv;

    assert_exit(skip_linked_list_structure_legal_p(list));
    assert_exit(skip_linked_list_structure_legal_p(tgt));
    assert_exit(tgt->key < list->key);

    lv = SKIP_LIST_MAX_LVL_IDX;

    while (SKIP_LIST_BOTTOM_IDX != lv) {
        tgt->layer[lv] = list->layer[lv];
        list->layer[lv] = NULL;
        lv--;
    }

    tgt->layer[SKIP_LIST_BOTTOM_IDX] = list;

    return tgt;
}

static inline s_skip_linked_list_t *
skip_linked_list_insert_i(s_skip_linked_list_t **list,
    s_skip_linked_list_t *tgt)
{
    uint32 lv;
    uint32 lmt;
    s_skip_linked_list_t *skip;
    s_skip_linked_list_t **head;
    s_skip_linked_list_t *prev_list[SKIP_LIST_MAX_LVL];

    assert_exit(!NULL_PTR_P(list));
    assert_exit(skip_linked_list_ordering_p(*list));
    assert_exit(skip_linked_list_structure_legal_p(tgt));
    assert_exit(skip_linked_list_structure_legal_p(*list));

    lv = SKIP_LIST_MAX_LVL_IDX;
    head = list;
    skip = *head;

    if (tgt->key < skip->key) {
        *list = skip_linked_list_insert_before_head(skip, tgt);
    } else {
        while (true) {
            head = &skip->layer[lv];

            if (skip == tgt || *head == tgt) {
                pr_log_warn("Insert node already existed in linked list.\n");
                return NULL;
            } else if (!*head || (*head)->key >= tgt->key) {
                prev_list[lv] = skip;
                if (SKIP_LIST_BOTTOM_IDX != lv) {
                    lv--;
                } else {
                    lmt = random_uint32_with_limit(SKIP_LIST_MAX_LVL);
                    skip_linked_list_insert_update_with_lv(tgt, prev_list, lmt);
                    break;
                }
            } else {
                skip = skip->layer[lv];
            }
        }
    }

    assert_exit(skip_linked_list_ordering_p(*list));
    return tgt;
}

s_skip_linked_list_t *
skip_linked_list_insert(s_skip_linked_list_t **list,
    s_skip_linked_list_t *tgt)
{
    if (NULL_PTR_P(list)) {
        return PTR_INVALID;
    } else if (!skip_linked_list_structure_legal_p(*list)) {
        return PTR_INVALID;
    } else if (!skip_linked_list_structure_legal_p(tgt)) {
        return PTR_INVALID;
    } else {
        return skip_linked_list_insert_i(list, tgt);
    }
}

static inline void
skip_linked_list_insert_update_with_lv(s_skip_linked_list_t *tgt,
    s_skip_linked_list_t **prev_list, uint32 lv)
{
    assert_exit(!NULL_PTR_P(prev_list));
    assert_exit(skip_linked_list_structure_legal_p(tgt));
    assert_exit(SKIP_LIST_MAX_LVL > lv);

    do {
        tgt->layer[lv] = prev_list[lv]->layer[lv];
        prev_list[lv]->layer[lv] = tgt;
    } while (0 != lv--);
}

s_skip_linked_list_t *
skip_linked_list_node_by_index(s_skip_linked_list_t *list, uint32 index)
{
    uint32 len;
    s_skip_linked_list_t *skip;

    if (!skip_linked_list_structure_legal_p(list)) {
        return PTR_INVALID;
    } else {
        skip = list;
        len = skip_linked_list_length(list);

        if (index >= len) {
            pr_log_warn("Index out of the lenght, rotated to front.\n");
            index = index % len;
        }

        while (index) {
            skip = skip->next;
            index--;
        }

        return skip;
    }
}

static inline void
skip_linked_list_node_clean(s_skip_linked_list_t *list)
{
    uint32 lv;

    assert_exit(skip_linked_list_structure_legal_p(list));

    lv = SKIP_LIST_BOTTOM_IDX;

    while (NULL != list->layer[lv] && lv < SKIP_LIST_MAX_LVL) {
        list->layer[lv++] = NULL;
    }
}

static inline void
skip_linked_list_remove_head(s_skip_linked_list_t *list)
{
    uint32 lv;
    s_skip_linked_list_t *next;

    assert_exit(skip_linked_list_structure_legal_p(list));

    next = list->next;

    if (next) {
        lv = SKIP_LIST_MAX_LVL_IDX;

        while (NULL == next->layer[lv] && lv > SKIP_LIST_BOTTOM_IDX) {
            next->layer[lv] = list->layer[lv];
            lv--;
        }
    }

    skip_linked_list_node_clean(list);
}

static inline void
skip_linked_list_remove_on_level(s_skip_linked_list_t *list,
    s_skip_linked_list_t *removed, uint32 level)
{
    s_skip_linked_list_t *skip;

    assert_exit(SKIP_LIST_MAX_LVL > level);
    assert_exit(skip_linked_list_structure_legal_p(list));
    assert_exit(skip_linked_list_structure_legal_p(removed));

    do {
        skip = list;
        assert_exit(skip_linked_list_exist_on_level(list, removed, level));

        while (skip->layer[level] != removed) {
            skip = skip->layer[level];
        }

        skip->layer[level] = removed->layer[level];
        level--;
    } while (SKIP_LIST_MAX_LVL > level);

    skip_linked_list_node_clean(removed);
}

static inline s_skip_linked_list_t *
skip_linked_list_remove_i(s_skip_linked_list_t **list,
    s_skip_linked_list_t *tgt)
{
    uint32 lv;
    s_skip_linked_list_t *node;
    s_skip_linked_list_t *head;
    s_skip_linked_list_t *removed;

    assert_exit(!NULL_PTR_P(list));
    assert_exit(skip_linked_list_ordering_p(*list));
    assert_exit(skip_linked_list_structure_legal_p(tgt));
    assert_exit(skip_linked_list_structure_legal_p(*list));

    if ((*list)->key == tgt->key) {
        removed = *list;
        *list = removed->next;
        skip_linked_list_remove_head(removed);
    } else {
        node = *list;
        head = node;
        lv = SKIP_LIST_MAX_LVL_IDX;

        while (true) {
            list = &node->layer[lv];

            if (!*list || (*list)->key > tgt->key) {
                if (SKIP_LIST_BOTTOM_IDX == lv) {
                    removed = NULL;
                    pr_log_warn("The removing node do not exist in given list.\n");
                    break;
                } else {
                    lv--;
                }
            } else if ((*list) == tgt) {
                removed = tgt;
                skip_linked_list_remove_on_level(head, removed, lv);
                break;
            } else {
                node = node->layer[lv];
            }
        }

        assert_exit(skip_linked_list_ordering_p(head));
    }

    return removed;
}

s_skip_linked_list_t *
skip_linked_list_remove(s_skip_linked_list_t **list,
    s_skip_linked_list_t *tgt)
{
    if (NULL_PTR_P(list)) {
        return PTR_INVALID;
    } else if (!skip_linked_list_structure_legal_p(*list)) {
        return PTR_INVALID;
    } else if (!skip_linked_list_structure_legal_p(tgt)) {
        return PTR_INVALID;
    } else {
        return skip_linked_list_remove_i(list, tgt);
    }
}

void
skip_linked_list_iterate(s_skip_linked_list_t *list,
    void (*handler)(void *))
{
    s_skip_linked_list_t *skip;

    if (!NULL_PTR_P(handler)
        && skip_linked_list_structure_legal_p(list)) {
        skip = list;
        while(skip) {
            (*handler)(skip);
            skip = skip->next;
        }
    }
}

s_skip_linked_list_t *
skip_linked_list_merge(s_skip_linked_list_t *m, s_skip_linked_list_t *n)
{
    s_skip_linked_list_t *skip;
    s_skip_linked_list_t *inserted;

    if (!skip_linked_list_structure_legal_p(m)
        && !skip_linked_list_structure_legal_p(n)) {
        return PTR_INVALID;
    } else if (!skip_linked_list_structure_legal_p(m)) {
        return n;
    } else if (!skip_linked_list_structure_legal_p(n)) {
        return m;
    } else {
        skip = n;

        while (skip) {
            inserted = skip_linked_list_create_with_key(skip->key);
            skip_linked_list_insert_i(&m, inserted);
            skip = skip->next;
        }

        return m;
    }
}

s_single_linked_list_t *
single_linked_list_next(s_single_linked_list_t *list)
{
    if (single_linked_list_structure_illegal_ip(list)) {
        return PTR_INVALID;
    } else {
        return list->next;
    }
}

void
single_linked_list_next_set(s_single_linked_list_t *list,
    s_single_linked_list_t *next)
{
    if (single_linked_list_structure_legal_ip(list)
        && single_linked_list_structure_legal_ip(next)) {
        list->next = next;
    }
}

bool
single_linked_list_structure_legal_p(s_single_linked_list_t *list)
{
    return single_linked_list_structure_legal_ip(list);
}

bool
single_linked_list_structure_illegal_p(s_single_linked_list_t *list)
{
    return single_linked_list_structure_illegal_ip(list);
}

static inline bool
single_linked_list_structure_illegal_ip(s_single_linked_list_t *list)
{
    return !single_linked_list_structure_legal_ip(list);
}

static inline bool
single_linked_list_structure_legal_ip(s_single_linked_list_t *list)
{
    if (NULL_PTR_P(list)) {
        return false;
    } else if (list->next == NULL) {
        return false;
    } else {
        return true;
    }
}

s_single_linked_list_t *
single_linked_list_create(void)
{
    return single_linked_list_create_i();
}

static inline void
single_linked_list_initial_i(s_single_linked_list_t *list)
{
    assert_exit(!NULL_PTR_P(list));

    list->next = list;
}

static inline s_single_linked_list_t *
single_linked_list_create_i(void)
{
    s_single_linked_list_t *list;

    list = memory_cache_allocate(sizeof(*list));
    single_linked_list_initial_i(list);

    return list;
}

void
single_linked_list_initial(s_single_linked_list_t *list)
{
    if (!NULL_PTR_P(list)) {
        single_linked_list_initial_i(list);
    }
}

static inline void
single_linked_list_insert_after_i(s_single_linked_list_t *list,
    s_single_linked_list_t *node)
{
    assert_exit(NON_NULL_PTR_P(node));
    assert_exit(single_linked_list_structure_legal_ip(list));
    assert_exit(!single_linked_list_contains_ip(list, node));

    node->next = list->next;
    list->next = node;
}

void
single_linked_list_insert_after(s_single_linked_list_t *list,
    s_single_linked_list_t *node)
{
    if (single_linked_list_structure_illegal_ip(list) || NULL_PTR_P(node)) {
        return;
    } else {
        /*
         * Use assert_exit for contains check here for performance.
         * If check contains, the cost wiil be O(1) => O(n) for each insert.
         */
        single_linked_list_insert_after_i(list, node);
    }
}

static inline void
single_linked_list_insert_before_i(s_single_linked_list_t *list,
    s_single_linked_list_t *node)
{
    s_single_linked_list_t *prev;

    assert_exit(NON_NULL_PTR_P(node));
    assert_exit(single_linked_list_structure_legal_ip(list));
    assert_exit(!single_linked_list_contains_ip(list, node));

    prev = single_linked_list_previous_i(list);
    prev->next = node;
    node->next = list;
}

void
single_linked_list_insert_before(s_single_linked_list_t *list,
    s_single_linked_list_t *node)
{
    if (single_linked_list_structure_illegal_ip(list) || NULL_PTR_P(node)) {
        return;
    } else {
        /*
         * Use assert_exit for contains check here for performance.
         * If check contains, the cost wiil be O(1) => O(n) for each insert.
         */
        single_linked_list_insert_before_i(list, node);
    }
}

s_single_linked_list_t *
single_linked_list_node_copy(s_single_linked_list_t *node)
{
    s_single_linked_list_t *copy;

    if (single_linked_list_structure_illegal_ip(node)) {
        return PTR_INVALID;
    } else {
        copy = single_linked_list_create_i();
        copy->next = node->next;
        return copy;
    }
}

void
single_linked_list_destroy(s_single_linked_list_t **list)
{
    s_single_linked_list_t *node;
    s_single_linked_list_t *next;

    if (!NULL_PTR_P(list)
        && single_linked_list_structure_legal_ip(*list)) {
        node = *list;
        do {
            next = node->next;
            memory_cache_free(node);
            node = next;
        } while (*list != node);

        *list = NULL;
    }
}

static inline uint32
single_linked_list_length_i(s_single_linked_list_t *list)
{
    uint32 len;
    s_single_linked_list_t *node;

    assert_exit(single_linked_list_structure_legal_ip(list));

    len = 0u;
    node = list;

    do {
        len++;
        node = node->next;
    } while (node != list);

    return len;
}

uint32
single_linked_list_length(s_single_linked_list_t *list)
{
    if (single_linked_list_structure_illegal_ip(list)) {
        return LIST_SIZE_INVALID;
    } else {
        return single_linked_list_length_i(list);
    }
}

s_single_linked_list_t *
single_linked_list_node_by_index(s_single_linked_list_t *list, uint32 index)
{
    uint32 len;
    s_single_linked_list_t *node;

    if (single_linked_list_structure_illegal_ip(list)) {
        return PTR_INVALID;
    } else {
        len = single_linked_list_length_i(list);
        if (index >= len) {
            pr_log_warn("Index out of the lenght, rotated to front.\n");
            index = index % len;
        }

        node = list;
        while (index > 0) {
            node = node->next;
            index--;
        }

        return node;
    }
}

static inline bool
single_linked_list_contains_ip(s_single_linked_list_t *list,
    s_single_linked_list_t *node)
{
    s_single_linked_list_t *single;

    assert_exit(single_linked_list_structure_legal_ip(list));
    assert_exit(single_linked_list_structure_legal_ip(node));

    single = list;

    do {
        if (single == node) {
            return true;
        }
        single = single ->next;
    } while (single != list);

    return false;
}

bool
single_linked_list_contains_p(s_single_linked_list_t *list,
    s_single_linked_list_t *node)
{
    if (single_linked_list_structure_illegal_ip(list)) {
        return false;
    } else if (single_linked_list_structure_illegal_ip(node)) {
        return false;
    } else {
        return single_linked_list_contains_ip(list, node);
    }
}

static inline s_single_linked_list_t *
single_linked_list_previous_i(s_single_linked_list_t *list)
{
    register s_single_linked_list_t *prev;

    assert_exit(single_linked_list_structure_legal_ip(list));
    assert_exit(single_linked_list_structure_legal_ip(list->next));

    prev = list;

    do {
        prev = prev->next;
    } while (prev->next != list);

    return prev;
}

s_single_linked_list_t *
single_linked_list_previous(s_single_linked_list_t *list)
{
    if (single_linked_list_structure_illegal_ip(list)) {
        return PTR_INVALID;
    } else {
        return single_linked_list_previous_i(list);
    }
}

static inline s_single_linked_list_t *
single_linked_list_remove_i(s_single_linked_list_t **list)
{
    s_single_linked_list_t *prev;
    s_single_linked_list_t *removed;

    assert_exit(!NULL_PTR_P(list));
    assert_exit(single_linked_list_structure_legal_ip(*list));

    removed = *list;

    if (*list == removed->next) {
        *list = NULL;
        return removed;
    } else {
        prev = single_linked_list_previous_i(removed);
        prev->next = removed->next;
        *list = removed->next;
        removed->next = removed;

        return removed;
    }
}

s_single_linked_list_t *
single_linked_list_remove(s_single_linked_list_t **list)
{
    if (NULL_PTR_P(list)) {
        return PTR_INVALID;
    } else if (single_linked_list_structure_illegal_ip(*list)) {
        return PTR_INVALID;
    } else {
        return single_linked_list_remove_i(list);
    }
}

void
single_linked_list_iterate(s_single_linked_list_t *list,
    void (*handler)(void *))
{
    s_single_linked_list_t *node;

    if (!NULL_PTR_P(handler)
        && single_linked_list_structure_legal_ip(list)) {
        node = list;

        do {
            (*handler)(node);
            node = node->next;
        } while (node != list);
    }
}

static inline s_single_linked_list_t *
single_linked_list_merge_i(s_single_linked_list_t *m,
    s_single_linked_list_t *n)
{
    s_single_linked_list_t *m_prev;
    s_single_linked_list_t *n_prev;

    assert_exit(single_linked_list_structure_legal_p(m));
    assert_exit(single_linked_list_structure_legal_p(n));
    assert_exit(!single_linked_list_contains_ip(m, n));

    m_prev = single_linked_list_previous_i(m);
    n_prev = single_linked_list_previous_i(n);

    m_prev->next = n;
    n_prev->next = m;

    return m;
}

s_single_linked_list_t *
single_linked_list_merge(s_single_linked_list_t *m,
    s_single_linked_list_t *n)
{
    if (single_linked_list_structure_illegal_ip(m)
        && single_linked_list_structure_illegal_ip(n)) {
        return PTR_INVALID;
    } else if (single_linked_list_structure_illegal_ip(m)) {
        return n;
    } else if (single_linked_list_structure_illegal_ip(n)) {
        return m;
    } else {
        /*
         * Use assert_exit for contains check here for performance.
         * If check contains, the cost wiil be O(1) => O(n) for each insert.
         */
        return single_linked_list_merge_i(m, n);
    }
}

s_doubly_linked_list_t *
doubly_linked_list_next(s_doubly_linked_list_t *list)
{
    if (doubly_linked_list_structure_illegal_ip(list)) {
        return PTR_INVALID;
    } else {
        return list->next;
    }
}

void
doubly_linked_list_next_set(s_doubly_linked_list_t *list,
    s_doubly_linked_list_t *next)
{
    if (doubly_linked_list_structure_legal_ip(list)
        && doubly_linked_list_structure_legal_ip(next)) {
        list->next = next;
    }
}

bool
doubly_linked_list_structure_legal_p(s_doubly_linked_list_t *list)
{
    return doubly_linked_list_structure_legal_ip(list);
}

bool
doubly_linked_list_structure_illegal_p(s_doubly_linked_list_t *list)
{
    return doubly_linked_list_structure_illegal_ip(list);
}

static inline bool
doubly_linked_list_structure_illegal_ip(s_doubly_linked_list_t *list)
{
    return !doubly_linked_list_structure_legal_ip(list);
}

static inline bool
doubly_linked_list_structure_legal_ip(s_doubly_linked_list_t *list)
{
    if (NULL_PTR_P(list)) {
        return false;
    } else if (list->next == NULL || list->previous == NULL) {
        return false;
    } else {
        return true;
    }
}

s_doubly_linked_list_t *
doubly_linked_list_previous(s_doubly_linked_list_t *list)
{
    if (doubly_linked_list_structure_illegal_ip(list)) {
        return PTR_INVALID;
    } else {
        return list->previous;
    }
}

void
doubly_linked_list_previous_set(s_doubly_linked_list_t *list,
    s_doubly_linked_list_t *previous)
{
    if (doubly_linked_list_structure_legal_ip(list)
        && doubly_linked_list_structure_legal_ip(previous)) {
        list->previous = previous;
    }
}

s_doubly_linked_list_t *
doubly_linked_list_create(void)
{
    return doubly_linked_list_create_i(NULL);
}

static inline void
doubly_linked_list_initial_i(s_doubly_linked_list_t *list)
{
    assert_exit(NON_NULL_PTR_P(list));

    list->next = list;
    list->previous = list;
}

void
doubly_linked_list_initial(s_doubly_linked_list_t *list)
{
    if (NON_NULL_PTR_P(list)) {
        doubly_linked_list_initial_i(list);
    }
}

static inline s_doubly_linked_list_t *
doubly_linked_list_create_i()
{
    s_doubly_linked_list_t *list;

    list = memory_cache_allocate(sizeof(*list));
    doubly_linked_list_initial_i(list);

    return list;
}

static inline void
doubly_linked_list_insert_after_i(s_doubly_linked_list_t *list,
    s_doubly_linked_list_t *node)
{
    assert_exit(NON_NULL_PTR_P(node));
    assert_exit(doubly_linked_list_structure_legal_ip(list));
    assert_exit(!doubly_linked_list_contains_ip(list, node));

    list->next->previous = node;
    node->next = list->next;
    list->next = node;
    node->previous = list;
}

void
doubly_linked_list_insert_after(s_doubly_linked_list_t *list,
    s_doubly_linked_list_t *node)
{
    if (doubly_linked_list_structure_illegal_ip(list) || NULL_PTR_P(node)) {
        return;
    } else {
        /*
         * Use assert_exit for contains check here for performance.
         * If check contains, the cost wiil be O(1) => O(n) for each insert.
         */
        doubly_linked_list_insert_after_i(list, node);
    }
}

static inline void
doubly_linked_list_insert_before_i(s_doubly_linked_list_t *list,
    s_doubly_linked_list_t *node)
{
    assert_exit(NON_NULL_PTR_P(node));
    assert_exit(doubly_linked_list_structure_legal_ip(list));
    assert_exit(!doubly_linked_list_contains_ip(list, node));

    doubly_linked_list_insert_after_i(list->previous, node);
}

void
doubly_linked_list_insert_before(s_doubly_linked_list_t *list,
    s_doubly_linked_list_t *node)
{
    if (doubly_linked_list_structure_illegal_ip(list) || NULL_PTR_P(node)) {
        return;
    } else {
        /*
         * Use assert_exit for contains check here for performance.
         * If check contains, the cost wiil be O(1) => O(n) for each insert.
         */
        doubly_linked_list_insert_before_i(list, node);
    }
}

s_doubly_linked_list_t *
doubly_linked_list_node_copy(s_doubly_linked_list_t *node)
{
    s_doubly_linked_list_t *copy;

    if (doubly_linked_list_structure_illegal_ip(node)) {
        return PTR_INVALID;
    } else {
        copy = doubly_linked_list_create_i();
        copy->next = node->next;
        copy->previous = node->previous;
        return copy;
    }
}

void
doubly_linked_list_destroy(s_doubly_linked_list_t **list)
{
    s_doubly_linked_list_t *node;
    s_doubly_linked_list_t *next;

    if (!NULL_PTR_P(list)
        && doubly_linked_list_structure_legal_ip(*list)) {
        node = *list;
        do {
            next = node->next;
            memory_cache_free(node);
            node = next;
        } while(*list != node);

        *list = NULL;
    }
}

static inline uint32
doubly_linked_list_length_i(s_doubly_linked_list_t *list)
{
    uint32 len;
    register s_doubly_linked_list_t *node;

    assert_exit(doubly_linked_list_structure_legal_ip(list));

    len = 0;
    node = list;

    do {
        len++;
        node = node->next;
    } while (node != list);

    return len;
}

uint32
doubly_linked_list_length(s_doubly_linked_list_t *list)
{
    if (doubly_linked_list_structure_illegal_ip(list)) {
        return LIST_SIZE_INVALID;
    } else {
        return doubly_linked_list_length_i(list);
    }
}

s_doubly_linked_list_t *
doubly_linked_list_node_by_index(s_doubly_linked_list_t *list,
    uint32 index)
{
    uint32 len;
    s_doubly_linked_list_t *node;

    if (doubly_linked_list_structure_illegal_ip(list)) {
        return PTR_INVALID;
    } else {
        len = doubly_linked_list_length_i(list);

        if (index >= len) {
            pr_log_warn("Index out of the lenght, rotated to front.\n");
            index = index % len;
        }

        node = list;
        while (index > 0) {
            node = node->next;
            index--;
        }

        return node;
    }
}

static inline bool
doubly_linked_list_contains_ip(s_doubly_linked_list_t *list,
    s_doubly_linked_list_t *node)
{
    s_doubly_linked_list_t *doubly;

    assert_exit(NON_NULL_PTR_P(node));
    assert_exit(doubly_linked_list_structure_legal_ip(list));

    doubly = list;

    do {
        if (doubly == node) {
            return true;
        }

        doubly = doubly->next;
    } while (doubly != list);

    return false;
}

bool
doubly_linked_list_contains_p(s_doubly_linked_list_t *list,
    s_doubly_linked_list_t *node)
{
    if (doubly_linked_list_structure_illegal_ip(list)) {
        return false;
    } else if (doubly_linked_list_structure_illegal_ip(node)) {
        return false;
    } else {
        return doubly_linked_list_contains_ip(list, node);
    }
}

static inline s_doubly_linked_list_t *
doubly_linked_list_remove_i(s_doubly_linked_list_t **list)
{
    s_doubly_linked_list_t *removed;

    assert_exit(!NULL_PTR_P(list));
    assert_exit(doubly_linked_list_structure_legal_ip(*list));

    removed = *list;

    if (*list == removed->next) {
        *list = NULL;
        return removed;
    } else {
        removed->previous->next = removed->next;
        removed->next->previous = removed->previous;

        *list = removed->next;

        removed->next = removed;
        removed->previous = removed;

        return removed;
    }
}

s_doubly_linked_list_t *
doubly_linked_list_remove(s_doubly_linked_list_t **list)
{
    if (NULL_PTR_P(list)) {
        return PTR_INVALID;
    } else if (doubly_linked_list_structure_illegal_ip(*list)) {
        return PTR_INVALID;
    } else {
        return doubly_linked_list_remove_i(list);
    }
}

void
doubly_linked_list_iterate(s_doubly_linked_list_t *list, void (*handler)(void *))
{
    s_doubly_linked_list_t *node;

    if (!NULL_PTR_P(handler)
        && doubly_linked_list_structure_legal_ip(list)) {
        node = list;

        do {
            (*handler)(node);
            node = node->next;
        } while (node != list);
    }
}

static inline s_doubly_linked_list_t *
doubly_linked_list_merge_i(s_doubly_linked_list_t *m,
    s_doubly_linked_list_t *n)
{
    s_doubly_linked_list_t *n_prev;

    assert_exit(doubly_linked_list_structure_legal_p(m));
    assert_exit(doubly_linked_list_structure_legal_p(n));
    assert_exit(!doubly_linked_list_contains_ip(m, n));

    m->previous->next = n;
    n->previous->next = m;

    n_prev = n->previous;
    n->previous = m->previous;
    m->previous = n_prev;

    return m;
}

s_doubly_linked_list_t *
doubly_linked_list_merge(s_doubly_linked_list_t *m,
    s_doubly_linked_list_t *n)
{
    if (doubly_linked_list_structure_illegal_ip(m)
        && doubly_linked_list_structure_illegal_ip(n)) {
        return PTR_INVALID;
    } else if (doubly_linked_list_structure_illegal_ip(m)) {
        return n;
    } else if (doubly_linked_list_structure_illegal_ip(n)) {
        return m;
    } else {
        /*
         * Use assert_exit for contains check here for performance.
         * If check contains, the cost wiil be O(1) => O(n) for each insert.
         */
        return doubly_linked_list_merge_i(m, n);
    }
}

static inline bool
skip_linked_list_ordering_p(s_skip_linked_list_t *list)
{
    assert_exit(skip_linked_list_structure_legal_ip(list));

    while (NULL != list->next) {
        if (list->key > list->next->key) {
            return false;
        }
        list = list->next;
    }

    return true;
}

static inline bool
skip_linked_list_exist_on_level(s_skip_linked_list_t *list,
    s_skip_linked_list_t *node, uint32 level)
{
    s_skip_linked_list_t *iter;

    assert_exit(SKIP_LIST_MAX_LVL > level);
    assert_exit(skip_linked_list_structure_legal_ip(list));
    assert_exit(skip_linked_list_structure_legal_ip(node));

    iter = list;
    while (NULL != iter) {
        if (iter == node) {
            return true;
        }
        iter = iter->layer[level];
    }

    return false;
}

static inline bool
array_queue_enter_optimize_legal_p(s_array_queue_t *queue, void *member)
{
    uint32 dim;
    uint32 rest;
    void **rear, **rear_backup;

    assert_exit(array_queue_structure_legal_ip(queue));

    dim = queue->space.dim;
    rest = queue->space.rest;

    rear_backup = queue->space.rear;
    rear = queue->space.rear + 1;

    if (rear == queue->space.base + dim) {
        rear = queue->space.base;
    }

    ARRAY_QUEUE_ENTER_I(queue, member);

    if (*rear_backup != member) {
        return false;
    } else if (rear != queue->space.rear) {
        return false;
    } else if (rest != queue->space.rest + 1) {
        return false;
    } else { // retrieve here
        queue->space.rest++;
        queue->space.rear = rear_backup;

        return true;
    }
}

static inline bool
array_queue_leave_optimize_legal_p(s_array_queue_t *queue)
{
    uint32 dim;
    uint32 rest;
    void *val, *val_backup;
    void **front, **front_backup;

    assert_exit(array_queue_structure_legal_ip(queue));

    dim = queue->space.dim;
    rest = queue->space.rest;

    val_backup = *queue->space.front;
    front_backup = queue->space.front;
    front = queue->space.front + 1;

    if (front == queue->space.base + dim) {
        front = queue->space.base;
    }

    ARRAY_QUEUE_LEAVE_I(queue, val);

    if (val != val_backup) {
        return false;
    } else if (rest + 1 != queue->space.rest) {
        return false;
    } else if (front != queue->space.front) {
        return false;
    } else { // retrieve here
        queue->space.rest--;
        queue->space.front = front_backup;

        return true;
    }
}

/*
 * *HACK*, use the last node of resize expand for store the element list of
 *         queue for sanity check.
 *         As the last element of expand space should not be touched, use it
 *         as the pointer to void * array, which hold all element in queue
 *         in order.
 *         then the other function will use the data for sanity check, and
 *         free them elegantly.
 */
static inline bool
array_queue_resize_restore_data_p(s_array_queue_t *queue, uint32 size,
    void **addr)
{
    void **restore_array;
    void **data_i, **data_limit;
    uint32 queue_element_size;

    assert_exit(!complain_null_pointer_p(addr));
    assert_exit(!complain_zero_size_p(size));
    assert_exit(array_queue_structure_legal_ip(queue));

    if (array_queue_empty_ip(queue)) {
        return true;
    } else {
        queue_element_size = array_queue_size(queue);
        addr[size] = dp_malloc(sizeof(void *) * (queue_element_size + 1));
        restore_array = addr[size];

        data_limit = queue->space.base + queue->space.dim;
        data_i = queue->space.front;

        do {
            *restore_array++ = *data_i++;
            if (data_limit == data_i) {
                data_i = queue->space.base;
            }
        } while (data_i != queue->space.rear);

        *restore_array = NULL;
        assert_exit(queue_element_size == restore_array - (void **)addr[size]);

        return true;
    }
}

static inline bool
array_queue_resize_data_consistant_p(s_array_queue_t *queue)
{
    uint32 i;
    void **restore_array;
    void **data_i, **data_limit;

    assert_exit(array_queue_structure_legal_ip(queue));
    assert_exit(queue->space.front == queue->space.base);

    if (array_queue_empty_ip(queue)) {
        return true;
    } else {
        data_i = queue->space.base;
        data_limit = queue->space.base + array_queue_size(queue);

        i = 0;
        restore_array = queue->space.base[queue->space.dim];

        do {
            if (restore_array[i] != *data_i) {
                dp_free(restore_array);
                return false;
            }
            i++;
            data_i++;
        } while (data_i != data_limit);

        dp_free(restore_array);
        return true;
    }
}

uint32
array_queue_capacity(s_array_queue_t *queue)
{
    if (ARRAY_QUEUE_ILLEGAL_P(queue)) {
        return QUEUE_CPCT_INVALID;
    } else {
        return queue->space.dim;
    }
}

/*
 * Return the size of elements in given queue.
 */
uint32
array_queue_size(s_array_queue_t *queue)
{
    if (array_queue_structure_illegal_ip(queue)) {
        return QUEUE_CPCT_INVALID;
    } else {
        return queue->space.dim - queue->space.rest;
    }
}

s_array_iterator_t *
array_queue_iterator_obtain(s_array_queue_t *queue)
{
    if (ARRAY_QUEUE_LEGAL_P(queue)) {
        return &queue->iterator;
    } else {
        return PTR_INVALID;
    }
}

static inline void
array_queue_iterator_index_initial(void *queue)
{
    uint32 index;
    uint32 limit;
    s_array_iterator_t *iterator;
    s_array_queue_t *array_queue;

    assert_exit(array_queue_structure_legal_ip(queue));

    array_queue = queue;
    iterator = &array_queue->iterator;

    index = (uint32)(array_queue->space.front - array_queue->space.base);
    limit = (uint32)(array_queue->space.rear - array_queue->space.base);

    iterator->index = index;
    iterator->limit = limit;
    iterator->accessed = false;
}

static inline bool
array_queue_iterator_next_exist_p(void *queue)
{
    s_array_iterator_t *iterator;
    s_array_queue_t *array_queue;

    assert_exit(array_queue_structure_legal_ip(queue));

    array_queue = queue;
    iterator = &array_queue->iterator;

    if (array_queue_empty_ip(queue)) {
        return false;
    } else if (iterator->index == iterator->limit && iterator->accessed) {
        /* if iterator not accessed, index == limit is the first node rotated */
        return false;
    } else {
        return true;
    }
}

static inline void *
array_queue_iterator_next_obtain(void *queue)
{
    void *next_node;
    s_array_iterator_t *iterator;
    s_array_queue_t *array_queue;

    assert_exit(array_queue_structure_legal_ip(queue));
    assert_exit(array_queue_iterator_next_exist_p(queue));

    array_queue = queue;
    iterator = &array_queue->iterator;
    next_node = array_queue->space.base[iterator->index];

    iterator->index++;
    iterator->accessed = true;

    if (iterator->index == array_queue->space.dim) { /* Index of array rotated */
        iterator->index = 0;
    }

    return next_node;
}

s_array_queue_t *
array_queue_create(void)
{
    uint32 size;
    s_array_queue_t *queue;
    void *initial, *next_exist_p, *next_obtain;

    size = QUEUE_SIZE_DFT;
    queue = memory_cache_allocate(sizeof(*queue));

    queue->space.dim = size;
    queue->space.base = memory_cache_allocate(sizeof(void *) * size);
    queue->space.rest = size;
    queue->space.front = queue->space.base;
    queue->space.rear = queue->space.base;

    initial = &array_queue_iterator_index_initial;
    next_exist_p = &array_queue_iterator_next_exist_p;
    next_obtain = &array_queue_iterator_next_obtain;

    array_iterator_initial(&queue->iterator, initial, next_exist_p, next_obtain);

    return queue;
}

void
array_queue_destroy(s_array_queue_t **queue)
{
    if (NON_NULL_PTR_P(queue) && ARRAY_QUEUE_LEGAL_P(*queue)) {
        memory_cache_free((*queue)->space.base);
        memory_cache_free(*queue);

        *queue = NULL;
    }
}

static inline bool
array_queue_space_structure_legal_p(s_array_queue_space_t *space)
{
    if (NULL_PTR_P(space)) {
        return false;
    } else if (space->rest > space->dim) {
        return false;
    } else if (NULL_PTR_P(space->base)) {
        return false;
    } else if (NULL_PTR_P(space->front)) {
        return false;
    } else if (NULL_PTR_P(space->rear)) {
        return false;
    } else {
        return true;
    }
}

static inline bool
array_queue_structure_legal_ip(s_array_queue_t *queue)
{
    if (NULL_PTR_P(queue)) {
        return false;
    } else if (!array_iterator_structure_legal_p(&queue->iterator)) {
        return false;
    } else {
        return array_queue_space_structure_legal_p(&queue->space);
    }
}

bool
array_queue_structure_legal_p(s_array_queue_t *queue)
{
    return array_queue_structure_legal_ip(queue);
}

static inline bool
array_queue_structure_illegal_ip(s_array_queue_t *queue)
{
    return !array_queue_structure_legal_ip(queue);
}

bool
array_queue_structure_illegal_p(s_array_queue_t *queue)
{
    return array_queue_structure_illegal_ip(queue);
}

static inline bool
array_queue_resize_rotated_p(s_array_queue_t *queue)
{
    assert_exit(array_queue_structure_legal_ip(queue));

    if (queue->space.front < queue->space.rear) {
        return false;
    } else if (array_queue_empty_ip(queue)) {
        return false;
    } else {
        return true;
    }
}

static inline void
array_queue_resize_expand(s_array_queue_t *queue, uint32 size)
{
    void **addr;
    uint32 left_size, right_size;
    uint32 data_size, leading_size;

    assert_exit(array_queue_structure_legal_ip(queue));
    assert_exit(size > queue->space.dim);

    /* Additional 1 position for checking */
    addr = memory_cache_allocate(sizeof(void *) * (size + 1));
    assert_exit(array_queue_resize_restore_data_p(queue, size, addr));

    if (queue->space.front < queue->space.rear) {
        data_size = queue->space.rear - queue->space.front;
        dp_memcpy(addr, queue->space.front, sizeof(void *) * data_size);
    } else if (array_queue_resize_rotated_p(queue)) {
        /*
         *      r   f            f         r
         * +-+-+-+-+-+-+-+      +-+-+-+-+-+-+-+-+-+
         * |a|b| | |X|Y|Z|  ==> |X|Y|Z|a|b| | | | |
         * +-+-+-+-+-+-+-+      +-+-+-+-+-+-+-+-+-+
         * left|   |right
         * leading |
         */
        leading_size = queue->space.front - queue->space.base;
        left_size = queue->space.rear - queue->space.base;
        right_size = queue->space.dim - leading_size;

        dp_memcpy(addr, queue->space.front, sizeof(void *) * right_size);
        dp_memcpy(addr + right_size, queue->space.base, sizeof(void *) * left_size);

        data_size = left_size + right_size;
    } else { /* queue is empty */
        data_size = 0;
    }

    memory_cache_free(queue->space.base);
    queue->space.front = queue->space.base = addr;
    queue->space.rear = addr + data_size;
    queue->space.rest = size - data_size;
    queue->space.dim = size;

    assert_exit(array_queue_resize_data_consistant_p(queue));
}

static inline void
array_queue_resize_narrow(s_array_queue_t *queue, uint32 size)
{
    void **addr;
    uint32 left_size, right_size;
    uint32 data_size, leading_size, rest_size;

    assert_exit(array_queue_structure_legal_ip(queue));
    assert_exit(size < queue->space.dim);

    /* Additional 1 position for checking */
    addr = memory_cache_allocate(sizeof(void *) * (size + 1));
    assert_exit(array_queue_resize_restore_data_p(queue, size, addr));

    if (queue->space.front < queue->space.rear) {
        data_size = queue->space.rear - queue->space.front;
        data_size = MIN_U32(data_size, size); /* May be trancated from tail. */
        dp_memcpy(addr, queue->space.front, sizeof(void *) * data_size);
    } else if (array_queue_resize_rotated_p(queue)) {
        /*                             r
         *            r   f            f
         * +-+-+-+-+-+-+-+-+-+-+      +-+-+-+-+-+
         * |A|B|C|D|E| | |x|y|z|  ==> |x|y|z|A|B|
         * +-+-+-+-+-+-+-+-+-+-+      +-+-+-+-+-+
         *     left  |   |right
         *     leading   |
         */
        leading_size = queue->space.front - queue->space.base;
        left_size = queue->space.rear - queue->space.base;
        right_size = queue->space.dim - leading_size;

        data_size = MIN_U32(right_size, size); /* May be trancated from tail. */
        dp_memcpy(addr, queue->space.front, sizeof(void *) * data_size);

        if (right_size < size) {
            rest_size = size - right_size;
            rest_size = MIN_U32(rest_size, left_size); /* May be trancated from tail. */
            dp_memcpy(addr + data_size, queue->space.base, sizeof(void *) * rest_size);
            data_size += rest_size;
        }
    } else { /* queue is empty */
        data_size = 0;
    }

    memory_cache_free(queue->space.base);
    queue->space.front = queue->space.base = addr;
    queue->space.rear = size == data_size ? addr : addr + data_size;
    queue->space.rest = size - data_size;
    queue->space.dim = size;

    assert_exit(array_queue_resize_data_consistant_p(queue));
}

static inline void
array_queue_resize_i(s_array_queue_t *queue, uint32 size)
{
    assert_exit(!complain_zero_size_p(size));
    assert_exit(array_queue_structure_legal_ip(queue));
    assert_exit(size != queue->space.dim);

    if (size > queue->space.dim) {
        array_queue_resize_expand(queue, size);
    } else {
        array_queue_resize_narrow(queue, size);
    }
}

void
array_queue_resize(s_array_queue_t *queue, uint32 size)
{
    if (ARRAY_QUEUE_ILLEGAL_P(queue)) {
        return;
    } else if (size == queue->space.dim) {
        return;
    } else if (complain_zero_size_p(size)) {
        size = queue->space.dim * 2 + QUEUE_EXPD_SIZE_MIN;
        pr_log_info("Expanding size not specified, use default.\n");
    }

    array_queue_resize_i(queue, size);
}

uint32
array_queue_rest(s_array_queue_t *queue)
{
    if (ARRAY_QUEUE_ILLEGAL_P(queue)) {
        return QUEUE_REST_INVALID;
    } else {
        return queue->space.rest;
    }
}

static inline ALWAYS_INLINE bool
array_queue_full_ip(s_array_queue_t *queue)
{
    assert_exit(array_queue_structure_legal_ip(queue));

    return 0u == queue->space.rest ? true : false;
}

bool
array_queue_full_p(s_array_queue_t *queue)
{
    if (ARRAY_QUEUE_ILLEGAL_P(queue)) {
        return true;
    } else {
        return array_queue_full_ip(queue);
    }
}

static inline ALWAYS_INLINE bool
array_queue_empty_ip(s_array_queue_t *queue)
{
    assert_exit(array_queue_structure_legal_ip(queue));

    return queue->space.dim == queue->space.rest ? true : false;
}

bool
array_queue_empty_p(s_array_queue_t *queue)
{
    if (ARRAY_QUEUE_ILLEGAL_P(queue)) {
        return false;
    } else {
        return array_queue_empty_ip(queue);
    }
}

void *
array_queue_front(s_array_queue_t *queue)
{
    if (ARRAY_QUEUE_ILLEGAL_P(queue)) {
        return PTR_INVALID;
    } else if (array_queue_empty_ip(queue)) {
        return PTR_INVALID;
    } else {
        return *queue->space.front;
    }
}

void *
array_queue_rear(s_array_queue_t *queue)
{
    if (ARRAY_QUEUE_ILLEGAL_P(queue)) {
        return PTR_INVALID;
    } else if (array_queue_empty_ip(queue)) {
        return PTR_INVALID;
    } else {
        if (queue->space.rear == queue->space.base) {
             return *(queue->space.base + queue->space.dim - 1);
        } else {
             return *(queue->space.rear - 1);
        }
    }
}

void
array_queue_enter(s_array_queue_t *queue, void *member)
{
    if (ARRAY_QUEUE_LEGAL_P(queue)) {
        if (array_queue_full_ip(queue)) {
            array_queue_resize_i(queue, queue->space.dim * 2);
        }

        assert_exit(array_queue_enter_optimize_legal_p(queue, member));
        ARRAY_QUEUE_ENTER_I(queue, member);
        /*
         * *HACK* => performance
         * ARRAY_QUEUE_ENTER_I rewrite follow code within assembly code.
         * array_queue_enter_optimize_legal_p guarantee the correctness.
         *
         * uint32 dim;
         * queue->space.rear++ = member;
         * dim = queue->space.dim;
         *
         * if (queue->space.rear == queue->space.base + dim) {
         *     pr_log_info("Reach the limitation of array, will rotate.\n");
         *     queue->space.rear = queue->space.base;
         * }
         *
         * queue->space.rest--;
         */
    }
}

void *
array_queue_leave(s_array_queue_t *queue)
{
    void *val;

    if (ARRAY_QUEUE_ILLEGAL_P(queue)) {
        return PTR_INVALID;
    } else if (array_queue_empty_ip(queue)) {
        pr_log_warn("Attempt to leave from _EMPTY_ queue.\n");
        return PTR_INVALID;
    } else {
        assert_exit(array_queue_leave_optimize_legal_p(queue));
        ARRAY_QUEUE_LEAVE_I(queue, val);
        /*
         * *HACK* => performance
         * ARRAY_QUEUE_LEAVE_I rewrite follow code within assembly code.
         * array_queue_leave_optimize_legal_p guarantee the correctness.
         *
         * uint32 dim;
         * val = *queue->space.front++;
         * dim = queue->space.dim;
         *
         * if (queue->space.front == queue->space.base + dim) {
         *     pr_log_info("Reach the limitation of array, will rotate.\n");
         *     queue->space.front = queue->space.base;
         * }
         *
         * queue->space.rest++;
         */

        return val;
    }
}

void
array_queue_cleanup(s_array_queue_t *queue)
{
    uint32 dim;

    if (ARRAY_QUEUE_LEGAL_P(queue)) {
        dim = queue->space.dim;
        dp_memset(queue->space.base, 0, sizeof(void *) * dim);

        queue->space.rest = dim;
        queue->space.front = queue->space.base;
        queue->space.rear = queue->space.base;
    }
}

void
array_queue_iterate(s_array_queue_t *queue, void (*handler)(void *))
{
    void *element;
    s_array_iterator_t *iterator;

    if (ARRAY_QUEUE_ILLEGAL_P(queue)) {
        return;
    } else if (NULL_PTR_P(handler)) {
        return;
    } else if (array_queue_empty_ip(queue)) {
        pr_log_warn("Iterate on _EMPTY_ queue, nothing will be done.\n");
    } else {
        iterator = &queue->iterator;
        iterator->fp_index_initial(queue);

        while (iterator->fp_next_exist_p(queue)) {
            element = iterator->fp_next_obtain(queue);
            (*handler)(element);
        }
    }
}

static inline void
array_queue_merge_i(s_array_queue_t *queue_dest, s_array_queue_t *queue_src)
{
    void *element;
    s_array_iterator_t *iterator;


    iterator = &queue_src->iterator;
    iterator->fp_index_initial(queue_src);

    while (iterator->fp_next_exist_p(queue_src)) {
        element = iterator->fp_next_obtain(queue_src);
        array_queue_enter(queue_dest, element);
    }

}


static inline void
array_queue_copy_i(s_array_queue_t *queue_dest, s_array_queue_t *queue_src)
{
    assert_exit(array_queue_structure_legal_ip(queue_src));
    assert_exit(array_queue_structure_legal_ip(queue_dest));

    array_queue_cleanup(queue_dest);
    array_queue_merge_i(queue_dest, queue_src);
}

void
array_queue_merge(s_array_queue_t *queue_dest, s_array_queue_t *queue_src)
{
    if (ARRAY_QUEUE_ILLEGAL_P(queue_dest)) {
        return;
    } else if (ARRAY_QUEUE_ILLEGAL_P(queue_src)) {
        return;
    } else {
        array_queue_merge_i(queue_dest, queue_src);
    }
}

void
array_queue_copy(s_array_queue_t *queue_dest, s_array_queue_t *queue_src)
{
    if (ARRAY_QUEUE_ILLEGAL_P(queue_dest)) {
        return;
    } else if (ARRAY_QUEUE_ILLEGAL_P(queue_src)) {
        return;
    } else {
        array_queue_copy_i(queue_dest, queue_src);
    }
}

uint32
stacked_queue_capacity(s_stacked_queue_t *queue)
{
    if (!queue) {
        return QUEUE_CPCT_INVALID;
    } else {
        return queue->dim;
    }
}

s_stacked_queue_t *
stacked_queue_create(void)
{
    s_stacked_queue_t *queue;

    queue = memory_cache_allocate(sizeof(*queue));

    queue->enter = array_stack_create();
    queue->leave = array_stack_create();
    queue->dim = array_stack_capacity(queue->enter);

    return queue;
}

static inline bool
stacked_queue_structure_legal_p(s_stacked_queue_t *queue)
{
    if (NULL_PTR_P(queue)) {
        return false;
    } else if (complain_zero_size_p(queue->dim)) {
        return false;
    } else if (!array_stack_structure_legal_p(queue->enter)) {
        return false;
    } else if (!array_stack_structure_legal_p(queue->leave)) {
        return false;
    } else {
        return true;
    }
}

void
stacked_queue_destroy(s_stacked_queue_t **queue)
{
    if (!NULL_PTR_P(queue) && stacked_queue_structure_legal_p(*queue)) {
        array_stack_destroy(&(*queue)->enter);
        array_stack_destroy(&(*queue)->leave);
        memory_cache_free(*queue);

        *queue = NULL;
    }
}

static inline void
stacked_queue_resize_i(s_stacked_queue_t *queue, uint32 dim)
{
    assert_exit(!complain_zero_size_p(dim));
    assert_exit(stacked_queue_structure_legal_p(queue));

    array_stack_resize(queue->enter, dim);
    array_stack_resize(queue->leave, dim);
    queue->dim = array_stack_capacity(queue->enter);
}

void
stacked_queue_resize(s_stacked_queue_t *queue, uint32 dim)
{
    if (stacked_queue_structure_legal_p(queue)) {
        if (0 == dim) {
            pr_log_info("Expanding size not specified, use default.\n");
            dim = queue->dim * 2 + QUEUE_EXPD_SIZE_MIN;
        }

        stacked_queue_resize_i(queue, dim);
    }
}

static inline uint32
stacked_queue_rest_i(s_stacked_queue_t *queue)
{
    uint32 rest;

    assert_exit(stacked_queue_structure_legal_p(queue));

    rest = array_stack_rest(queue->enter);
    if (array_stack_empty_p(queue->leave)) {
        rest += array_stack_capacity(queue->leave);
    }

    return rest;
}

uint32
stacked_queue_rest(s_stacked_queue_t *queue)
{
    if (!stacked_queue_structure_legal_p(queue)) {
        return QUEUE_SIZE_INVALID;
    } else {
        return stacked_queue_rest_i(queue);
    }
}

bool
stacked_queue_full_p(s_stacked_queue_t *queue)
{
    if (!stacked_queue_structure_legal_p(queue)) {
        return true;
    } else {
        return 0u == stacked_queue_rest_i(queue) ? true : false;
    }
}

static inline bool
stacked_queue_empty_ip(s_stacked_queue_t *queue)
{
    assert_exit(stacked_queue_structure_legal_p(queue));

    return array_stack_empty_p(queue->enter) && array_stack_empty_p(queue->leave);
}

bool
stacked_queue_empty_p(s_stacked_queue_t *queue)
{
    if (!stacked_queue_structure_legal_p(queue)) {
        return false;
    } else {
        return stacked_queue_empty_ip(queue);
    }
}

void *
stacked_queue_front(s_stacked_queue_t *queue)
{
    if (!stacked_queue_structure_legal_p(queue)) {
        return PTR_INVALID;
    } else if (stacked_queue_empty_ip(queue)) {
        pr_log_warn("Attempt to access _EMPTY_ queue.\n");
        return PTR_INVALID;
    } else if (array_stack_empty_p(queue->leave)) {
        stacked_queue_stack_dump(queue->enter, queue->leave);
        return array_stack_top(queue->leave);
    } else {
        return array_stack_top(queue->leave);
    }
}

void
stacked_queue_enter(s_stacked_queue_t *queue, void *member)
{
    if (stacked_queue_structure_legal_p(queue) && !NULL_PTR_P(member)) {
        if (array_stack_full_p(queue->enter) && array_stack_empty_p(queue->leave)) {
            pr_log_info("Empty leave stack, will dump enter stack to leave.\n");
            stacked_queue_stack_dump(queue->enter, queue->leave);
        } else if (array_stack_full_p(queue->enter)) {
            pr_log_info("Fail to dump enter stack, will expand enter space.\n");
            stacked_queue_resize_i(queue, queue->dim * 2);
        }

        array_stack_push(queue->enter, member);
    }
}

static inline void
stacked_queue_stack_dump(s_array_stack_t *from, s_array_stack_t *to)
{
    assert_exit(array_stack_structure_legal_p(to));
    assert_exit(array_stack_structure_legal_p(from));
    assert_exit(array_stack_empty_p(to));
    assert_exit(array_stack_capacity(from) == array_stack_capacity(to));

    while (!array_stack_empty_p(from)) {
        array_stack_push(to, array_stack_pop(from));
    }
}

void *
stacked_queue_rear(s_stacked_queue_t *queue)
{
    if (!stacked_queue_structure_legal_p(queue)) {
        return PTR_INVALID;
    } else if (stacked_queue_empty_ip(queue)) {
        pr_log_warn("Attempt to access _EMPTY_ queue.\n");
        return PTR_INVALID;
    } else if (array_stack_empty_p(queue->enter)) {
        stacked_queue_stack_dump(queue->leave, queue->enter);
        return array_stack_top(queue->enter);
    } else {
        return array_stack_top(queue->enter);
    }
}

void *
stacked_queue_leave(s_stacked_queue_t *queue)
{
    if (!stacked_queue_structure_legal_p(queue)) {
        return PTR_INVALID;
    } else if (stacked_queue_empty_ip(queue)) {
        pr_log_warn("Attempt to leave from _EMPTY_ queue.\n");
        return PTR_INVALID;
    } else {
        if (array_stack_empty_p(queue->leave)) {
            pr_log_info("Empty leave stack, will dump enter stack to leave.\n");
            stacked_queue_stack_dump(queue->enter, queue->leave);
        }
        return array_stack_pop(queue->leave);
    }
}

void
stacked_queue_cleanup(s_stacked_queue_t *queue)
{
    if (stacked_queue_structure_legal_p(queue)) {
        array_stack_cleanup(queue->enter);
        array_stack_cleanup(queue->leave);
    }
}

void
stacked_queue_iterate(s_stacked_queue_t *queue, void (*handler)(void *))
{
    if (stacked_queue_structure_legal_p(queue) && !NULL_PTR_P(handler)) {
        array_stack_iterate(queue->leave, handler);
        array_stack_iterate(queue->enter, handler);
    }
}

s_doubly_end_queue_t *
doubly_end_queue_create(void)
{
    s_doubly_end_queue_t *queue;

    queue = memory_cache_allocate(sizeof(*queue));
    queue->front = queue->rear = NULL;

    return queue;
}

bool
doubly_end_queue_structure_legal_p(s_doubly_end_queue_t *queue)
{
    if (NULL_PTR_P(queue)) {
        return false;
    } else if (NULL == queue->front && NULL != queue->rear) {
        return false;
    } else if (NULL != queue->front && NULL == queue->rear) {
        return false;
    } else {
        return true;
    }
}

static inline bool
doubly_end_queue_list_structure_legal_p(s_doubly_end_queue_list_t *list)
{
    if (NULL_PTR_P(list)) {
        return false;
    } else {
        return doubly_linked_list_structure_legal_p(&list->link);
    }
}

void
doubly_end_queue_destroy(s_doubly_end_queue_t **queue)
{
    if (!NULL_PTR_P(queue) && doubly_end_queue_structure_legal_p(*queue)) {
        if (!doubly_end_queue_empty_ip(*queue)) {
            doubly_end_queue_cleanup_i(*queue);
        }

        memory_cache_free(*queue);
        *queue = NULL;
    }
}

uint32
doubly_end_queue_length(s_doubly_end_queue_t *queue)
{
    uint32 retval;
    s_doubly_end_queue_list_t *link;

    if (!doubly_end_queue_structure_legal_p(queue)) {
        return QUEUE_SIZE_INVALID;
    } else {
        if (doubly_end_queue_empty_ip(queue)) {
            pr_log_info("Empty doubly end queue.\n");
            return 0u;
        } else {
            retval = 0u;
            link = queue->front;

            do {
                link = doubly_end_queue_list_next(link);
                retval++;
            } while (link != queue->front);

            return retval;
        }
    }
}

static inline s_doubly_end_queue_list_t *
doubly_end_queue_list_reflect(s_doubly_linked_list_t *list)
{
    ptr_t offset;

    assert_exit(doubly_linked_list_structure_legal_p(list));

    offset = (ptr_t)&((s_doubly_end_queue_list_t *)0)->link;

    return (void *)((ptr_t)list - offset);
}

static inline s_doubly_end_queue_list_t *
doubly_end_queue_list_next(s_doubly_end_queue_list_t *node)
{
    assert_exit(doubly_end_queue_list_structure_legal_p(node));

    return doubly_end_queue_list_reflect(node->link.next);
}

static inline s_doubly_end_queue_list_t *
doubly_end_queue_list_previous(s_doubly_end_queue_list_t *node)
{
    assert_exit(doubly_end_queue_list_structure_legal_p(node));

    return doubly_end_queue_list_reflect(node->link.previous);
}

static inline bool
doubly_end_queue_empty_ip(s_doubly_end_queue_t *queue)
{
    assert_exit(doubly_end_queue_structure_legal_p(queue));

    if (NULL == queue->front && NULL == queue->rear) {
        return true;
    } else {
        return false;
    }
}

bool
doubly_end_queue_empty_p(s_doubly_end_queue_t *queue)
{
    if (!doubly_end_queue_structure_legal_p(queue)) {
        return false;
    } else {
        return doubly_end_queue_empty_ip(queue);
    }
}

void
doubly_end_queue_front_enter(s_doubly_end_queue_t *queue, void *member)
{
    s_doubly_end_queue_list_t *list;

    if (doubly_end_queue_structure_legal_p(queue)) {
        list = memory_cache_allocate(sizeof(*list));
        list->val = member;

        if (doubly_end_queue_empty_ip(queue)) {
            queue->front = list;
            queue->rear = list;
            doubly_linked_list_initial(&queue->front->link);
        } else {
            doubly_linked_list_insert_before(&queue->front->link, &list->link);
            queue->front = list;
        }
    }
}

void
doubly_end_queue_rear_enter(s_doubly_end_queue_t *queue, void *member)
{
    s_doubly_end_queue_list_t *list;

    if (doubly_end_queue_structure_legal_p(queue)) {
        list = memory_cache_allocate(sizeof(*list));
        list->val = member;

        if (doubly_end_queue_empty_ip(queue)) {
            queue->front = list;
            queue->rear = list;
            doubly_linked_list_initial(&queue->front->link);
        } else {
            doubly_linked_list_insert_after(&queue->rear->link, &list->link);
            queue->rear = list;
        }
    }
}

void *
doubly_end_queue_front(s_doubly_end_queue_t *queue)
{
    if (!doubly_end_queue_structure_legal_p(queue)) {
        return PTR_INVALID;
    } else if (doubly_end_queue_empty_ip(queue)) {
        pr_log_warn("Attempt to leave from _EMPTY_ queue.\n");
        return PTR_INVALID;
    } else {
        return queue->front->val;
    }
}

void *
doubly_end_queue_rear(s_doubly_end_queue_t *queue)
{
    if (!doubly_end_queue_structure_legal_p(queue)) {
        return PTR_INVALID;
    } else if (doubly_end_queue_empty_ip(queue)) {
        pr_log_warn("Attempt to leave from _EMPTY_ queue.\n");
        return PTR_INVALID;
    } else {
        return queue->rear->val;
    }
}

void *
doubly_end_queue_front_leave(s_doubly_end_queue_t *queue)
{
    void *retval;
    s_doubly_linked_list_t *link;
    s_doubly_end_queue_list_t *next;

    if (!doubly_end_queue_structure_legal_p(queue)) {
        return PTR_INVALID;
    } else if (doubly_end_queue_empty_ip(queue)) {
        pr_log_warn("Attempt to leave from _EMPTY_ queue.\n");
        return PTR_INVALID;
    } else {
        retval = queue->front->val;
        next = doubly_end_queue_list_next(queue->front);

        if (next == queue->front) {
            doubly_end_queue_last_node_clean(queue);
        } else {
            link = &queue->front->link;
            doubly_linked_list_remove(&link);
            memory_cache_free(queue->front);
            queue->front = next;
        }
        return retval;
    }
}

void *
doubly_end_queue_rear_leave(s_doubly_end_queue_t *queue)
{
    void *retval;
    s_doubly_linked_list_t *link;
    s_doubly_end_queue_list_t *list;

    if (!doubly_end_queue_structure_legal_p(queue)) {
        return PTR_INVALID;
    } else {
        if (doubly_end_queue_empty_ip(queue)) {
            pr_log_warn("Attempt to leave from _EMPTY_ queue.\n");
            return PTR_INVALID;
        } else {
            retval = queue->rear->val;
            list = doubly_end_queue_list_previous(queue->rear);

            if (list == queue->rear) {
                doubly_end_queue_last_node_clean(queue);
            } else {
                link = &queue->rear->link;
                doubly_linked_list_remove(&link);
                memory_cache_free(queue->rear);
                queue->rear = list;
            }
            return retval;
        }
    }
}

static inline void
doubly_end_queue_last_node_clean(s_doubly_end_queue_t *queue)
{
    assert_exit(doubly_end_queue_structure_legal_p(queue));
    assert_exit(queue->front == queue->rear);

    memory_cache_free(queue->front);
    queue->front = NULL;
    queue->rear = NULL;
}

static inline void
doubly_end_queue_cleanup_i(s_doubly_end_queue_t *queue)
{
    s_doubly_end_queue_list_t *list;
    s_doubly_end_queue_list_t *next;
    s_doubly_linked_list_t *link;

    assert_exit(doubly_end_queue_structure_legal_p(queue));

    list = queue->front;

    while (list != queue->rear) {
        next = doubly_end_queue_list_next(list);
        link = &list->link;
        doubly_linked_list_remove(&link);
        memory_cache_free(list);

        list = next;
    }

    queue->front = list;
    doubly_end_queue_last_node_clean(queue);
}

void
doubly_end_queue_cleanup(s_doubly_end_queue_t *queue)
{
    if (!doubly_end_queue_structure_legal_p(queue)) {
        return;
    } else {
        doubly_end_queue_cleanup_i(queue);
    }
}

void
doubly_end_queue_iterate(s_doubly_end_queue_t *queue, void (*handle)(void *))
{
    s_doubly_end_queue_list_t *list;

    if (!doubly_end_queue_structure_legal_p(queue)) {
        return;
    } else if (doubly_end_queue_empty_ip(queue)) {
        pr_log_info("Iterate on _EMPTY_ queue, nothing will be done.\n");
    } else {
        list = queue->front;

        do {
            (*handle)(list->val);
            list = doubly_end_queue_list_next(list);
        } while (list != queue->front);
    }
}

s_bitmap_t *
bitmap_create(native_wide_t min, native_wide_t max)
{
    s_bitmap_t *bitmap;
    uint32 bytes_count;

    if (min > max) {
        return PTR_INVALID;
    } else {
        bitmap = memory_cache_allocate(sizeof(*bitmap));
        bitmap->max = max;
        bitmap->min = min;

        bytes_count = (bitmap->max - bitmap->min + 1) / 8;
        bytes_count = bitmap_native_align(bytes_count);

        bitmap->map = memory_cache_allocate(bytes_count);
        dp_memset(bitmap->map, 0, bytes_count);

        return bitmap;
    }
}

bool
bitmap_structure_legal_p(s_bitmap_t *bitmap)
{
    return bitmap_structure_legal_ip(bitmap);
}

bool
bitmap_structure_illegal_p(s_bitmap_t *bitmap)
{
    return bitmap_structure_illegal_ip(bitmap);
}

static inline bool
bitmap_structure_illegal_ip(s_bitmap_t *bitmap)
{
    return !bitmap_structure_legal_ip(bitmap);
}

static inline bool
bitmap_structure_legal_ip(s_bitmap_t *bitmap)
{
    if (NULL_PTR_P(bitmap)) {
        return false;
    } else if (PTR_INVALID == bitmap) {
        return false;
    } else if (NULL_PTR_P(bitmap->map)) {
        return false;
    } else {
        return true;
    }
}

void
bitmap_destroy(s_bitmap_t **bitmap)
{
    if (NULL_PTR_P(bitmap)) {
        return;
    } else if (bitmap_structure_legal_ip(*bitmap)) {
        memory_cache_free((*bitmap)->map);
        memory_cache_free((*bitmap));

        *bitmap = NULL;
    }
}

static inline native_wide_t
bitmap_map_index(s_bitmap_t *bitmap, native_wide_t val)
{
    native_wide_t index;

    assert_exit(bitmap_structure_legal_ip(bitmap));
    assert_exit(val >= bitmap->min && val <= bitmap->max);

    index = (val - bitmap->min) / BITMAP_NATIVE_BITS;

    return index;
}

static inline native_wide_t
bitmap_map_reminder(s_bitmap_t *bitmap, native_wide_t val)
{
    native_wide_t reminder;

    assert_exit(bitmap_structure_legal_ip(bitmap));
    assert_exit(val >= bitmap->min && val <= bitmap->max);

    reminder = (val - bitmap->min) % BITMAP_NATIVE_BITS;

    return reminder;
}

static inline uint32
bitmap_native_align(uint32 bytes_count)
{
    uint32 bytes_padding;
    uint32 bytes_count_aligned;

    bytes_padding = BITMAP_NATIVE_BYTES - bytes_count % BITMAP_NATIVE_BYTES;
    bytes_count_aligned = bytes_count + bytes_padding;

    return bytes_count_aligned;
}

static inline uint32
bitmap_native_bytes_count(s_bitmap_t *bitmap)
{
    uint32 bytes_count;

    assert_exit(bitmap_structure_legal_ip(bitmap));

    bytes_count = (bitmap->max - bitmap->min + 1) / 8;
    bytes_count = bitmap_native_align(bytes_count);

    return bytes_count;
}

static inline void
bitmap_map_expand(s_bitmap_t *bitmap, native_wide_t val)
{
    uint32 bytes_count;

    assert_exit(bitmap_structure_legal_ip(bitmap));
    assert_exit(val > bitmap->max);

    bytes_count = (val - bitmap->min) / 8;
    bytes_count = bitmap_native_align(bytes_count);

    bitmap->max = val;
    bitmap->map = memory_cache_re_allocate(bitmap->map, bytes_count);
}

static inline void
bitmap_bit_clear_i(s_bitmap_t *bitmap, native_wide_t val)
{
    native_wide_t index;
    native_wide_t reminder;

    assert_exit(bitmap_structure_legal_ip(bitmap));
    assert_exit(val >= bitmap->min);
    assert_exit(val <= bitmap->max);

    index = bitmap_map_index(bitmap, val);
    reminder = bitmap_map_reminder(bitmap, val);

    bitmap->map[index] &= (BITMAP_ALL ^ (BITMAP_SET << reminder));
}

static inline void
bitmap_bit_set_i(s_bitmap_t *bitmap, native_wide_t val)
{
    native_wide_t index;
    native_wide_t reminder;

    assert_exit(bitmap_structure_legal_ip(bitmap));
    assert_exit(val >= bitmap->min);

    if (val > bitmap->max) {
        bitmap_map_expand(bitmap, val);
    }

    index = bitmap_map_index(bitmap, val);
    reminder = bitmap_map_reminder(bitmap, val);

    bitmap->map[index] |= (BITMAP_SET << reminder);
}

static inline native_wide_t
bitmap_bit_get_i(s_bitmap_t *bitmap, native_wide_t val)
{
    native_wide_t bit;
    native_wide_t index;
    native_wide_t reminder;

    assert_exit(bitmap_structure_legal_ip(bitmap));
    assert_exit(val >= bitmap->min);
    assert_exit(val <= bitmap->max);

    index = bitmap_map_index(bitmap, val);
    reminder = bitmap_map_reminder(bitmap, val);

    bit = (bitmap->map[index] >> reminder) & BITMAP_SET;

    return bit;
}

native_wide_t
bitmap_bit_get(s_bitmap_t *bitmap, native_wide_t val)
{
    if (BITMAP_ILLEGAL_P(bitmap)) {
        return BITMAP_INVALID;
    } else if (val > bitmap->max) {
        return BITMAP_INVALID;
    } else if (val < bitmap->min) {
        return BITMAP_INVALID;
    } else {
        return bitmap_bit_get_i(bitmap, val);
    }
}

void
bitmap_bit_clear(s_bitmap_t *bitmap, native_wide_t val)
{
    if (BITMAP_ILLEGAL_P(bitmap)) {
        return;
    } else if (val > bitmap->max) {
        return;
    } else if (val < bitmap->min) {
        return;
    } else {
        bitmap_bit_clear_i(bitmap, val);
    }
}

void
bitmap_bit_set(s_bitmap_t *bitmap, native_wide_t val)
{
    if (BITMAP_ILLEGAL_P(bitmap)) {
        return;
    } else if (val < bitmap->min) {
        return;
    } else {
        bitmap_bit_set_i(bitmap, val);
    }
}

bool
bitmap_bit_set_p(s_bitmap_t *bitmap, native_wide_t val)
{
    if (BITMAP_ILLEGAL_P(bitmap)) {
        return false;
    } else if (val > bitmap->max) {
        return false;
    } else if (val < bitmap->min) {
        return false;
    } else {
        return bitmap_bit_get_i(bitmap, val) == BITMAP_SET;
    }
}

bool
bitmap_bit_clear_p(s_bitmap_t *bitmap, native_wide_t val)
{
    if (BITMAP_ILLEGAL_P(bitmap)) {
        return false;
    } else if (val > bitmap->max) {
        return false;
    } else if (val < bitmap->min) {
        return false;
    } else {
        return bitmap_bit_get_i(bitmap, val) == BITMAP_CLR;
    }
}

void
bitmap_map_cleanup(s_bitmap_t *bitmap)
{
    uint32 bytes_count;

    if (BITMAP_LEGAL_P(bitmap)) {
        bytes_count = bitmap_native_bytes_count(bitmap);
        dp_memset(bitmap->map, 0, bytes_count);
    }
}

/*
 * Index = SUM(key[size - i - 1] * (32 ^ i)), 0 <= i <= size - 1
 *     0x0 <= key[size - i - 1] <= 0xF
 */
uint32
hashing_function_polynomial(void *key, uint32 size)
{
    uint64 raw;
    uint64 retval;

    if (0 == size) {
        pr_log_err("Hash table should not have zero table size.\n");
    } else {
        raw = PTR_TO_UNSIGNED(key);

        retval = 0x0ull;
        while (0ull != raw) {
            retval = ((raw & 0xfull) << 5) + retval;
            raw = raw >> 4;
        }
    }

    return (uint32)retval % size;
}

static inline uint32
hashing_function_square(uint32 iter)
{
    assert_exit(iter < 0x10000);

    return iter * iter;
}

uint32
hashing_function_open_addressing(void *key, uint32 size, uint32 counter)
{
    uint64 retval;

    retval = hashing_function_polynomial(key, size);
    retval = (retval + hashing_function_square(counter)) % size;

    return (uint32)retval;
}

uint32
separate_chain_hash_size(s_separate_chain_hash_t *hash)
{
    if (!separate_chain_hash_structure_legal_p(hash)) {
        return HASH_SIZE_INVALID;
    } else {
        return hash->table->load.size;
    }
}

uint32
separate_chain_hash_load_factor_peak(s_separate_chain_hash_t *hash)
{
    if (!separate_chain_hash_structure_legal_p(hash)) {
        return HASH_LD_FTR_INVALID;
    } else {
        return hash->table->load.peak;
    }
}

static inline s_separate_chain_hash_t *
separate_chain_hash_create_i(uint32 size)
{
    s_separate_chain_hash_t *hash;

    hash = memory_cache_allocate(sizeof(*hash));

    if (complain_zero_size_p(size)) {
        size = SPT_CHN_HASH_SIZE_DFT;
        pr_log_warn("Hash table size not specified, use default size.\n");
    }

    hash->table = hashing_table_create(size);
    hash->table->func = &hashing_function_polynomial;
    hash->table->load.peak = SPT_CHN_HASH_LOAD_FTR;

    return hash;
}


s_separate_chain_hash_t *
separate_chain_hash_create(uint32 size)
{
    if (size == 0) {
        return PTR_INVALID;
    } else {
        return separate_chain_hash_create_i(size);
    }
}

static inline bool
separate_chain_hash_structure_legal_p(s_separate_chain_hash_t *hash)
{
    if (NULL_PTR_P(hash)) {
        return false;
    } else {
        return hashing_table_structure_legal_p(hash->table);
    }
}

static inline s_separate_chain_t *
separate_chain_create(void *val)
{
    s_separate_chain_t *chain;

    chain = memory_cache_allocate(sizeof(*chain));
    chain->val = val;

    doubly_linked_list_initial(&chain->list);

    return chain;
}

static inline bool
separate_chain_structure_legal_p(s_separate_chain_t *chain)
{
    if (NULL_PTR_P(chain)) {
        return false;
    } else if (!doubly_linked_list_structure_legal_p(&chain->list)) {
        return false;
    } else {
        return true;
    }
}

static inline s_separate_chain_t *
separate_chain_next(s_separate_chain_t *chain)
{
    ptr_t offset;

    if (!chain) {
        return NULL;
    } else {
        offset = (ptr_t)&chain->list - (ptr_t)chain;
        return (void *)((ptr_t)chain->list.next - offset);
    }
}

static inline s_separate_chain_t *
separate_chain_ptr_to_chain(s_doubly_linked_list_t *node)
{
    assert_exit(NON_NULL_PTR_P(node));

    return CONTAINER_OF(node, s_separate_chain_t, list);
}

static inline void
separate_chain_destroy(s_separate_chain_t *chain)
{
    s_separate_chain_t *tmp;
    s_separate_chain_t *next;

    assert_exit(separate_chain_structure_legal_p(chain));

    tmp = chain;
    do {
        next = separate_chain_next(tmp);
        memory_cache_free(tmp);
        tmp = next;
    } while (tmp != chain);
}

static inline void
separate_chain_hash_chain_destroy(s_separate_chain_hash_t *hash)
{
    void *chain;
    void **chain_array;

    assert_exit(separate_chain_hash_structure_legal_p(hash));

    chain_array = hash->table->space;
    while (chain_array < hash->table->space + hash->table->load.size) {
        chain = *chain_array;
        if (chain) {
            separate_chain_destroy(chain);
        }
        chain_array++;
    }
}

static inline void
separate_chain_hash_destroy_i(s_separate_chain_hash_t *hash)
{
    assert_exit(separate_chain_hash_structure_legal_p(hash));

    separate_chain_hash_chain_destroy(hash);
    hashing_table_destroy(&hash->table);
    memory_cache_free(hash);
}


void
separate_chain_hash_destroy(s_separate_chain_hash_t **hash)
{
    if (NULL_PTR_P(hash)) {
        return;
    } else if (!separate_chain_hash_structure_legal_p(*hash)) {
        return;
    } else {
        separate_chain_hash_destroy_i(*hash);
        *hash = NULL;
    }
}

uint32
separate_chain_hash_load_factor(s_separate_chain_hash_t *hash)
{
    if (!separate_chain_hash_structure_legal_p(hash)) {
        return 0u;
    } else {
        return hashing_table_load_factor(hash->table);
    }
}

static inline uint32
separate_chain_hash_index_calculate(s_separate_chain_hash_t *hash, void *key)
{
    uint32 index;

    assert_exit(separate_chain_hash_structure_legal_p(hash));

    index = hash->table->separate_chain(key, hash->table->load.size);

    assert_exit(index < hash->table->load.size);
    return index;
}

static inline void *
separate_chain_hash_insert_i(s_separate_chain_hash_t *hash, void *key)
{
    uint32 index;
    uint32 factor;
    s_separate_chain_t *head;
    s_separate_chain_t *chain_tmp;

    assert_exit(!NULL_PTR_P(key));
    assert_exit(separate_chain_hash_structure_legal_p(hash));

    factor = hashing_table_load_factor(hash->table);
    if (factor >= hash->table->load.peak) {
        pr_log_info("Reach the load factor limit, will rehashing.\n");
        separate_chain_hash_rehashing_i(hash);
    }

    chain_tmp = separate_chain_create(key);
    index = separate_chain_hash_index_calculate(hash, key);
    head = hash->table->space[index];

    if (!head) {
        hash->table->load.amount++;
        hash->table->space[index] = chain_tmp;
    } else {
        doubly_linked_list_insert_after(&head->list, &chain_tmp->list);
    }

    return key;
}

void *
separate_chain_hash_insert(s_separate_chain_hash_t *hash, void *key)
{
    if (!separate_chain_hash_structure_legal_p(hash)) {
        return PTR_INVALID;
    } else if (NULL_PTR_P(key)) {
        return PTR_INVALID;
    } else {
        return separate_chain_hash_insert_i(hash, key);
    }
}

static inline void *
separate_chain_hash_remove_i(s_separate_chain_hash_t *hash, void *key)
{
    uint32 index;
    s_doubly_linked_list_t *removed, *list;
    s_separate_chain_t *head, *chain, *next;

    assert_exit(NON_NULL_PTR_P(key));
    assert_exit(separate_chain_hash_structure_legal_p(hash));

    index = separate_chain_hash_index_calculate(hash, key);
    head = hash->table->space[index];

    if (head != NULL) {
        chain = head;
        do {
            next = separate_chain_next(chain);
            if (key == chain->val) {
                list = &chain->list;
                removed = doubly_linked_list_remove(&list);
                memory_cache_free(separate_chain_ptr_to_chain(removed));

                if (list) {
                    hash->table->space[index] = separate_chain_ptr_to_chain(list);
                } else {
                    hash->table->space[index] = NULL;
                    hash->table->load.amount--;
                }

                return key;
            }
            chain = next;
        } while (chain != head);
    }

    pr_log_warn("Failed to find key in hash table.\n");
    return NULL;
}

void *
separate_chain_hash_remove(s_separate_chain_hash_t *hash, void *key)
{
    if (!separate_chain_hash_structure_legal_p(hash)) {
        return PTR_INVALID;
    } else if (NULL_PTR_P(key)) {
        return PTR_INVALID;
    } else {
        return separate_chain_hash_remove_i(hash, key);
    }
}

void *
separate_chain_hash_find(s_separate_chain_hash_t *hash, void *key)
{
    uint32 index;
    s_separate_chain_t *head;
    s_separate_chain_t *chain;

    if (!separate_chain_hash_structure_legal_p(hash)) {
        return PTR_INVALID;
    } else if (NULL_PTR_P(key)) {
        return PTR_INVALID;
    } else {
        index = separate_chain_hash_index_calculate(hash, key);
        head = hash->table->space[index];

        if (head) {
            chain = head;
            do {
                if (key == chain->val) {
                    return key;
                }
                chain = separate_chain_next(chain);
            } while (chain != head);
        }

        pr_log_info("Not such a key in given hash.\n");
        return NULL;
    }
}

static inline void
separate_chain_hash_rehashing_i(s_separate_chain_hash_t *hash)
{
    void **chain;
    uint32 new_size;
    s_separate_chain_t *head, *list;
    s_separate_chain_hash_t *new_hash;

    assert_exit(separate_chain_hash_structure_legal_p(hash));

    new_size = prime_numeral_next(hash->table->load.size + 1);
    new_hash = separate_chain_hash_create_i(new_size);

    chain = hash->table->space;
    while (chain < hash->table->space + hash->table->load.size) {
        if (*chain) {
            head = list = *chain;
            do {
                separate_chain_hash_insert_i(new_hash, list->val);
                list = separate_chain_next(list);
            } while (head != list);
        }
        chain++;
    }

    /* swap space of hash and new */
    chain = hash->table->space;
    hash->table->space = new_hash->table->space;
    new_hash->table->space = chain;

    new_hash->table->load.size = hash->table->load.size;
    hash->table->load.size = new_size;

    separate_chain_hash_destroy_i(new_hash);
}

void
separate_chain_hash_rehashing(s_separate_chain_hash_t *hash)
{
    if (separate_chain_hash_structure_legal_p(hash)) {
        separate_chain_hash_rehashing_i(hash);
    }
}

uint32
open_addressing_hash_size(s_open_addressing_hash_t *hash)
{
    if (!open_addressing_hash_structure_legal_p(hash)) {
        return HASH_SIZE_INVALID;
    } else {
        return hash->table->load.size;
    }
}

uint32
open_addressing_hash_load_factor_peak(s_open_addressing_hash_t *hash)
{
    if (!open_addressing_hash_structure_legal_p(hash)) {
        return HASH_LD_FTR_INVALID;
    } else {
        return hash->table->load.peak;
    }
}

static inline s_open_addressing_hash_t *
open_addressing_hash_create_i(uint32 size)
{
    s_open_addressing_hash_t *hash;

    hash = memory_cache_allocate(sizeof(*hash));

    if (complain_zero_size_p(size)) {
        size = SPT_CHN_HASH_SIZE_DFT;
        pr_log_warn("Hash table size not specified, use default size.\n");
    }

    /* open addressing requires prime table size. */
    hash->table = hashing_table_create(prime_numeral_next(size));
    hash->table->func = &hashing_function_open_addressing;
    hash->table->load.peak = OPN_ADDR_HASH_LOAD_FTR;

    return hash;
}

s_open_addressing_hash_t *
open_addressing_hash_create(uint32 size)
{
    if (size == 0) {
        return PTR_INVALID;
    } else {
        return open_addressing_hash_create_i(size);
    }
}

static inline bool
open_addressing_hash_structure_legal_p(s_open_addressing_hash_t *hash)
{
    if (NULL_PTR_P(hash)) {
        return false;
    } else {
        return hashing_table_structure_legal_p(hash->table);
    }
}

static inline void
open_addressing_hash_destroy_i(s_open_addressing_hash_t *hash)
{
    assert_exit(open_addressing_hash_structure_legal_p(hash));

    hashing_table_destroy(&hash->table);
    memory_cache_free(hash);
}

void
open_addressing_hash_destroy(s_open_addressing_hash_t **hash)
{
    if (NULL_PTR_P(hash)) {
        return;
    } else if (open_addressing_hash_structure_legal_p(*hash)) {
        open_addressing_hash_destroy_i(*hash);
        *hash = NULL;
    }
}

uint32
open_addressing_hash_load_factor(s_open_addressing_hash_t *hash)
{
    if (!open_addressing_hash_structure_legal_p(hash)) {
        return 0u;
    } else {
        return hashing_table_load_factor(hash->table);
    }
}

static inline uint32
open_addressing_hash_index_calculate(s_open_addressing_hash_t *hash,
    void *key, uint32 counter)
{
    uint32 index;

    assert_exit(open_addressing_hash_structure_legal_p(hash));
    assert_exit(counter < open_addressing_hash_limit(hash));

    index = hash->table->open_addressing(key, hash->table->load.size, counter);

    assert_exit(index < hash->table->load.size);
    return index;
}

void *
open_addressing_hash_insert_i(s_open_addressing_hash_t *hash, void *key)
{
    uint32 i;
    uint32 index;
    uint32 factor;

    assert_exit(!NULL_PTR_P(key));
    assert_exit(open_addressing_hash_structure_legal_p(hash));

    factor = hashing_table_load_factor(hash->table);
    if (factor >= hash->table->load.peak) {
        pr_log_info("Reach the load factor limit, will rehashing.\n");
        open_addressing_hash_rehashing_i(hash);
    }

    i = 0;
    do {
        index = open_addressing_hash_index_calculate(hash, key, i++);
    } while (hash->table->space[index]);

    hash->table->load.amount++;
    hash->table->space[index] = key;

    return key;
}

void *
open_addressing_hash_insert(s_open_addressing_hash_t *hash, void *key)
{
    if (!open_addressing_hash_structure_legal_p(hash)) {
        return PTR_INVALID;
    } else if (NULL_PTR_P(key)) {
        return PTR_INVALID;
    } else {
        return open_addressing_hash_insert_i(hash, key);
    }
}

void *
open_addressing_hash_remove(s_open_addressing_hash_t *hash, void *key)
{
    uint32 index;

    if (!open_addressing_hash_structure_legal_p(hash)) {
        return PTR_INVALID;
    } else if (NULL_PTR_P(key)) {
        return PTR_INVALID;
    } else {
        index = open_addressing_hash_find_index(hash, key);
        if (HASH_IDX_INVALID == index) {
            return NULL;
        } else {
            hash->table->load.amount--;
            hash->table->space[index] = NULL;
            return key;
        }
    }
}

static inline uint32
open_addressing_hash_find_index(s_open_addressing_hash_t *hash, void *key)
{
    uint32 i;
    uint32 index;

    assert_exit(!NULL_PTR_P(key));
    assert_exit(open_addressing_hash_structure_legal_p(hash));

    i = 0;
    do {
        index = open_addressing_hash_index_calculate(hash, key, i++);
        if (key == hash->table->space[index]) {
            return index;
        }
    } while (hash->table->space[index]);

    pr_log_info("Not such a key in given hash.\n");
    return HASH_IDX_INVALID;
}

void *
open_addressing_hash_find(s_open_addressing_hash_t *hash, void *key)
{
    if (!open_addressing_hash_structure_legal_p(hash)) {
        return PTR_INVALID;
    } else if (NULL_PTR_P(key)) {
        return PTR_INVALID;
    } else if (HASH_IDX_INVALID == open_addressing_hash_find_index(hash, key)) {
        return NULL;
    } else {
        return key;
    }
}

static inline void
open_addressing_hash_rehashing_i(s_open_addressing_hash_t *hash)
{
    void **i;
    uint32 new_size;
    s_open_addressing_hash_t *new_hash;

    assert_exit(open_addressing_hash_structure_legal_p(hash));

    new_size = prime_numeral_next(hash->table->load.size + 1);
    new_hash = open_addressing_hash_create_i(new_size);

    i = hash->table->space;
    while (i < hash->table->space + hash->table->load.size) {
        if (*i) {
            open_addressing_hash_insert_i(new_hash, *i);
        }
        i++;
    }


    /* swap space of hash and new */
    i = hash->table->space;
    hash->table->space = new_hash->table->space;
    new_hash->table->space = i;

    new_hash->table->load.size = hash->table->load.size;
    hash->table->load.size = new_size;

    open_addressing_hash_destroy_i(new_hash);
}

void
open_addressing_hash_rehashing(s_open_addressing_hash_t *hash)
{
    if (open_addressing_hash_structure_legal_p(hash)) {
        open_addressing_hash_rehashing_i(hash);
    }
}

static inline s_hashing_table_t *
hashing_table_create(uint32 size)
{
    s_hashing_table_t *table;

    assert_exit(!complain_zero_size_p(size));

    table = memory_cache_allocate(sizeof(*table));

    table->load.size = size;
    table->load.amount = 0;

    hashing_table_initial(table);

    return table;
}

static inline bool
hashing_table_structure_legal_p(s_hashing_table_t *table)
{
    if (NULL_PTR_P(table)) {
        return false;
    } else if (NULL_PTR_P(table->space)) {
        return false;
    } else if (NULL_PTR_P(table->func)) {
        return false;
    } else if (complain_zero_size_p(table->load.size)) {
        return false;
    } else if (table->load.peak > 100) {
        return false;
    } else {
        return true;
    }
}

static inline void
hashing_table_initial(s_hashing_table_t *table)
{
    uint32 size;

    assert_exit(NON_NULL_PTR_P(table));

    size = sizeof(*table->space) * table->load.size;

    table->space = memory_cache_allocate(size);
    dp_memset(table->space, 0, size);
}

static inline void
hashing_table_destroy(s_hashing_table_t **table)
{
    assert_exit(!NULL_PTR_P(table));
    assert_exit(hashing_table_structure_legal_p(*table));

    memory_cache_free((*table)->space);
    memory_cache_free((*table));
    *table = NULL;
}

static inline uint32
hashing_table_load_factor(s_hashing_table_t *table)
{
    uint32 load_factor;

    assert_exit(hashing_table_structure_legal_p(table));

    load_factor = HASHING_TABLE_LOAD_FACTOR(table);
    assert_exit(load_factor == hashing_table_load_factor_calculate(table));

    return load_factor;
}

static inline uint32
open_addressing_hash_limit(s_open_addressing_hash_t *hash)
{
    uint32 limit;

    assert_exit(open_addressing_hash_structure_legal_p(hash));

    limit = hash->table->load.size * hash->table->load.peak;
    limit = limit / 100u;

    return limit;
}

static inline uint32
hashing_table_load_factor_calculate(s_hashing_table_t *table)
{
    void **iter;
    uint32 load_factor;

    assert_exit(hashing_table_structure_legal_p(table));

    load_factor = 0u;
    iter = table->space;

    while (iter < table->space + table->load.size) {
        if (NULL != *iter) {
            load_factor++;
        }
        iter++;
    }

    return (load_factor * 100 / table->load.size);
}

s_array_iterator_t *
array_iterator_create(void)
{
    s_array_iterator_t *iterator;

    iterator = dp_malloc(sizeof(*iterator));

    iterator->fp_index_initial = NULL;
    iterator->fp_next_exist_p = NULL;
    iterator->fp_next_obtain = NULL;

    return iterator;
}

bool
array_iterator_structure_legal_p(s_array_iterator_t *iterator)
{
    return array_iterator_structure_legal_ip(iterator);
}

static inline bool
array_iterator_structure_legal_ip(s_array_iterator_t *iterator)
{
    if (NULL_PTR_P(iterator)) {
        return false;
    } else if (!iterator->fp_index_initial) {
        return false;
    } else if (!iterator->fp_next_exist_p) {
        return false;
    } else if (!iterator->fp_next_obtain) {
        return false;
    } else {
        return true;
    }
}

void
array_iterator_initial(s_array_iterator_t *iterator,
               f_array_iterator_initial_t fp_index_initial,
            f_array_iterator_next_exist_t fp_next_exist_p,
           f_array_iterator_next_obtain_t fp_next_obtain)
{
    if (NULL_PTR_P(iterator)) {
        return;
    }

    /* pr_log_err will exit program */
    if (NULL_PTR_P(fp_index_initial)) {
        pr_log_err("Function pointer 'fp_index_initial' is NULL.");
    }

    if (NULL_PTR_P(fp_next_exist_p)) {
        pr_log_err("Function pointer 'fp_next_exist_p' is NULL.");
    }

    if (NULL_PTR_P(fp_next_obtain)) {
        pr_log_err("Function pointer 'fp_next_obtain' is NULL.");
    }

    iterator->fp_index_initial = fp_index_initial;
    iterator->fp_next_exist_p = fp_next_exist_p;
    iterator->fp_next_obtain = fp_next_obtain;
}

void
array_iterator_destroy(s_array_iterator_t *iterator)
{
    if (array_iterator_structure_legal_ip(iterator)) {
        iterator->fp_index_initial = NULL;
        iterator->fp_next_exist_p = NULL;
        iterator->fp_next_obtain = NULL;

        dp_free(iterator);
    }
}

void
swap_pointer(void **ptr_a, void **ptr_b)
{
    void *tmp;

    if (complain_null_pointer_p(ptr_a)) {
        return;
    } else if (complain_null_pointer_p(ptr_b)) {
        return;
    } else if (*ptr_a == *ptr_b) {
        return;
    } else {
        // TODO, CMPXCHG optimization
        tmp = *ptr_a;
        *ptr_a = *ptr_b;
        *ptr_b = tmp;
    }
}

/*
 * convert_string_to_uint32_array has a output argv 'uint32 *len'.
 */
uint32 *
convert_string_to_uint32_array(char *string, uint32 *len)
{
    uint32 *converted;
    uint32 i, string_len;

    if (!string || !len) {
        return PTR_INVALID;
    } else {
        string_len = dp_strlen(string);
        converted = dp_malloc(sizeof(*converted) * string_len);

        i = 0;
        while (i < string_len) {
            converted[i] = (uint32)string[i];
            i++;
        }

        *len = string_len;
        return converted;
    }
}

void *
malloc_wrap(uint32 size)
{
    malloc_cnt++;

    return dp_malloc(size);
}

void *
realloc_wrap(void *ptr, uint32 size)
{
    realloc_cnt++;

    return dp_realloc(ptr, size);
}

void
free_wrap(void *ptr)
{
    free_cnt++;
    dp_free(ptr);
}

void *
memory_cache_allocate(uint32 size)
{
    void *retval;

    if (complain_zero_size_p(size)) {
        return NULL;
    } else if (memory_cache_find_entry_of_size_p(&retval, size)) {
        memory_cache_remove_entry_by_index(mem_cc_set.index);
        return retval;
    } else if (memory_cache_find_entry_greater_size_p(&retval, size)) {
        memory_cache_remove_entry_by_index(mem_cc_set.index);
        return retval;
    } else {
        retval = malloc_ds(size + MEM_PRE_LEN);

        if (!complain_no_memory_p(retval)) {
            *(uint32 *)retval = size;
            return retval + MEM_PRE_LEN;
        } else {
            return NULL;
        }
    }
}

void *
memory_cache_re_allocate(void *addr, uint32 size)
{
    uint32 old_size;
    void *new_addr;

    if (complain_zero_size_p(size)) {
        return NULL;
    } else if (NULL_PTR_P(addr)) {
        return memory_cache_allocate(size);
    } else {
        old_size = *(uint32 *)MEM_TO_REAL(addr);
        assert_exit(0 != old_size);

        if (size <= old_size) {
            return addr;
        } else {
            new_addr = realloc_ds(MEM_TO_REAL(addr), size + MEM_PRE_LEN);
            *(uint32 *)new_addr = size;
            return new_addr + MEM_PRE_LEN;
        }
    }
}

void
memory_cache_free(void *addr)
{
    uint32 size;

    if (!NULL_PTR_P(addr)) {
        size = *(uint32 *)(MEM_TO_REAL(addr));
        assert_exit(0 < size);

        if (!memory_cache_add_entry_p(addr, size)) {
            free_ds(MEM_TO_REAL(addr));
        }
    }
}

static inline void
memory_cache_remove_entry_by_index(uint32 index)
{
    assert_exit(index < MEM_ENTRY_CNT);

    mem_cc_set.list[index].addr = NULL;
    mem_cc_set.list[index].size = MEM_ENTRY_NULL;
}

static inline bool
memory_cache_add_entry_p(void *addr, uint32 size)
{
    uint32 index_start;
    struct memory_cache_entry *cache;

    assert_exit(NULL != addr);
    assert_exit(!complain_zero_size_p(size));

    index_start = mem_cc_set.index;
    cache = mem_cc_set.list;

    do {
        if (MEM_ENTRY_NULL == cache[index_start].size) {
            cache[index_start].addr = addr;
            cache[index_start].size = size;
            return true;
        }

        index_start++;
        if (MEM_ENTRY_CNT == index_start) {
            index_start = 0;
        }
    } while (index_start != mem_cc_set.index);

    return false;
}

static inline bool
memory_cache_find_entry_of_size_p(void **addr, uint32 size)
{
    uint32 index_start;

    assert_exit(!complain_zero_size_p(size));

    index_start = mem_cc_set.index;

    while (index_start < MEM_ENTRY_CNT) {
        if (size == mem_cc_set.list[index_start].size) {
            *addr = mem_cc_set.list[index_start].addr;
            mem_cc_set.index = index_start;
            return true;
        }
        index_start++;
    }

    mem_cc_set.index = 0;

    return false;
}

static inline bool
memory_cache_find_entry_greater_size_p(void **addr, uint32 size)
{
    uint32 index_start;

    assert_exit(!complain_zero_size_p(size));

    index_start = mem_cc_set.index;

    while (index_start < MEM_ENTRY_CNT) {
        if (size <= mem_cc_set.list[index_start].size) {
            *addr = mem_cc_set.list[index_start].addr;
            mem_cc_set.index = index_start;
            return true;
        }
        index_start++;
    }

    mem_cc_set.index = 0;

    return false;
}

void
memory_cache_cleanup(void)
{
    uint32 index_start;

    index_start = 0;
    while (index_start < MEM_ENTRY_CNT) {
        if (MEM_ENTRY_NULL != mem_cc_set.list[index_start].size) {
            free_ds(MEM_TO_REAL(mem_cc_set.list[index_start].addr));
        }
        index_start++;
    }
}

uint32
random_uint32_with_limit(uint32 limit)
{
    if (limit) {
        return random_uint32_limited(limit);
    } else {
        return 0;
    }
}

static inline uint32
random_uint32_limited(uint32 limit)
{
    assert_exit(!complain_zero_size_p(limit));

    return (uint32)dp_rand() % limit;
}

sint64
random_sint64(void)
{
    return (sint64)dp_rand() - (sint64)dp_rand();
}

uint32 *
random_sequence_uint32_limited_obtain(uint32 len, uint32 limit)
{
    uint32 i;
    uint32 *sequence;

    if (!len || !limit) {
        return PTR_INVALID;
    } else {
        sequence = dp_malloc(sizeof(*sequence) * len);

        i = 0;
        while (i < len) {
            sequence[i++] = random_uint32_limited(limit);
        }

        return sequence;
    }
}

uint32 *
random_sequence_uint32_obtain(uint32 len)
{
    uint32 i;
    uint32 *sequence;

    if (!len) {
        return PTR_INVALID;
    } else {
        sequence = dp_malloc(sizeof(*sequence) * len);

        i = 0;
        while (i < len) {
            sequence[i++] = dp_rand();
        }

        return sequence;
    }
}

void
random_sequence_drop(uint32 *sequence)
{
    if (!NULL_PTR_P(sequence)) {
        dp_free(sequence);
    }
}

bool
complain_null_pointer_p(void *ptr)
{
    if (NULL == ptr) {
        pr_log_warn("Attempt to access NULL pointer.\n");
        return true;
    } else {
        return false;
    }
}

bool
complain_null_string_p(char *string)
{
    if (NULL == string) {
        pr_log_warn("Attempt to access NULL pointer.\n");
        return true;
    } else if (string[0] == '\0') {
        return true;
    } else {
        return false;
    }
}

bool
complain_no_memory_p(void *ptr)
{
    if (NULL == ptr) {
        pr_log_err("Fail to get memory from system.\n");
    }

    return false;
}

bool
complain_zero_size_p(uint32 size)
{
    if (0 == size) {
        pr_log_warn("Zero size detected.\n");
        return true;
    } else {
        return false;
    }
}

void
complain_assert_caution(char *msg, const char *fname, const char *func,
    uint32 line)
{
    if (msg && fname && func) {
        dp_printf("COMPLAIN ASSERTION: => %s Fail ... \n"
                  "  In function %s\n"
                  "  At file %s +%d\n", msg, func, fname, line);
    }
}

void
complain_assert_exit(char *msg, const char *fname, const char *func,
    uint32 line)
{
    complain_assert_caution(msg, fname, func, line);
    *(uint32 *)0 = 0;
}

void
libds_log_print(enum log_level lvl, const char *msg)
{
    assert_exit(NULL != logfile);

    if (msg) {
        switch (lvl) {
            case INFO:
                dp_fprintf(logfile, " >> %4s: %s", SYM_2_STR(INFO), msg);
                break;
            case WARN:
                dp_fprintf(logfile, " >> %4s: %s", SYM_2_STR(WARN), msg);
                break;
            case DBUG:
                dp_fprintf(logfile, " >> %4s: %s", SYM_2_STR(DBUG), msg);
                break;
            case ERRR:
                dp_fprintf(stdout,  " >> %4s: %s", SYM_2_STR(ERRR), msg);
                break;
            default:
                msg = "Unknown enum value of enum log_level.\n";
                dp_fprintf(logfile, " >> %4s: %s", SYM_2_STR(WARN), msg);
                break;
        }
    }
}

void
libds_log_file_create(void)
{
    logfile = dp_fopen(LOG_FNAME, "w");

    if (!logfile) {
        pr_log_err("Failed to create logfile.\n");
    }
}

void
libds_log_file_close(void)
{
    if (logfile) {
        dp_fclose(logfile);
        libds_log_memory_stat_print();
    }
}

static inline void
libds_log_memory_stat_print(void)
{
    dp_printf("  >> LIBDS Memory Usage Summary <<\n");
    dp_printf("    . Malloc  %10"U64_MDF_U"\n", malloc_cnt);
    dp_printf("    . Realloc %10"U64_MDF_U"\n", realloc_cnt);
    dp_printf("    . Free    %10"U64_MDF_U"\n", free_cnt);
    dp_printf("  >> Memory End <<\n\n");

    if (free_cnt != malloc_cnt) {
        dp_printf("Memory leak at dp_malloc %10"U64_MDF_U" <=> dp_free %10"U64_MDF_U"\n",
            malloc_cnt, free_cnt);
        dp_exit(1);
    }
}


uint32
prime_numeral_next(uint32 prime)
{
    uint32 *iter;

    if (prime < FIXED_PRIME_MAX) {
        iter = (uint32 *)fixed_prime;
        while (iter < fixed_prime + array_sizeof(fixed_prime)) {
            if (prime <= *iter) {
                break;
            }
            iter++;
        }
        return *iter;
    } else {
        return prime_numeral_calculate_next(prime);
    }
}

static inline uint32
prime_numeral_calculate_next(uint32 prime)
{
    uint32 start;
    uint32 next;
    uint32 limit;
    uint32 retval;

    prime |= 0x1u;

    retval = 0;
    next = prime + 2;
    while (next != UINT_MAX) {
        start = 3;
        limit = (uint32)dp_sqrt(next);
        while (start < limit) {
            if (0u == next % start) {
                break;
            }
            start += 2;
        }

        if (start < limit) {
            /* If next is not PRIME */
            next += 2;
        } else {
            retval = next;
            break;
        }
    }

    if (0 == retval) {
        pr_log_warn("Failed to find next PRIME numeral less than UINT_MAX.\n");
    }

    return retval;
}

static inline struct binary_heap *
binary_heap_create(uint32 capacity)
{
    struct binary_heap *heap;
    register struct heap_data **iterator;

    if (complain_zero_size_p(capacity)) {
        capacity = DEFAULT_BINARY_HEAP_SIZE;
    }

    heap = memory_cache_allocate(sizeof(*heap));
    heap->base = memory_cache_allocate(sizeof(*iterator) * (capacity + 1));

    heap->capacity = capacity;
    heap->size = 0;

    iterator = heap->base;
    while (iterator < heap->base + u_offset(capacity, 1)) {
        *iterator++ = NULL;
    }

    return heap;
}

static inline void
binary_heap_destroy(struct binary_heap **heap)
{
    assert_exit(NULL != heap);
    assert_exit(binary_heap_structure_legal_p(*heap));

    binary_heap_cleanup(*heap);
    memory_cache_free((*heap)->base);
    memory_cache_free(*heap);

    *heap = NULL;
}

static inline bool
binary_heap_empty_p(struct binary_heap *heap)
{
    assert_exit(binary_heap_structure_legal_p(heap));

    return heap->size == 0 ? true : false;
}

static inline bool
binary_heap_full_p(struct binary_heap *heap)
{
    assert_exit(binary_heap_structure_legal_p(heap));

    return heap->size == heap->capacity ? true : false;
}

static inline void
binary_heap_cleanup(struct binary_heap *heap)
{
    register uint32 index;

    assert_exit(binary_heap_structure_legal_p(heap));

    index = INDEX_FIRST;

    while (index <= INDEX_LAST(heap)) {
        memory_cache_free(HEAP_DATA(heap, index));
        HEAP_DATA(heap, index) = NULL;
        index++;
    }

    heap->size = 0;
}

static inline void *
binary_heap_root(struct binary_heap *heap)
{
    assert_exit(binary_heap_structure_legal_p(heap));

    return HEAP_VAL(heap, INDEX_ROOT);
}

static inline void
binary_heap_capacity_extend(struct binary_heap *heap)
{
    struct heap_data **new;
    uint32 size;

    assert_exit(binary_heap_structure_legal_p(heap));

    size = sizeof(heap->base[0]) * u_offset(heap->capacity * 2, 1);
    new = memory_cache_allocate(size);
    memset(new, 0, size);

    size = sizeof(heap->base[0]) * u_offset(heap->capacity, 1);
    dp_memcpy(new, heap->base, size);

    heap->capacity = heap->capacity * 2;
    memory_cache_free(heap->base);
    heap->base = new;

    assert_exit(binary_heap_structure_legal_p(heap));
}

static inline void
binary_heap_node_create_by_index(struct binary_heap *heap, uint32 index,
    sint64 nice, void *val)
{
    assert_exit(NULL_PTR_P(heap->base[index]));
    assert_exit(binary_heap_nice_legal_p(nice));
    assert_exit(binary_heap_index_legal_p(heap, index));
    assert_exit(binary_heap_structure_legal_p(heap));

    HEAP_DATA(heap, index) = binary_heap_node_create(nice, val);
}

static inline struct heap_data *
binary_heap_node_create(sint64 nice, void *val)
{
    struct heap_data *retval;

    assert_exit(binary_heap_nice_legal_p(nice));

    retval = memory_cache_allocate(sizeof(*retval));
    retval->nice = nice;
    retval->val = val;

    return retval;
}

/*
 * index      - specific the empty hole index of heap.
 * nice       - nice value of percolate up.
 * heap_order - function pointer of heap order, should be one of function
 *              in file binary_heap_order.c.
 * RETURN the re-ordered index of heap with that nice.
 */
static inline uint32
binary_heap_reorder(struct binary_heap *heap, uint32 index, sint64 nice,
    void *heap_order)
{
    uint32 index_next;
    bool (*order)(struct binary_heap *, uint32, sint64, uint32 *);

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));
    assert_exit(binary_heap_nice_legal_p(nice));

    order = heap_order;

    while (!(*order)(heap, index, nice, &index_next)) {
        HEAP_DATA(heap, index) = HEAP_DATA(heap, index_next);
        index = index_next;
    }

    HEAP_DATA(heap, index) = NULL;
    return index;
}

static inline bool
binary_heap_child_exist_p(struct binary_heap *heap, uint32 index)
{
    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    return INDEX_L_CHILD(index) <= INDEX_LAST(heap) ? true : false;
}

static inline uint32
binary_heap_child_min_nice_index(struct binary_heap *heap, uint32 index)
{
    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    if (!binary_heap_child_exist_p(heap, index)) {
        return INDEX_INVALID;
    } else if (INDEX_R_CHILD(index) > INDEX_LAST(heap)) {
        return INDEX_L_CHILD(index);
    } else if (HEAP_L_CHILD_NICE(heap, index) > HEAP_R_CHILD_NICE(heap, index)) {
        return INDEX_R_CHILD(index);
    } else {
        return INDEX_L_CHILD(index);
    }
}

static inline uint32
binary_heap_child_max_nice_index(struct binary_heap *heap, uint32 index)
{
    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    if (!binary_heap_child_exist_p(heap, index)) {
        return INDEX_INVALID;
    } else if (INDEX_R_CHILD(index) > INDEX_LAST(heap)) {
        return INDEX_L_CHILD(index);
    } else if (HEAP_L_CHILD_NICE(heap, index) < HEAP_R_CHILD_NICE(heap, index)) {
        return INDEX_R_CHILD(index);
    } else {
        return INDEX_L_CHILD(index);
    }
}

static inline uint32
binary_heap_gdp_randchild_min_nice_index(struct binary_heap *heap, uint32 index)
{
    uint32 begin;
    uint32 ret_index;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));
    assert_exit(binary_heap_depth_even_p(heap, index));

    if (!binary_heap_child_exist_p(heap, index)) {
        return INDEX_INVALID;
    } else if (INDEX_LL_CHILD(index) > INDEX_LAST(heap)) {
        return binary_heap_child_min_nice_index(heap, index);
    } else {
        begin = INDEX_LL_CHILD(index);
        ret_index = binary_heap_serial_small_nice_index(heap, begin, 4);

        if (HEAP_NICE(heap, INDEX_R_CHILD(index))
            < HEAP_NICE(heap, ret_index)) {
            return INDEX_R_CHILD(index);
        } else {
            return ret_index;
        }
    }
}

static inline uint32
binary_heap_gdp_randchild_max_nice_index(struct binary_heap *heap, uint32 index)
{
    uint32 begin;
    uint32 ret_index;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));
    assert_exit(binary_heap_depth_odd_p(heap, index));

    if (!binary_heap_child_exist_p(heap, index)) {
        return INDEX_INVALID;
    } else if (INDEX_LL_CHILD(index) > INDEX_LAST(heap)) {
        return binary_heap_child_max_nice_index(heap, index);
    } else {
        begin = INDEX_LL_CHILD(index);
        ret_index = binary_heap_serial_big_nice_index(heap, begin, 4);

        if (HEAP_NICE(heap, INDEX_R_CHILD(index))
            > HEAP_NICE(heap, ret_index)) {
            return INDEX_R_CHILD(index);
        } else {
            return ret_index;
        }
    }
}

static inline uint32
binary_heap_serial_small_nice_index(struct binary_heap *heap,
    uint32 index, uint32 count)
{
    uint32 small_index;
    uint32 rest;

    assert_exit(!complain_zero_size_p(count));
    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    small_index = index;
    rest = INDEX_LAST(heap) - index + 1;
    count = count > rest ? rest : count;

    while (count--) {
        if (HEAP_NICE(heap, index) < HEAP_NICE(heap, small_index)) {
            small_index = index;
        }
        index++;
    }

    return small_index;
}

static inline uint32
binary_heap_serial_big_nice_index(struct binary_heap *heap,
    uint32 index, uint32 count)
{
    uint32 big_index;
    uint32 rest;

    assert_exit(!complain_zero_size_p(count));
    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    big_index = index;
    rest = INDEX_LAST(heap) - index + 1;
    count = count > rest ? rest : count;

    while (count--) {
        if (HEAP_NICE(heap, index) > HEAP_NICE(heap, big_index)) {
            big_index = index;
        }
        index++;
    }

    return big_index;
}

static inline void
binary_heap_insert(struct binary_heap *heap, void *val, sint64 nice,
    void *ordering)
{
    uint32 index;

    assert_exit(binary_heap_nice_legal_p(nice));
    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_valid_ordered_func_ptr_p(ordering));

    if (binary_heap_full_p(heap)) {
        pr_log_warn("Binary heap is full, will rebuild for percolate up.\n");
        binary_heap_capacity_extend(heap);
    }

    heap->size++;
    HEAP_DATA(heap, heap->size) = NULL;

    index = binary_heap_reorder(heap, heap->size, nice, ordering);
    binary_heap_node_create_by_index(heap, index, nice, val);

    assert_exit(binary_heap_ordered_p(heap, ordering));
}

static inline void *
binary_heap_data_destroy(struct heap_data *data)
{
    void * retval;

    assert_exit(!NULL_PTR_P(data));

    retval = data->val;
    memory_cache_free(data);

    return retval;
}

static inline void *
binary_heap_remove_root(struct binary_heap *heap, void *order)
{
    sint64 nice;
    uint32 index;
    uint32 index_last;
    struct heap_data *last;
    void *retval;

    assert_exit(!binary_heap_empty_p(heap));
    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_valid_ordered_func_ptr_p(order));

    index_last = INDEX_LAST(heap);
    retval = binary_heap_data_destroy(HEAP_DATA(heap, INDEX_ROOT));
    heap->size--;

    if (INDEX_ROOT != index_last) {
        nice = HEAP_NICE(heap, index_last);
        last = HEAP_DATA(heap, index_last);

        HEAP_DATA(heap, index_last) = NULL;

        index = binary_heap_reorder(heap, INDEX_ROOT, nice, order);
        assert_exit(NULL == HEAP_DATA(heap, index));

        HEAP_DATA(heap, index) = last;
    }

    assert_exit(binary_heap_ordered_p(heap, order));
    return retval;
}

static inline uint32
binary_heap_depth(uint32 index)
{
    uint32 depth;

    assert_exit(INDEX_INVALID != index);

    depth = 0;

    while (index >= (1u << (depth + 1))) {
        depth++;
    }

    return depth;
}

static inline bool
binary_heap_nice_legal_p(sint64 nice)
{
    if (nice > HEAP_NICE_LOWER_LMT && nice < HEAP_NICE_UPPER_LMT) {
        return true;
    } else {
        pr_log_warn("Nice specificed reach the limit.\n");
        return false;
    }
}

static inline bool
binary_heap_index_legal_p(struct binary_heap *heap, uint32 index)
{
    assert_exit(binary_heap_structure_legal_p(heap));

    if (INDEX_INVALID == index || index > INDEX_LAST(heap)) {
        pr_log_warn("Illegal index value of heap.\n");
        return false;
    } else {
        return true;
    }
}

static inline bool
binary_heap_structure_legal_p(struct binary_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return false;
    } else if (NULL_PTR_P(heap->base)) {
        return false;
    } else if (complain_zero_size_p(heap->capacity)) {
        return false;
    } else {
        return true;
    }
}

/*
 * Heap-order, minimal heap
 * If nice put into index position ordered
 *     Return true, or false.
 */
static inline bool
binary_heap_minimal_ordered_p(struct binary_heap *heap,
    uint32 index, sint64 nice, uint32 *tgt_index)
{
    uint32 small_child;
    uint32 parent;
    uint32 next;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    parent = INDEX_PARENT(index);
    small_child = binary_heap_child_min_nice_index(heap, index);

    if (INDEX_INVALID != parent && HEAP_NICE(heap, parent) > nice) {
        next = parent;
        goto HEAP_UNORDERED;
    } else if (INDEX_INVALID != small_child
        && HEAP_NICE(heap, small_child) < nice) {
        next = small_child;
        goto HEAP_UNORDERED;
    } else {
        return true;
    }

HEAP_UNORDERED:
    if (tgt_index) {
        *tgt_index = next;
    }

    return false;
}

/*
 * Only for Build heap, as it need percolate down
 */
static inline bool
binary_heap_minimal_percolate_down(struct binary_heap *heap,
    uint32 index, sint64 nice, uint32 *tgt_index)
{
    uint32 next;
    uint32 small_child;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    small_child = binary_heap_child_min_nice_index(heap, index);

    if (INDEX_INVALID != small_child
        && HEAP_NICE(heap, small_child) < nice) {
        next = small_child;

        if (tgt_index) {
            *tgt_index = next;
        }

        return false;
    } else {
        return true;
    }
}

/*
 * Heap-order, maximal heap
 * If nice put into index position ordered
 *     Return true, or false.
 */
static inline bool
binary_heap_maximal_ordered_p(struct binary_heap *heap,
    uint32 index, sint64 nice, uint32 *tgt_index)
{
    uint32 big_child;
    uint32 parent;
    uint32 next;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    parent = INDEX_PARENT(index);
    big_child = binary_heap_child_max_nice_index(heap, index);

    if (INDEX_INVALID != parent && HEAP_NICE(heap, parent) < nice) {
        next = parent;
        goto HEAP_UNORDERED;
    } else if (INDEX_INVALID != big_child
        && HEAP_NICE(heap, big_child) > nice) {
        next = big_child;
        goto HEAP_UNORDERED;
    } else {
        return true;
    }

HEAP_UNORDERED:
    if (tgt_index) {
        *tgt_index = next;
    }

    return false;
}

/*
 * Only for Build heap, as it need percolate down
 */
static inline bool
binary_heap_maximal_percolate_down(struct binary_heap *heap,
    uint32 index, sint64 nice, uint32 *tgt_index)
{
    uint32 next;
    uint32 big_child;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    big_child = binary_heap_child_max_nice_index(heap, index);

    if (INDEX_INVALID != big_child
        && HEAP_NICE(heap, big_child) > nice) {
        next = big_child;

        if (tgt_index) {
            *tgt_index = next;
        }

        return false;
    } else {
        return true;
    }
}

static inline bool
binary_heap_min_max_up_ordered_p(struct binary_heap *heap,
    uint32 index, sint64 nice, uint32 *tgt_index)
{
    uint32 tgt;
    uint32 depth;
    uint32 parent;
    uint32 gdp_randparent;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    parent = INDEX_PARENT(index);
    gdp_randparent = INDEX_GD_PARENT(index);

    if (INDEX_INVALID == parent) {
        return true;
    } else if (INDEX_INVALID == gdp_randparent
        && HEAP_NICE(heap, INDEX_ROOT) <= nice) {
        return true;
    } else if (INDEX_INVALID == gdp_randparent
        && HEAP_NICE(heap, INDEX_ROOT) > nice) {
        tgt = INDEX_ROOT;
    } else {
        depth = binary_heap_depth(index);
        if (depth % 2) {
            // max level;
            if (nice > HEAP_NICE(heap, gdp_randparent)) {
                tgt = gdp_randparent;
            } else if (nice < HEAP_NICE(heap, parent)) {
                tgt = parent;
            } else {
                return true;
            }
        } else {
            // min level;
            if (nice < HEAP_NICE(heap, gdp_randparent)) {
                tgt = gdp_randparent;
            } else if (nice > HEAP_NICE(heap, parent)) {
                tgt = parent;
            } else {
                return true;
            }
        }
    }

    if (tgt_index) {
        *tgt_index = tgt;
    }

    return false;
}

static inline bool
binary_heap_min_max_down_ordered_p(struct binary_heap *heap,
    uint32 index, sint64 nice, uint32 *tgt_index)
{
    uint32 tgt;
    uint32 depth;
    uint32 child_index;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    depth = binary_heap_depth(index);

    if (INDEX_L_CHILD(index) > INDEX_LAST(heap)) {
        return true;
    } else {
        if (depth % 2) {
            // max level
            child_index = binary_heap_gdp_randchild_max_nice_index(heap, index);
            if (nice < HEAP_NICE(heap, child_index)) {
                tgt = child_index;
            } else {
                return true;
            }
        } else {
            // min level
            child_index = binary_heap_gdp_randchild_min_nice_index(heap, index);
            if (nice > HEAP_NICE(heap, child_index)) {
                tgt = child_index;
            } else {
                return true;
            }
        }
    }

    if (tgt_index) {
        *tgt_index = tgt;
    }

    return false;
}

static inline struct leftist_heap *
leftist_heap_create_internal(void *val, sint32 npl, sint64 nice)
{
    struct leftist_heap *heap;

    assert_exit(NPL_NULL != npl);

    heap = memory_cache_allocate(sizeof(*heap));

    if (!complain_no_memory_p(heap)) {
        heap->npl = npl;
        heap->data.nice = nice;
        heap->data.val = val;

        heap->left = NULL;
        heap->right = NULL;
    }

    return heap;
}

struct leftist_heap *
leftist_heap_left(struct leftist_heap *heap)
{
    return heap->left;
}

struct leftist_heap *
leftist_heap_right(struct leftist_heap *heap)
{
    return heap->right;
}

void
leftist_heap_nice_set(struct leftist_heap *heap, sint64 nice)
{
    heap->data.nice = nice;
}

void
leftist_heap_npl_set(struct leftist_heap *heap, sint32 npl)
{
    heap->npl = npl;
}

sint64
leftist_heap_nice(struct leftist_heap *heap)
{
    return heap->data.nice;
}

sint32
leftist_heap_npl(struct leftist_heap *heap)
{
    return heap->npl;
}

void *
leftist_heap_val(struct leftist_heap *heap)
{
    return heap->data.val;
}

void
leftist_heap_val_set(struct leftist_heap *heap, void *val)
{
    heap->data.val = val;
}

struct leftist_heap *
leftist_heap_create(void)
{
    return leftist_heap_create_internal(NULL, 0, 0);
}

struct leftist_heap *
leftist_heap_node_create(void *val, sint32 nlp, sint64 nice)
{
    return leftist_heap_create_internal(val, nlp, nice);
}

static inline void
leftist_heap_destroy_internal(struct leftist_heap *heap)
{
    if (heap) {
        leftist_heap_destroy_internal(heap->left);
        leftist_heap_destroy_internal(heap->right);

        memory_cache_free(heap);
    }
}

void
leftist_heap_destroy(struct leftist_heap **heap)
{
    if (!NULL_PTR_P(heap) && !NULL_PTR_P(*heap)) {
        leftist_heap_destroy_internal(*heap);
        *heap = NULL;
    }
}

static inline void *
leftist_heap_get_min_internal(struct leftist_heap *heap)
{
    assert_exit(leftist_heap_structure_legal_p(heap));

    return heap->data.val;
}

void *
leftist_heap_get_min(struct leftist_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return NULL;
    } else {
        return leftist_heap_get_min_internal(heap);
    }
}

/*
 * _RETURN_ the new leftist heap.
 */
struct leftist_heap *
leftist_heap_insert(struct leftist_heap *heap, void *val, sint64 nice)
{
    struct leftist_heap *node;

    if (NULL_PTR_P(heap)) {
        return NULL;
    } else {
        node = leftist_heap_create_internal(val, 0, nice);
        return leftist_heap_merge_internal(heap, node);
    }
}

static inline void
leftist_heap_node_npl_update(struct leftist_heap *node)
{
    sint32 npl_l;
    sint32 npl_r;

    assert_exit(leftist_heap_structure_legal_p(node));

    npl_l = leftist_heap_npl_internal(node->left);
    npl_r = leftist_heap_npl_internal(node->right);

    node->npl = MIN_S32(npl_l, npl_r) + 1;
}

static inline bool
leftist_heap_node_npl_ordered_p(struct leftist_heap *node)
{
    assert_exit(NULL != node);

    if (leftist_heap_npl_internal(node->left)
        >= leftist_heap_npl_internal(node->right)) {
        return true;
    } else {
        return false;
    }
}

static inline void
leftist_heap_node_child_swap(struct leftist_heap *node)
{
    struct leftist_heap *tmp;

    assert_exit(NULL != node);

    tmp = node->left;
    node->left = node->right;
    node->right = tmp;
}

static inline struct leftist_heap *
leftist_heap_merge_from_right(struct leftist_heap *heap,
    struct leftist_heap *merge)
{
    struct leftist_heap *retval;
    struct leftist_heap *tmp;
    struct leftist_heap **major;
    struct leftist_heap *minor;

    assert_exit(leftist_heap_structure_legal_p(heap));
    assert_exit(leftist_heap_structure_legal_p(merge));

    if (heap->data.nice <= merge->data.nice) {
        retval = heap;
        major = &heap;
        minor = merge;
    } else {
        retval = merge;
        major = &merge;
        minor = heap;
    }

    while (minor) {
        if (!*major) {
            *major = minor;
            break;
        } else if ((*major)->data.nice <= minor->data.nice) {
            major = &(*major)->right;
        } else {
            tmp = *major;
            *major = minor;
            major = &minor->right;
            minor = tmp;
        }
    }

    return retval;
}

static inline void
leftist_heap_reorder_from_right(struct leftist_heap *heap)
{
    if (heap) {
        leftist_heap_reorder_from_right(heap->right);
        leftist_heap_node_npl_update(heap);
        if (!leftist_heap_node_npl_ordered_p(heap)) {
            leftist_heap_node_child_swap(heap);
        }
    }
}

static inline struct leftist_heap *
leftist_heap_merge_internal(struct leftist_heap *heap,
    struct leftist_heap *merge)
{
    struct leftist_heap *retval;

    assert_exit(NULL != heap);
    assert_exit(NULL != merge);
    assert_exit(leftist_heap_validity_p(heap));
    assert_exit(leftist_heap_validity_p(merge));

    retval = leftist_heap_merge_from_right(heap, merge);
    leftist_heap_reorder_from_right(retval);

    assert_exit(leftist_heap_validity_p(retval));

    return retval;
}

/*
 * _RETURN_ the new leftist heap.
 */
struct leftist_heap *
leftist_heap_merge(struct leftist_heap *heap, struct leftist_heap *merge)
{
    if (NULL_PTR_P(heap) && NULL_PTR_P(merge)) {
        return NULL;
    } else if (NULL == heap) {
        return merge;
    } else if (NULL == merge) {
        return heap;
    } else {
        return leftist_heap_merge_internal(heap, merge);
    }
}

static inline struct leftist_heap *
leftist_heap_remove_min_internal(struct leftist_heap **heap)
{
    struct leftist_heap *removed;
    struct leftist_heap *left;
    struct leftist_heap *right;

    assert_exit(NULL != heap);
    assert_exit(leftist_heap_structure_legal_p(*heap));

    removed = *heap;
    left = removed->left;
    right = removed->right;

    if (NULL == left) {
        *heap = right;
    } else if (NULL == right) {
        *heap = left;
    } else {
        *heap = leftist_heap_merge_internal(left, right);
    }

    removed->left = NULL;
    removed->right = NULL;
    return removed;
}

/*
 * _RETURN_ the removed leftist heap node.
 */
struct leftist_heap *
leftist_heap_remove_min(struct leftist_heap **heap)
{
    if (NULL_PTR_P(heap) || NULL_PTR_P(*heap)) {
        return NULL;
    } else {
        return leftist_heap_remove_min_internal(heap);
    }
}

void
leftist_heap_remove_min_and_destroy(struct leftist_heap **heap)
{
    if (!NULL_PTR_P(heap) && !NULL_PTR_P(*heap)) {
        memory_cache_free(leftist_heap_remove_min_internal(heap));
    }
}

static inline sint32
leftist_heap_npl_internal(struct leftist_heap *node)
{
    sint32 npl;

    leftist_heap_npl_optimize(node, npl);
    assert_exit(leftist_heap_npl_optimize_validity_p(node, npl));

    return npl;
}

uint32
maximal_heap_index_last(struct maximal_heap *heap)
{
    return heap->alias->size;
}

uint32
maximal_heap_index_limit(struct maximal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return HEAP_CPCT_INVALID;
    } else {
        return heap->alias->capacity + 1;
    }
}

uint32
maximal_heap_size(struct maximal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return HEAP_SIZE_INVALID;
    } else {
        return heap->alias->size;
    }
}

sint64
maximal_heap_nice(struct maximal_heap *heap, uint32 index)
{
    if (NULL_PTR_P(heap)) {
        return HEAP_NICE_INVALID;
    } else if (index == HEAP_IDX_INVALID || index >= maximal_heap_index_limit(heap)) {
        return HEAP_NICE_INVALID;
    } else {
        return heap->alias->base[index]->nice;
    }
}

void *
maximal_heap_val(struct maximal_heap *heap, uint32 index)
{
    if (NULL_PTR_P(heap)) {
        return PTR_INVALID;
    } else if (index == HEAP_IDX_INVALID || index >= maximal_heap_index_limit(heap)) {
        return PTR_INVALID;
    } else {
        return heap->alias->base[index]->val;
    }
}

struct maximal_heap *
maximal_heap_create(uint32 capacity)
{
    struct maximal_heap *heap;

    heap = memory_cache_allocate(sizeof(*heap));
    heap->alias = binary_heap_create(capacity);

    return heap;
}

void
maximal_heap_destroy(struct maximal_heap **heap)
{
    if (!NULL_PTR_P(heap) && !NULL_PTR_P(*heap)) {
        binary_heap_destroy(&(*heap)->alias);
        memory_cache_free(*heap);
        *heap = NULL;
    }
}

bool
maximal_heap_empty_p(struct maximal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return false;
    } else {
        return binary_heap_empty_p(heap->alias);
    }
}

bool
maximal_heap_full_p(struct maximal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return true;
    } else {
        return binary_heap_full_p(heap->alias);
    }
}

void
maximal_heap_cleanup(struct maximal_heap *heap)
{
    if (!NULL_PTR_P(heap)) {
        binary_heap_cleanup(heap->alias);
    }
}

void *
maximal_heap_get_max(struct maximal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return NULL;
    } else {
        return binary_heap_root(heap->alias);
    }
}

void
maximal_heap_insert(struct maximal_heap *heap, void *val, sint64 nice)
{
    if (NULL_PTR_P(heap)) {
        return;
    } else if (binary_heap_nice_legal_p(nice)) {
        binary_heap_insert(heap->alias, val, nice,
            &binary_heap_maximal_ordered_p);
    }
}

static inline void *
maximal_heap_remove_internal(struct maximal_heap *heap, uint32 index)
{
    sint64 nice;
    struct heap_data *tmp;
    struct binary_heap *alias;

    assert_exit(!NULL_PTR_P(heap));
    assert_exit(!binary_heap_empty_p(heap->alias));
    assert_exit(binary_heap_structure_legal_p(heap->alias));
    assert_exit(binary_heap_index_legal_p(heap->alias, index));

    alias = heap->alias;

    tmp = HEAP_DATA(alias, index);
    HEAP_DATA(alias, index) = NULL;
    nice = HEAP_NICE(alias, INDEX_ROOT) + 1;

    /*
     * percolate current index node to root, then remove the root.
     */
    index = binary_heap_reorder(alias, index, nice, &binary_heap_maximal_ordered_p);

    assert_exit(INDEX_ROOT == index);
    assert_exit(NULL_PTR_P(HEAP_DATA(alias, INDEX_ROOT)));

    HEAP_DATA(alias, INDEX_ROOT) = tmp;
    return binary_heap_remove_root(alias, &binary_heap_maximal_ordered_p);
}

void *
maximal_heap_remove(struct maximal_heap *heap, uint32 index)
{
    if (NULL_PTR_P(heap)) {
        return NULL;
    } else if (!binary_heap_index_legal_p(heap->alias, index)) {
        return NULL;
    } else if (INDEX_ROOT == index) {
        return binary_heap_remove_root(heap->alias,
            &binary_heap_maximal_ordered_p);
    } else {
        return maximal_heap_remove_internal(heap, index);
    }
}

void *
maximal_heap_remove_max(struct maximal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return NULL;
    } else if (binary_heap_empty_p(heap->alias)) {
        pr_log_warn("Attempt to remove node in empty heap.\n");
        return NULL;
    } else {
        return binary_heap_remove_root(heap->alias,
            &binary_heap_maximal_ordered_p);
    }
}

static inline void
maximal_heap_nice_alter(struct maximal_heap *heap, uint32 index,
    sint64 new_nice)
{
    struct heap_data *tmp;
    struct binary_heap *alias;

    assert_exit(!NULL_PTR_P(heap));
    assert_exit(binary_heap_structure_legal_p(heap->alias));
    assert_exit(binary_heap_index_legal_p(heap->alias, index));

    alias = heap->alias;
    tmp = HEAP_DATA(alias, index);
    HEAP_DATA(alias, index) = NULL;

    index = binary_heap_reorder(alias, index, new_nice,
        &binary_heap_maximal_ordered_p);
    assert_exit(NULL == HEAP_DATA(alias, index));

    tmp->nice = new_nice;
    HEAP_DATA(alias, index) = tmp;
}

void
maximal_heap_decrease_nice(struct maximal_heap *heap, uint32 index,
    uint32 offset)
{
    sint64 nice;

    if (NULL_PTR_P(heap)) {
        return;
    } else if (complain_zero_size_p(offset)) {
        return;
    } else if (!binary_heap_index_legal_p(heap->alias, index)) {
        return;
    } else {
        nice = HEAP_NICE(heap->alias, index) - offset;
        maximal_heap_nice_alter(heap, index, nice);
    }
}

void
maximal_heap_increase_nice(struct maximal_heap *heap, uint32 index,
    uint32 offset)
{
    sint64 nice;

    if (NULL_PTR_P(heap)) {
        return;
    } else if (complain_zero_size_p(offset)) {
        return;
    } else if (!binary_heap_index_legal_p(heap->alias, index)) {
        return;
    } else {
        nice = HEAP_NICE(heap->alias, index) + offset;
        maximal_heap_nice_alter(heap, index, nice);
    }
}

static inline void
maximal_heap_build_internal(struct maximal_heap *heap)
{
    uint32 iter;
    uint32 index;
    sint64 nice;
    struct heap_data *tmp;
    struct binary_heap *alias;

    assert_exit(!NULL_PTR_P(heap));
    assert_exit(binary_heap_full_p(heap->alias));
    assert_exit(binary_heap_structure_legal_p(heap->alias));

    alias = heap->alias;
    iter = HEAP_SIZE(alias) / 2;

    while (iter != INDEX_INVALID) {
        index = iter;
        tmp = HEAP_DATA(alias, index);
        nice = HEAP_NICE(alias, index);
        HEAP_DATA(alias, index) = NULL;

        index = binary_heap_reorder(alias, index, nice,
            &binary_heap_maximal_percolate_down);

        assert_exit(NULL == HEAP_DATA(alias, index));
        HEAP_DATA(alias, index) = tmp;

        iter--;
    }
}

struct maximal_heap *
maximal_heap_build(struct heap_data **hd_array, uint32 size)
{
    struct maximal_heap *heap;

    if (NULL_PTR_P(hd_array)) {
        return NULL;
    } else if (complain_zero_size_p(size)) {
        return NULL;
    } else {
        heap = memory_cache_allocate(sizeof(*heap));
        heap->alias = memory_cache_allocate(sizeof(*heap->alias));

        heap->alias->base = hd_array;
        heap->alias->capacity = size - 1;
        heap->alias->size = size - 1;

        hd_array[0] = NULL;
        maximal_heap_build_internal(heap);

        return heap;
    }
}

uint32
min_max_heap_index_limit(struct min_max_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return HEAP_CPCT_INVALID;
    } else {
        return heap->alias->capacity + 1;
    }
}

uint32
min_max_heap_index_last(struct min_max_heap *heap)
{
    return heap->alias->size;
}

uint32
min_max_heap_size(struct min_max_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return HEAP_SIZE_INVALID;
    } else {
        return heap->alias->size;
    }
}

sint64
min_max_heap_nice(struct min_max_heap *heap, uint32 index)
{
    if (NULL_PTR_P(heap)) {
        return HEAP_NICE_INVALID;
    } else if (index == HEAP_IDX_INVALID || index >= min_max_heap_index_limit(heap)) {
        return HEAP_NICE_INVALID;
    } else {
        return heap->alias->base[index]->nice;
    }
}

void *
min_max_heap_val(struct min_max_heap *heap, uint32 index)
{
    if (NULL_PTR_P(heap)) {
        return PTR_INVALID;
    } else if (index == HEAP_IDX_INVALID || index >= min_max_heap_index_limit(heap)) {
        return PTR_INVALID;
    } else {
        return heap->alias->base[index]->val;
    }
}

struct min_max_heap *
min_max_heap_create(uint32 capacity)
{
    struct min_max_heap *heap;

    heap = memory_cache_allocate(sizeof(*heap));
    heap->alias = binary_heap_create(capacity);

    return heap;
}

void
min_max_heap_destroy(struct min_max_heap **heap)
{
    if (!NULL_PTR_P(heap) && min_max_heap_structure_legal_p(*heap)) {
        binary_heap_destroy(&(*heap)->alias);
        memory_cache_free(*heap);
        *heap = NULL;
    }
}

bool
min_max_heap_empty_p_internal(struct min_max_heap *heap)
{
    assert_exit(min_max_heap_structure_legal_p(heap));

    return binary_heap_empty_p(heap->alias);
}

bool
min_max_heap_empty_p(struct min_max_heap *heap)
{
    if (!min_max_heap_structure_legal_p(heap)) {
        return false;
    } else {
        return min_max_heap_empty_p_internal(heap);
    }
}

bool
min_max_heap_full_p(struct min_max_heap *heap)
{
    if (!min_max_heap_structure_legal_p(heap)) {
        return true;
    } else {
        return binary_heap_full_p(heap->alias);
    }
}

void
min_max_heap_cleanup(struct min_max_heap *heap)
{
    if (min_max_heap_structure_legal_p(heap)) {
        binary_heap_cleanup(heap->alias);
    }
}

/*
 * minimal node is root node
 * foreach node in heap, if
 *     even, > gdp_randfater > ... > depth 2 node > root
 *     odd,  > father > gdp_randfater > ... > depth 2 node > root
 */
void *
min_max_heap_get_min(struct min_max_heap *heap)
{
    if (!min_max_heap_structure_legal_p(heap)) {
        return NULL;
    } else {
        return binary_heap_root(heap->alias);
    }
}

/*
 * minimal node is either left or right child of root
 * foreach node in heap, if
 *     even, < fater < gdp_randfater < ... < depth 1 node > root
 *     odd,  < gdp_randfather < ... < depth 1 node > root
 */
void *
min_max_heap_get_max(struct min_max_heap *heap)
{
    uint32 index;

    if (!min_max_heap_structure_legal_p(heap)) {
        return NULL;
    } else if (INDEX_ROOT == INDEX_LAST(heap->alias)) {
        return HEAP_VAL(heap->alias, INDEX_ROOT);
    } else {
        index = binary_heap_child_max_nice_index(heap->alias, INDEX_ROOT);

        assert_exit(INDEX_INVALID != index);
        return HEAP_VAL(heap->alias, index);
    }
}

static inline bool
min_max_heap_structure_legal_p(struct min_max_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return false;
    } else {
        return binary_heap_structure_legal_p(heap->alias);
    }
}

static inline bool
min_max_heap_index_legal_p(struct min_max_heap *heap, uint32 index)
{
    assert_exit(min_max_heap_structure_legal_p(heap));

    return binary_heap_index_legal_p(heap->alias, index);
}

void
min_max_heap_insert(struct min_max_heap *heap, void *val, sint64 nice)
{
    struct binary_heap *alias;
    bool (*order)(struct binary_heap *, uint32, sint64, uint32 *);

    if (!min_max_heap_structure_legal_p(heap)) {
        return;
    } else {
        alias = heap->alias;
        order = &binary_heap_min_max_up_ordered_p;

        binary_heap_insert(alias, val, nice, order);
        assert_exit(min_max_heap_ordered_p(heap));
    }
}

uint32
min_max_heap_depth(struct min_max_heap *heap, uint32 index)
{
    if (!min_max_heap_structure_legal_p(heap)) {
        return DEPTH_INVALID;
    } else if (!min_max_heap_index_legal_p(heap, index)) {
        return DEPTH_INVALID;
    } else {
        return binary_heap_depth(index);
    }
}

void *
min_max_heap_remove_min(struct min_max_heap *heap)
{
    void *retval;
    bool (*order)(struct binary_heap *, uint32, sint64, uint32 *);

    if (!min_max_heap_structure_legal_p(heap)) {
        return NULL;
    } else if (min_max_heap_empty_p_internal(heap)) {
        pr_log_warn("Attempt to remove node in empty heap.\n");
        return NULL;
    } else {
        order = &binary_heap_min_max_down_ordered_p;
        retval = binary_heap_remove_root(heap->alias, order);

        assert_exit(min_max_heap_ordered_p(heap));
        return retval;
    }
}

static inline struct heap_data *
min_max_heap_remove_isolate(struct min_max_heap *heap, uint32 index)
{
    sint64 nice;
    uint32 index_aim;
    struct heap_data *tmp;
    struct heap_data *retval;
    struct binary_heap *alias;
    bool (*order)(struct binary_heap *, uint32, sint64, uint32 *);

    assert_exit(min_max_heap_structure_legal_p(heap));
    assert_exit(!min_max_heap_empty_p_internal(heap));
    assert_exit(min_max_heap_index_legal_p(heap, index));

    alias = heap->alias;
    index_aim = INDEX_LAST(alias);
    retval = HEAP_DATA(alias, index);
    alias->size--;

    /*
     * percolate down at index with last node.
     */
    tmp = HEAP_DATA(alias, index_aim);
    nice = HEAP_NICE(alias, index_aim);
    order = &binary_heap_min_max_down_ordered_p;

    index_aim = binary_heap_reorder(alias, index, nice, order);
    HEAP_DATA(alias, index_aim) = tmp;

    /*
     * percolate up at index_aim with aim node.
     */
    tmp = HEAP_DATA(alias, index_aim);
    nice = HEAP_NICE(alias, index_aim);
    order = &binary_heap_min_max_up_ordered_p;

    index = binary_heap_reorder(alias, index_aim, nice, order);
    HEAP_DATA(alias, index) = tmp;

    assert_exit(min_max_heap_ordered_p(heap));
    return retval;
}

static inline void *
min_max_heap_remove_internal(struct min_max_heap *heap, uint32 index)
{
    void *retval;
    struct heap_data *tmp;

    assert_exit(min_max_heap_structure_legal_p(heap));
    assert_exit(!min_max_heap_empty_p_internal(heap));
    assert_exit(min_max_heap_index_legal_p(heap, index));

    tmp = min_max_heap_remove_isolate(heap, index);
    retval = binary_heap_data_destroy(tmp);

    return retval;
}

struct doubly_linked_list *
min_max_heap_remove(struct min_max_heap *heap, uint32 index)
{
    if (!min_max_heap_structure_legal_p(heap)) {
        return NULL;
    } else if (!min_max_heap_index_legal_p(heap, index)) {
        return NULL;
    } else {
        return min_max_heap_remove_internal(heap, index);
    }
}

static inline void *
min_max_heap_remove_max_internal(struct min_max_heap *heap)
{
    void *retval;
    uint32 max_index;
    struct binary_heap *alias;
    bool (*order)(struct binary_heap *, uint32, sint64, uint32 *);

    assert_exit(min_max_heap_structure_legal_p(heap));

    alias = heap->alias;
    max_index = binary_heap_child_max_nice_index(alias, INDEX_ROOT);

    if (INDEX_INVALID == max_index) {
        order = &binary_heap_min_max_down_ordered_p;
        retval = binary_heap_remove_root(alias, order);
    } else {
        retval = min_max_heap_remove_internal(heap, max_index);
    }

    assert_exit(min_max_heap_ordered_p(heap));
    return retval;
}

void *
min_max_heap_remove_max(struct min_max_heap *heap)
{
    if (!min_max_heap_structure_legal_p(heap)) {
        return NULL;
    } else if (min_max_heap_empty_p_internal(heap)) {
        pr_log_warn("Attempt to remove node in empty heap.\n");
        return NULL;
    } else {
        return min_max_heap_remove_max_internal(heap);
    }
}

static inline void
min_max_heap_nice_alter(struct min_max_heap *heap, uint32 index,
    sint64 new_nice)
{
    uint32 index_next;
    struct heap_data *tmp;
    struct binary_heap *alias;
    bool (*order)(struct binary_heap *, uint32, sint64, uint32 *);

    assert_exit(min_max_heap_structure_legal_p(heap));
    assert_exit(min_max_heap_index_legal_p(heap, index));

    alias = heap->alias;
    tmp = min_max_heap_remove_isolate(heap, index);

    alias->size++;
    tmp->nice = new_nice;

    order = &binary_heap_min_max_up_ordered_p;
    index_next = binary_heap_reorder(alias, alias->size, new_nice, order);

    HEAP_DATA(alias, index_next) = tmp;
    assert_exit(min_max_heap_ordered_p(heap));
}

void
min_max_heap_decrease_nice(struct min_max_heap *heap, uint32 index,
    uint32 offset)
{
    sint64 nice;

    if (!min_max_heap_structure_legal_p(heap)) {
        return;
    } else if (complain_zero_size_p(offset)) {
        return;
    } else if (!min_max_heap_index_legal_p(heap, index)) {
        return;
    } else {
        nice = HEAP_NICE(heap->alias, index) - offset;
        min_max_heap_nice_alter(heap, index, nice);
    }
}

void
min_max_heap_increase_nice(struct min_max_heap *heap, uint32 index,
    uint32 offset)
{
    sint64 nice;

    if (!min_max_heap_structure_legal_p(heap)) {
        return;
    } else if (complain_zero_size_p(offset)) {
        return;
    } else if (!min_max_heap_index_legal_p(heap, index)) {
        return;
    } else {
        nice = HEAP_NICE(heap->alias, index) + offset;
        min_max_heap_nice_alter(heap, index, nice);
    }
}

uint32
minimal_heap_index_limit(struct minimal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return HEAP_CPCT_INVALID;
    } else {
        return heap->alias->capacity + 1;
    }
}

uint32
minimal_heap_size(struct minimal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return HEAP_SIZE_INVALID;
    } else {
        return heap->alias->size;
    }
}

sint64
minimal_heap_nice(struct minimal_heap *heap, uint32 index)
{
    if (NULL_PTR_P(heap)) {
        return HEAP_NICE_INVALID;
    } else if (index == HEAP_IDX_INVALID || index >= minimal_heap_index_limit(heap)) {
        return HEAP_NICE_INVALID;
    } else {
        return heap->alias->base[index]->nice;
    }
}

void *
minimal_heap_val(struct minimal_heap *heap, uint32 index)
{
    if (NULL_PTR_P(heap)) {
        return PTR_INVALID;
    } else if (index == HEAP_IDX_INVALID || index >= minimal_heap_index_limit(heap)) {
        return PTR_INVALID;
    } else {
        return heap->alias->base[index]->val;
    }
}

struct minimal_heap *
minimal_heap_create(uint32 capacity)
{
    struct minimal_heap *heap;

    heap = memory_cache_allocate(sizeof(*heap));
    heap->alias = binary_heap_create(capacity);

    return heap;
}

void
minimal_heap_destroy(struct minimal_heap **heap)
{
    if (!NULL_PTR_P(heap) && !NULL_PTR_P(*heap)) {
        binary_heap_destroy(&(*heap)->alias);
        memory_cache_free(*heap);
        *heap = NULL;
    }
}

bool
minimal_heap_empty_p(struct minimal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return false;
    } else {
        return binary_heap_empty_p(heap->alias);
    }
}

bool
minimal_heap_full_p(struct minimal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return true;
    } else {
        return binary_heap_full_p(heap->alias);
    }
}

uint32
minimal_heap_index_last(struct minimal_heap *heap)
{
    return heap->alias->size;
}

void
minimal_heap_cleanup(struct minimal_heap *heap)
{
    if (!NULL_PTR_P(heap)) {
        binary_heap_cleanup(heap->alias);
    }
}

void *
minimal_heap_get_min(struct minimal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return NULL;
    } else {
        return binary_heap_root(heap->alias);
    }
}

void
minimal_heap_insert(struct minimal_heap *heap, void *val, sint64 nice)
{
    if (NULL_PTR_P(heap)) {
        return;
    } if (binary_heap_nice_legal_p(nice)) {
        binary_heap_insert(heap->alias, val, nice,
            &binary_heap_minimal_ordered_p);
    }
}

static inline void *
minimal_heap_remove_internal(struct minimal_heap *heap, uint32 index)
{
    sint64 nice;
    struct heap_data *tmp;
    struct binary_heap *alias;

    assert_exit(!NULL_PTR_P(heap));
    assert_exit(!binary_heap_empty_p(heap->alias));
    assert_exit(binary_heap_structure_legal_p(heap->alias));
    assert_exit(binary_heap_index_legal_p(heap->alias, index));

    alias = heap->alias;

    tmp = HEAP_DATA(alias, index);
    HEAP_DATA(alias, index) = NULL;
    nice = HEAP_NICE(alias, INDEX_ROOT) - 1;

    /*
     * percolate current index node to root, then remove the root.
     */
    index = binary_heap_reorder(alias, index, nice, &binary_heap_minimal_ordered_p);

    assert_exit(INDEX_ROOT == index);
    assert_exit(NULL_PTR_P(HEAP_DATA(alias, INDEX_ROOT)));

    HEAP_DATA(alias, INDEX_ROOT) = tmp;
    return binary_heap_remove_root(alias, &binary_heap_minimal_ordered_p);
}

void *
minimal_heap_remove(struct minimal_heap *heap, uint32 index)
{
    if (NULL_PTR_P(heap)) {
        return NULL;
    } else if (!binary_heap_index_legal_p(heap->alias, index)) {
        return NULL;
    } else if (INDEX_ROOT == index) {
        return binary_heap_remove_root(heap->alias,
            &binary_heap_minimal_ordered_p);
    } else {
        return minimal_heap_remove_internal(heap, index);
    }
}

void *
minimal_heap_remove_min(struct minimal_heap *heap)
{
    if (NULL_PTR_P(heap)) {
        return NULL;
    } else if (binary_heap_empty_p(heap->alias)) {
        pr_log_warn("Attempt to remove node in empty heap.\n");
        return NULL;
    } else {
        return binary_heap_remove_root(heap->alias,
            &binary_heap_minimal_ordered_p);
    }
}

static inline void
minimal_heap_nice_alter(struct minimal_heap *heap, uint32 index,
    sint64 new_nice)
{
    struct heap_data *tmp;
    struct binary_heap *alias;

    assert_exit(!NULL_PTR_P(heap));
    assert_exit(binary_heap_structure_legal_p(heap->alias));
    assert_exit(binary_heap_index_legal_p(heap->alias, index));

    alias = heap->alias;
    tmp = HEAP_DATA(alias, index);
    HEAP_DATA(alias, index) = NULL;

    index = binary_heap_reorder(alias, index, new_nice,
        &binary_heap_minimal_ordered_p);
    assert_exit(NULL == HEAP_DATA(alias, index));

    tmp->nice = new_nice;
    HEAP_DATA(alias, index) = tmp;
}

void
minimal_heap_decrease_nice(struct minimal_heap *heap, uint32 index,
    uint32 offset)
{
    sint64 nice;

    if (NULL_PTR_P(heap)) {
        return;
    } else if (complain_zero_size_p(offset)) {
        return;
    } else if (!binary_heap_index_legal_p(heap->alias, index)) {
        return;
    } else {
        nice = HEAP_NICE(heap->alias, index) - offset;
        minimal_heap_nice_alter(heap, index, nice);
    }
}

void
minimal_heap_increase_nice(struct minimal_heap *heap, uint32 index,
    uint32 offset)
{
    sint64 nice;

    if (NULL_PTR_P(heap)) {
        return;
    } else if (complain_zero_size_p(offset)) {
        return;
    } else if (!binary_heap_index_legal_p(heap->alias, index)) {
        return;
    } else {
        nice = HEAP_NICE(heap->alias, index) + offset;
        minimal_heap_nice_alter(heap, index, nice);
    }
}

static inline void
minimal_heap_build_internal(struct minimal_heap *heap)
{
    uint32 iter;
    uint32 index;
    sint64 nice;
    struct heap_data *tmp;
    struct binary_heap *alias;

    assert_exit(!NULL_PTR_P(heap));
    assert_exit(binary_heap_full_p(heap->alias));
    assert_exit(binary_heap_structure_legal_p(heap->alias));

    alias = heap->alias;
    iter = HEAP_SIZE(alias) / 2;

    while (iter != INDEX_INVALID) {
        index = iter;
        tmp = HEAP_DATA(alias, index);
        nice = HEAP_NICE(alias, index);
        HEAP_DATA(alias, index) = NULL;

        index = binary_heap_reorder(alias, index, nice,
            &binary_heap_minimal_percolate_down);

        assert_exit(NULL == HEAP_DATA(alias, index));
        HEAP_DATA(alias, index) = tmp;

        iter--;
    }
}

struct minimal_heap *
minimal_heap_build(struct heap_data **hd_array, uint32 size)
{
    struct minimal_heap *heap;

    if (NULL_PTR_P(hd_array)) {
        return NULL;
    } else if (complain_zero_size_p(size)) {
        return NULL;
    } else {
        heap = memory_cache_allocate(sizeof(*heap));
        heap->alias = memory_cache_allocate(sizeof(*heap->alias));

        heap->alias->base = hd_array;
        heap->alias->capacity = size - 1;
        heap->alias->size = size - 1;

        hd_array[0] = NULL;
        minimal_heap_build_internal(heap);

        return heap;
    }
}

static inline bool
binary_heap_valid_ordered_func_ptr_p(void *func_ptr)
{
    void **iter;

    assert_exit(NULL != func_ptr);

    iter = heap_order_func;
    while (iter < heap_order_func + array_sizeof(heap_order_func)) {
        if (*iter++ == func_ptr) {
            return true;
        }
    }

    return false;
}

static inline bool
binary_heap_depth_even_p(struct binary_heap *heap, uint32 index)
{
    uint32 depth;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    depth = binary_heap_depth(index);

    return 0u == (depth & 0x1u) ? true : false;
}

static inline bool
binary_heap_depth_odd_p(struct binary_heap *heap, uint32 index)
{
    uint32 depth;

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_index_legal_p(heap, index));

    depth = binary_heap_depth(index);

    return 1u == (depth & 0x1u) ? true : false;
}

static inline bool
binary_heap_ordered_p(struct binary_heap *heap, void *heap_order)
{
    uint32 index;
    bool (*order)(struct binary_heap *, uint32, sint64, uint32 *);

    assert_exit(binary_heap_structure_legal_p(heap));
    assert_exit(binary_heap_valid_ordered_func_ptr_p(heap_order));

    index = INDEX_ROOT;
    order = heap_order;

    while (index <= INDEX_LAST(heap)) {
        if (!(*order)(heap, index, HEAP_NICE(heap, index), NULL)) {
            return false;
        }
        index++;
    }

    return true;
}

static inline bool
min_max_heap_ordered_p(struct min_max_heap *heap)
{
    sint64 nice;
    uint32 index;
    struct binary_heap *alias;

    assert_exit(!NULL_PTR_P(heap));
    assert_exit(!NULL_PTR_P(heap->alias));

    index = INDEX_ROOT;
    alias = heap->alias;

    while (index <= INDEX_LAST(alias)) {
        nice = HEAP_NICE(alias, index);
        if (!binary_heap_min_max_up_ordered_p(alias, index, nice, NULL)) {
            return false;
        } else if (!binary_heap_min_max_down_ordered_p(alias, index, nice,
            NULL)) {
            return false;
        }
        index++;
    }

    return true;
}

static inline bool
leftist_heap_structure_legal_p(struct leftist_heap *heap)
{
    if (NULL == heap) {
        return false;
    } else if (NULL != heap->left && heap->left == heap->right) {
        return false;
    } else {
        return true;
    }
}

static inline sint32
leftist_heap_npl_internal_default(struct leftist_heap *node)
{
    if (!node) {
        return -1;
    } else {
        return node->npl;
    }
}

static inline bool
leftist_heap_npl_optimize_validity_p(struct leftist_heap *node,
    sint32 expected)
{
    sint32 computed;

    computed = leftist_heap_npl_internal_default(node);

    if (computed == expected) {
        return true;
    } else {
        dp_printf("expected: %d\n", expected);
        dp_printf("computed: %d\n", computed);
        return false;
    }
}

static inline bool
leftist_heap_node_heap_ordered_p(struct leftist_heap *node)
{
    if (!node) {
        return true;
    } else if (NULL == node->left && NULL == node->right) {
        return true;
    } else if (NULL != node->left
        && node->data.nice > node->left->data.nice) {
        return false;
    } else if (NULL != node->right
        && node->data.nice > node->right->data.nice) {
        return false;
    } else {
        return true;
    }
}

static inline bool
leftist_heap_validity_p(struct leftist_heap *heap)
{
    if (!heap) {
        return true;
    } else if (!leftist_heap_node_heap_ordered_p(heap)) {
        return false;
    } else if (!leftist_heap_node_npl_ordered_p(heap)) {
        return false;
    } else if (!leftist_heap_validity_p(heap->left)) {
        return false;
    } else if (!leftist_heap_validity_p(heap->right)) {
        return false;
    } else {
        return true;
    }
}

