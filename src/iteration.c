#include <stdlib.h>
#include <string.h>

#include "iteration.h"

/* INTERNAL: deep copy state */
static int copy_state(const K501_State *src, K501_State *dst) {
    dst->frames = malloc(sizeof(K501_Frame) * src->count);
    if (!dst->frames) return -1;

    dst->count = src->count;

    for (size_t i = 0; i < src->count; i++) {
        memcpy(dst->frames[i].hash, src->frames[i].hash, 32);

        dst->frames[i].payload_len = src->frames[i].payload_len;

        dst->frames[i].payload = malloc(src->frames[i].payload_len);
        if (!dst->frames[i].payload) return -1;

        memcpy(
            dst->frames[i].payload,
            src->frames[i].payload,
            src->frames[i].payload_len
        );
    }

    return 0;
}

/* INTERNAL: free state */
static void free_state(K501_State *s) {
    if (!s || !s->frames) return;

    for (size_t i = 0; i < s->count; i++) {
        free(s->frames[i].payload);
    }

    free(s->frames);
    s->frames = NULL;
    s->count = 0;
}

/* INTERNAL: compare states deterministically */
static int states_equal(const K501_State *a, const K501_State *b) {
    if (a->count != b->count) return 0;

    for (size_t i = 0; i < a->count; i++) {
        if (memcmp(a->frames[i].hash, b->frames[i].hash, 32) != 0)
            return 0;

        if (a->frames[i].payload_len != b->frames[i].payload_len)
            return 0;

        if (memcmp(
                a->frames[i].payload,
                b->frames[i].payload,
                a->frames[i].payload_len
            ) != 0)
            return 0;
    }

    return 1;
}

/* OPERATOR F (CURRENTLY IDENTITY — deterministic baseline) */
static int operator_F(const K501_State *in, K501_State *out) {
    return copy_state(in, out);
}

/* FIXPOINT ENGINE */
int k501_iterate_fixpoint(
    K501_State *initial,
    K501_State *final,
    int max_iterations
) {
    if (!initial || !final) return -1;

    K501_State current = {0};
    K501_State next = {0};

    if (copy_state(initial, &current) != 0) return -1;

    for (int i = 0; i < max_iterations; i++) {

        if (operator_F(&current, &next) != 0) {
            free_state(&current);
            return -1;
        }

        if (states_equal(&current, &next)) {
            *final = next;
            free_state(&current);
            return 0;
        }

        free_state(&current);
        current = next;
        next.frames = NULL;
        next.count = 0;
    }

    /* fallback: max_iterations reached */
    *final = current;
    return 0;
}