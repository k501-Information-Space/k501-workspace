#include <stdio.h>
#include "hard_index.h"

int k501_write_frames_ndjson(const K501_State *state, const char *file_path) {
    FILE *f = fopen(file_path, "w");
    if (!f) return -1;

    for (size_t i = 0; i < state->count; i++) {
        fprintf(f, "{\"id\":%zu,\"hash\":\"", i);
        for (int j = 0; j < 32; j++)
            fprintf(f, "%02x", state->frames[i].hash[j]);
        fprintf(f, "\"}\n");
    }

    fclose(f);
    return 0;
}