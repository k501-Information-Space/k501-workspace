#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "frame_engine.h"
#include "qh_core.h"

#define CHUNK_SIZE 4096

/* READ FILE */
static char *read_file(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    if (size <= 0) {
        fclose(f);
        return NULL;
    }

    char *buf = malloc(size);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    size_t read_bytes = fread(buf, 1, size, f);
    fclose(f);

    if (read_bytes != (size_t)size) {
        free(buf);
        return NULL;
    }

    *len = size;
    return buf;
}

/* PARSE → NORMALIZED SET */
int k501_parse_batch(const K501_DocumentSet *docs, K501_NormalizedSet *out) {
    out->units = NULL;
    out->count = 0;

    for (size_t i = 0; i < docs->count; i++) {

        size_t len = 0;
        char *data = read_file(docs->docs[i].path, &len);

        if (!data) continue;

        out->units = realloc(out->units,
            sizeof(K501_NormalizedUnit) * (out->count + 1));

        out->units[out->count].data = data;
        out->units[out->count].len = len;

        out->count++;
    }

    return 0;
}

/* FRAME BUILD WITH CHUNKING */
int k501_frame_build(const K501_NormalizedSet *norm, K501_State *state_out) {
    state_out->frames = NULL;
    state_out->count = 0;

    for (size_t i = 0; i < norm->count; i++) {

        const char *data = norm->units[i].data;
        size_t len = norm->units[i].len;

        size_t offset = 0;

        while (offset < len) {

            size_t chunk_len = CHUNK_SIZE;
            if (offset + chunk_len > len)
                chunk_len = len - offset;

            state_out->frames = realloc(
                state_out->frames,
                sizeof(K501_Frame) * (state_out->count + 1)
            );

            K501_Frame *f = &state_out->frames[state_out->count];

            f->payload = malloc(chunk_len);
            memcpy(f->payload, data + offset, chunk_len);
            f->payload_len = chunk_len;

            k501_hash_compute(
                (uint8_t*)f->payload,
                f->payload_len,
                f->hash
            );

            state_out->count++;

            offset += chunk_len;
        }
    }

    return 0;
}