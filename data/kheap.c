/*
GNU LESSER GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.


  This version of the GNU Lesser General Public License incorporates
the terms and conditions of version 3 of the GNU General Public
License, supplemented by the additional permissions listed below.

  0. Additional Definitions.

  As used herein, "this License" refers to version 3 of the GNU Lesser
General Public License, and the "GNU GPL" refers to version 3 of the GNU
General Public License.

  "The Library" refers to a covered work governed by this License,
other than an Application or a Combined Work as defined below.

  An "Application" is any work that makes use of an interface provided
by the Library, but which is not otherwise based on the Library.
Defining a subclass of a class defined by the Library is deemed a mode
of using an interface provided by the Library.

  A "Combined Work" is a work produced by combining or linking an
Application with the Library.  The particular version of the Library
with which the Combined Work was made is also called the "Linked
Version".

  The "Minimal Corresponding Source" for a Combined Work means the
Corresponding Source for the Combined Work, excluding any source code
for portions of the Combined Work that, considered in isolation, are
based on the Application, and not on the Linked Version.

  The "Corresponding Application Code" for a Combined Work means the
object code and/or source code for the Application, including any data
and utility programs needed for reproducing the Combined Work from the
Application, but excluding the System Libraries of the Combined Work.

  1. Exception to Section 3 of the GNU GPL.

  You may convey a covered work under sections 3 and 4 of this License
without being bound by section 3 of the GNU GPL.

  2. Conveying Modified Versions.

  If you modify a copy of the Library, and, in your modifications, a
facility refers to a function or data to be supplied by an Application
that uses the facility (other than as an argument passed when the
facility is invoked), then you may convey a copy of the modified
version:

   a) under this License, provided that you make a good faith effort to
   ensure that, in the event an Application does not supply the
   function or data, the facility still operates, and performs
   whatever part of its purpose remains meaningful, or

   b) under the GNU GPL, with none of the additional permissions of
   this License applicable to that copy.

  3. Object Code Incorporating Material from Library Header Files.

  The object code form of an Application may incorporate material from
a header file that is part of the Library.  You may convey such object
code under terms of your choice, provided that, if the incorporated
material is not limited to numerical parameters, data structure
layouts and accessors, or small macros, inline functions and templates
(ten or fewer lines in length), you do both of the following:

   a) Give prominent notice with each copy of the object code that the
   Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the object code with a copy of the GNU GPL and this license
   document.

  4. Combined Works.

  You may convey a Combined Work under terms of your choice that,
taken together, effectively do not restrict modification of the
portions of the Library contained in the Combined Work and reverse
engineering for debugging such modifications, if you also do each of
the following:

   a) Give prominent notice with each copy of the Combined Work that
   the Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the Combined Work with a copy of the GNU GPL and this license
   document.

   c) For a Combined Work that displays copyright notices during
   execution, include the copyright notice for the Library among
   these notices, as well as a reference directing the user to the
   copies of the GNU GPL and this license document.

   d) Do one of the following:

       0) Convey the Minimal Corresponding Source under the terms of this
       License, and the Corresponding Application Code in a form
       suitable for, and under terms that permit, the user to
       recombine or relink the Application with a modified version of
       the Linked Version to produce a modified Combined Work, in the
       manner specified by section 6 of the GNU GPL for conveying
       Corresponding Source.

       1) Use a suitable shared library mechanism for linking with the
       Library.  A suitable mechanism is one that (a) uses at run time
       a copy of the Library already present on the user's computer
       system, and (b) will operate properly with a modified version
       of the Library that is interface-compatible with the Linked
       Version.

   e) Provide Installation Information, but only if you would otherwise
   be required to provide such information under section 6 of the
   GNU GPL, and only to the extent that such information is
   necessary to install and execute a modified version of the
   Combined Work produced by recombining or relinking the
   Application with a modified version of the Linked Version. (If
   you use option 4d0, the Installation Information must accompany
   the Minimal Corresponding Source and Corresponding Application
   Code. If you use option 4d1, you must provide the Installation
   Information in the manner specified by section 6 of the GNU GPL
   for conveying Corresponding Source.)

  5. Combined Libraries.

  You may place library facilities that are a work based on the
Library side by side in a single library together with other library
facilities that are not Applications and are not covered by this
License, and convey such a combined library under terms of your
choice, if you do both of the following:

   a) Accompany the combined library with a copy of the same work based
   on the Library, uncombined with any other library facilities,
   conveyed under the terms of this License.

   b) Give prominent notice with the combined library that part of it
   is a work based on the Library, and explaining where to find the
   accompanying uncombined form of the same work.

  6. Revised Versions of the GNU Lesser General Public License.

  The Free Software Foundation may publish revised and/or new versions
of the GNU Lesser General Public License from time to time. Such new
versions will be similar in spirit to the present version, but may
differ in detail to address new problems or concerns.

  Each version is given a distinguishing version number. If the
Library as you received it specifies that a certain numbered version
of the GNU Lesser General Public License "or any later version"
applies to it, you have the option of following the terms and
conditions either of that published version or of any later version
published by the Free Software Foundation. If the Library as you
received it does not specify a version number of the GNU Lesser
General Public License, you may choose any version of the GNU Lesser
General Public License ever published by the Free Software Foundation.

  If the Library as you received it specifies that a proxy can decide
whether future versions of the GNU Lesser General Public License shall
apply, that proxy's public statement of acceptance of any version is
permanent authorization for you to choose that version for the
Library.
*/

void
kheap_initialize(void)
{


}

// <Heap>
// For performing some kind of dynamic memory allocation, with consistency
// checking and debugging.
// 
// <Data Structure>
// Index table with pointers to holes.
// Heap index
// +--------+---------+---------+---------+
// | size 8 | size 10 | size 16 | size 64 | ......
// |  Ptr   |   Ptr   |   Ptr   |   Ptr   |
// +--------+---------+---------+---------+
//    Ptr pointers to holes
// 
// There are two concepts, block and holes.
// Blocks are contiguous part of memory contains user data (such as malloc but
// not free yet). Holes are blocks but are not in use.
// 
// For each hole there is a corresponding descriptor in a index table. The index
// table is always ordered ascending by the size of hole.
// 
// Blocks and holes each contain decriptive data, a header and a footer. The header
// contains the most information about the block, and the footer merely contains
// a pointer to the header.
// 
// +---------------------+---------------+----------------+
// |       header        |               |     footer     |
// +-------+------+------| useable space |-------+--------+
// | magic | hole | size |               | magic | header |
// +-------+------+------+---------------+-------+--------+
// 
// Magic number is for error checking of serial number.
// 
// <Allocation>
// * Search the index table to find the smallest hole that fit the request size.
//     If fail to find the smallest hole, then
//         * Expand the heap space.
//         * If the index table is empty, then add new entry into it.
//         * else adjust the last header size and rewrite footer
// * Decide if the hole should be split into two parts
// * If the block should be page aligned, it should be create a new hole.
// * Write the new block's header and footer
// * If the hole was to be split into two parts, write a new hole into the index table.
// * Return the address of the block to user
// 
// <Deallocation>
// Freeing may be a little tricky for efficiency.
// When free a block, look at what is immediately to the left (for 0 ~ 4GB, left to right).
// If that is a footer, which can be discovered from the value of magic number, then follow
// the pointer to header and query if is a block or hole.
// 
// If hole, we can rewrite the header size attribute and merge both into one hole, without
// any insertion operation on the index table. This is unifying left, also unifying right.
// After the footer, If we find a header there, checking if hole and identified magic number.
// Then we can use the size attribute to find its footer. And then rewrite footer pointer to
// previous header. And remove its old entry from hole index and update it with new one.
// 
// At last of reclaiming space, if we are freeing the last block in the heap, then we can
// contract the size of the heap with one minimum heap size.

static inline void *
kmalloc_int(uint32 sz, bool align, ptr_t *phys)
{
    void *retval;

    if (align && 0 != (placement_ptr & 0xfff)) {
        placement_ptr &= ((ptr_t)-1 << 12);
        placement_ptr += 0x1000;
    }

    if (placement_ptr + sz > MEMORY_LIMIT) {
        return NULL;
    } else if (phys) {
        *phys = placement_ptr;
    }

    retval = (void *)placement_ptr;
    placement_ptr += sz;

    return retval;
}

static inline bool
kheap_legal_p(s_kheap_t *heap)
{
    if (NULL == heap) {
        return false;
    } else if (!ordered_array_legal_p(&heap->ordered)) {
        return false;
    } else if (heap->addr_start > heap->addr_end) {
        return false;
    } else if (heap->addr_end > heap->addr_max) {
        return false;
    } else {
        return true;
    }
}

static inline ptr_t
kheap_size(s_kheap_t *heap)
{
    kassert(kheap_legal_p(heap));
    return heap->addr_end - heap->addr_start;
}

static inline uint32
kheap_find_hole(s_kheap_t *heap, void *val)
{
    uint32 i;

    kassert(NULL != val);
    kassert(kheap_legal_p(heap));

    i = 0;
    while (i < heap->ordered.size) {
        if (val == ordered_array_lookup(&heap->ordered, i)) {
            return i;
        }
        i++;
    }

    return IDX_INVALID;
}

static inline sint32
kheap_compare(void *a, void *b)
{
    s_kheap_header_t *ka;
    s_kheap_header_t *kb;

    kassert(NULL != a);
    kassert(NULL != b);

    ka = a;
    kb = b;

    if (ka->size < kb->size) {
        return 1;
    } else if (ka->size > kb->size) {
        return -1;
    } else {
        return 0;
    }
}

s_kheap_t *
kheap_create(ptr_t addr_start, ptr_t addr_end, ptr_t addr_max,
    bool supervisor, bool read_only)
{
    s_kheap_t *heap;

    if (addr_start > addr_end || 0 == addr_max) {
        return NULL;
    } else if (0 != (addr_start & 0xfff)) {
        return NULL;
    } else if (0 != (addr_end & 0xfff)) {
        return NULL;
    } else {
        heap = kmalloc(sizeof(*heap));
        heap->ordered = ordered_array_place((void *)addr_start, KHEAP_IDX_SIZE, &kheap_compare);
        addr_start += sizeof(void *) * KHEAP_IDX_SIZE;

        if (0 != (addr_start & 0xfff)) {
            addr_start &= ((ptr_t)-1 << 12);
            addr_start += PAGE_SIZE;
        }

        heap->addr_start = addr_start;
        heap->addr_end = addr_end;
        heap->addr_max = addr_max;
        heap->supervisor = supervisor;
        heap->read_only = read_only;

        kheap_make_hole((void *)addr_start, addr_end - addr_start);
        ordered_array_insert(&heap->ordered, (void *)addr_start);
        // Fix-Me, no footer here ?

        return heap;
    }
}

static inline ptr_t
kheap_obtain_minimal_hole(s_kheap_t *heap, ptr_t *useable_addr,
    uint32 request_size, bool page_align)
{
    uint32 i;
    ptr_t hole_addr;
    ptr_t tmp_addr;
    uint32 hole_size;
    s_kheap_header_t *header;

    kassert(kheap_legal_p(heap));
    kassert(0 != request_size);
    kassert(NULL != useable_addr);

    i = 0;
    while (i < heap->ordered.size) {
        header = (void *)ordered_array_lookup(&heap->ordered, i);
        hole_addr = (ptr_t)header;
        tmp_addr = hole_addr + sizeof(s_kheap_header_t);

        if (page_align) {
            if (!PAGE_ALIGN_P(tmp_addr)) {
                tmp_addr = (tmp_addr & PAGE_ALIGN_MASK) + PAGE_SIZE;
            }

            hole_size = tmp_addr - hole_addr + request_size;
            hole_size += sizeof(s_kheap_footer_t);

            if (header->size >= hole_size) {
                goto FOUND;
            }
        } else if (header->size >= request_size) {
            goto FOUND;
        }
        i++;
    }

    *useable_addr = PTR_INVALID;
    return PTR_INVALID;

FOUND:
    *useable_addr = tmp_addr;
    ordered_array_remove(&heap->ordered, i);
    kassert(kheap_hole_legal_p((void *)hole_addr));

    return hole_addr;
}

static inline bool
kheap_hole_legal_p(void *hole_addr)
{
    s_kheap_header_t *header;
    s_kheap_footer_t *footer;

    kassert(NULL != hole_addr && (void *)PTR_INVALID != hole_addr);

    header = hole_addr;
    footer = kheap_obtain_footer(header);

    if (KHEAP_MAGIC != footer->magic || KHEAP_MAGIC != header->magic) {
        return false;
    } else if (footer->header != header) {
        return false;
    } else {
        return true;
    }
}

static inline s_kheap_footer_t *
kheap_make_hole(void *hole_addr, uint32 size)
{
    s_kheap_header_t *hole_header;
    s_kheap_footer_t *hole_footer;

    kassert(NULL != hole_addr && (void *)PTR_INVALID != hole_addr);
    kassert(size >= KHEAP_HOLE_MIN_SIZE);

    hole_header = (void *)hole_addr;
    hole_footer = (void *)(hole_addr + size - sizeof(s_kheap_footer_t));

    hole_header->magic = KHEAP_MAGIC;
    hole_header->is_hole = true;
    hole_header->size = size;

    hole_footer->magic = KHEAP_MAGIC;
    hole_footer->header = hole_header;

    kassert(kheap_hole_legal_p(hole_addr));
    return hole_footer;
}

static inline void
kheap_make_block(void *hole_addr, uint32 size)
{
    s_kheap_header_t *hole_header;

    kassert(NULL != hole_addr && (void *)PTR_INVALID != hole_addr);
    kassert(size >= KHEAP_HOLE_MIN_SIZE);

    kheap_make_hole(hole_addr, size);

    hole_header = (void *)hole_addr;
    hole_header->is_hole = false;
}

static inline void
kheap_handle_hole_unavailable(s_kheap_t *heap, uint32 hole_size)
{
    uint32 i;
    uint32 idx;
    ptr_t hole_addr;
    ptr_t heap_size;
    ptr_t last_addr;
    ptr_t added_size;
    s_kheap_header_t *header;

    kassert(kheap_legal_p(heap));
    kassert(hole_size >= KHEAP_MIN_SIZE);

    heap_size = heap->addr_end - heap->addr_start;
    kheap_resize(heap, heap_size + hole_size);
    kassert(heap->addr_end - heap->addr_start > heap_size);

    i = 0;
    last_addr = 0;
    idx = IDX_INVALID;
    added_size = heap->addr_end - heap->addr_start - heap_size;

    // optimize here, add one field addr_last
    while (i < heap->ordered.size) {
        hole_addr = (ptr_t)ordered_array_lookup(&heap->ordered, i);
        if (hole_addr > last_addr) {
            last_addr = hole_addr;
            idx = i;
        }
        i++;
    }

    if (IDX_INVALID == idx) { // If no hole in orderred array
        header = (s_kheap_header_t *)heap->addr_end;
        kheap_make_block(header, added_size);
        ordered_array_insert(&heap->ordered, header);
    } else {
        // Bug here ? if memory is block in use, which has bigger address than last_addr.
        header = (void *)last_addr;
        kheap_make_hole(header, header->size + added_size);
    }
}

void *
kheap_allocate(s_kheap_t *heap, uint32 request_size, bool page_align)
{
    ptr_t hole_addr;
    ptr_t split_addr;
    uint32 hole_size;
    uint32 rest_size;
    ptr_t useable_addr;
    uint32 lead_blank_size;

    kassert(0 != request_size);
    kassert(kheap_legal_p(heap));

    hole_addr = kheap_obtain_minimal_hole(heap, &useable_addr, request_size, page_align);

    if (PTR_INVALID == hole_addr) {
        hole_size = request_size + KHEAP_INFO_SIZE;
        if (page_align) {
            hole_size += PAGE_SIZE;
        }

        kheap_handle_hole_unavailable(heap, hole_size);
        return kheap_allocate(heap, request_size, page_align);
    } else {
        hole_size = ((s_kheap_header_t *)hole_addr)->size;
        lead_blank_size = (uint32)(useable_addr - hole_addr);
        lead_blank_size -= sizeof(s_kheap_header_t);

        if (lead_blank_size >= KHEAP_HOLE_MIN_SIZE) {
            split_addr = hole_addr;
            hole_addr += lead_blank_size;
            hole_size -= lead_blank_size;
            kheap_make_hole((void *)split_addr, lead_blank_size);
            ordered_array_insert(&heap->ordered, (void *)split_addr);
        }

        rest_size = (uint32)(useable_addr - hole_addr) + request_size;
        rest_size = hole_size - rest_size - sizeof(s_kheap_footer_t);

        if (rest_size >= KHEAP_HOLE_MIN_SIZE) {
            split_addr = hole_addr + (hole_size - rest_size);
            hole_size -= rest_size;
            kheap_make_hole((void *)split_addr, rest_size);
            ordered_array_insert(&heap->ordered, (void *)split_addr);
        }

        kheap_make_block((void *)hole_addr, hole_size);
        return (void *)useable_addr;
    }
}

static inline void *
kheap_obtain_footer(s_kheap_header_t *header)
{
    kassert(NULL != header);
    kassert(KHEAP_MAGIC == header->magic);
    kassert(header->size >= KHEAP_HOLE_MIN_SIZE);

    return (void *)((ptr_t)header + header->size - sizeof(s_kheap_footer_t));
}

void
kheap_free(s_kheap_t *heap, void *val)
{
    uint32 idx;
    ptr_t heap_size;
    uint32 hole_size;
    bool insert_needed;
    ptr_t deleted_size;
    s_kheap_header_t *header;
    s_kheap_footer_t *footer;
    s_kheap_header_t *next_header;
    s_kheap_footer_t *last_footer;

    kassert(NULL != val);
    kassert(kheap_legal_p(heap));

    insert_needed = true;
    header = (void *)((ptr_t)val - sizeof(s_kheap_header_t));
    header->is_hole = true;
    footer = kheap_obtain_footer(header);

    kassert(KHEAP_MAGIC == header->magic && KHEAP_MAGIC == footer->magic);

    last_footer = (void *)((ptr_t)header - sizeof(s_kheap_footer_t));
    if (KHEAP_MAGIC == last_footer->magic && last_footer->header->is_hole) {
        insert_needed = false;
        hole_size = last_footer->header->size + header->size;
        footer = kheap_make_hole(last_footer->header, hole_size);
        header = footer->header;
    }

    next_header = (void *)((ptr_t)header + header->size);
    if (KHEAP_MAGIC == next_header->magic && next_header->is_hole) {
        idx = kheap_find_hole(heap, next_header);
        ordered_array_remove(&heap->ordered, idx);

        hole_size = header->size + next_header->size;
        footer = kheap_make_hole(header, hole_size);
    }

    // If last hole in kheap
    if ((ptr_t)footer + sizeof(s_kheap_footer_t) == heap->addr_end) {
        heap_size = kheap_size(heap);
        kheap_resize(heap, (ptr_t)header - heap->addr_start);
        deleted_size = heap_size - kheap_size(heap);

        if (header->size >= deleted_size + KHEAP_HOLE_MIN_SIZE) {
            kheap_make_hole(header, header->size - deleted_size);
        } else if (header->size >= deleted_size) {
            idx = kheap_find_hole(heap, header);
            ordered_array_remove(&heap->ordered, idx);
            // May leave at most KHEAP_HOLE_MIN_SIZE
            return;
        } else {
            KERNEL_PANIC(MEM_OVERWRITE);
        }
    }

    if (insert_needed) {
        ordered_array_insert(&heap->ordered, header);
    } else {
        idx = kheap_find_hole(heap, header);
        ordered_array_adjust(&heap->ordered, idx);
    }
}

static inline void
kheap_resize(s_kheap_t *heap, uint32 new_size)
{
    ptr_t i;
    s_page_entry_t *pe;

    kassert(kheap_legal_p(heap));

    if (0 != (new_size & 0xfff)) {
        new_size &= ((ptr_t)-1 << 12);
        new_size += PAGE_SIZE;
    }

    i = heap->addr_end - heap->addr_start;
    kassert(0 == (i & 0xfff));

    if (new_size + heap->addr_start <= heap->addr_end) {
        // contract space
        if (new_size < KHEAP_MIN_SIZE) {
            new_size = KHEAP_MIN_SIZE;
        }

        while (new_size < i) {
            i -= PAGE_SIZE;
            pe = paging_get(heap->addr_start + i, false, kernel_dirt);
            frame_free(pe);
        }
    } else {
        // expand space
        kassert(new_size + heap->addr_start <= heap->addr_max);

        while (i < new_size) {
            pe = paging_get(heap->addr_start + i, true, kernel_dirt);
            frame_allocate(pe, heap->supervisor, !heap->read_only);
            i += PAGE_SIZE;
        }
    }

    kassert(i == new_size);
    heap->addr_end = heap->addr_start + new_size;
}

void *
kmalloc(uint32 sz)
{
    kprintf("We    are in kmalloc\n");
    return kmalloc_int(sz, false, NULL);
}

void *
kmalloc_algn(uint32 sz)
{
    return kmalloc_int(sz, true, NULL);
}

void *
kmalloc_phys(uint32 sz, ptr_t *phys)
{
    return kmalloc_int(sz, false, phys);
}

void *
kmalloc_algn_with_phys(uint32 sz, ptr_t *phys)
{
    return kmalloc_int(sz, true, phys);
}

