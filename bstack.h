#pragma once

#include "butil.h"

typedef struct bstack bstack;

typedef void (*bstack_growth_fun)(const bstack* stack, size_t* alloc_size);

typedef struct bstack {
    void* alloc_ptr;
    size_t element_capacity;
    size_t element_count;
    size_t element_size;
    bstack_growth_fun growth_fun;
    // todo: allow user defined data for scaling
} bstack;

void bstack_growth_fun_default(const bstack*, size_t* alloc_size)  { (*alloc_size) *= 2; }

void bstack_init(bstack* stack, size_t initial_capacity, size_t element_size, bstack_growth_fun growth_fun)
{
    stack->alloc_ptr = xmalloc(initial_capacity * element_size);
    stack->element_count = 0;
    stack->element_capacity = initial_capacity;
    stack->element_size = element_size;
    stack->growth_fun = growth_fun? growth_fun : bstack_growth_fun_default;
}

void bstack_push(bstack* stack, const void* value)
{
    if (stack->element_count >= stack->element_capacity)
    {
        size_t new_capacity = stack->element_capacity;
        stack->growth_fun(stack, &new_capacity);

        if (new_capacity <= stack->element_capacity)
            new_capacity = stack->element_capacity + 1;

        stack->alloc_ptr = xrealloc(stack->alloc_ptr, new_capacity * stack->element_size);
        stack->element_capacity = new_capacity;
    }

    void* target = (char*)stack->alloc_ptr + (stack->element_count * stack->element_size);
    memcpy(target, value, stack->element_size);
    stack->element_count++;
}

void bstack_pop(bstack* stack, void* out_value)
{
    if (stack->element_count == 0)
    {
        cry("attempt to pop from empty stack");
        return;
    }

    stack->element_count--;
    void* source = (char*)stack->alloc_ptr + (stack->element_count * stack->element_size);
    if (out_value)
        memcpy(out_value, source, stack->element_size);
}

void bstack_peek(const bstack* stack, void* out_value)
{
    if (stack->element_count == 0)
        return;

    void* source = (char*)stack->alloc_ptr + ((stack->element_count - 1) * stack->element_size);
    memcpy(out_value, source, stack->element_size);
}

void bstack_free(bstack* stack)
{
    free(stack->alloc_ptr);
    stack->alloc_ptr = NULL;
    stack->element_capacity = 0;
    stack->element_count = 0;
    stack->element_size = 0;
    stack->growth_fun = NULL;
}
