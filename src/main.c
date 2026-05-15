#include <stdio.h>
#include <stdlib.h>

#include "ingestion.h"
#include "frame_engine.h"
#include "iteration.h"
#include "hard_index.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: %s <dir>\n", argv[0]);
        return 1;
    }

    /* --- STATE OBJECTS --- */
    K501_DocumentSet docs = {0};
    K501_NormalizedSet norm = {0};
    K501_State state = {0};
    K501_State final = {0};

    /* --- PHASE 1 + 2: INGESTION + DEEP READ --- */
    if (k501_ingest_directory_recursive(argv[1], 2, &docs) != 0) {
        printf("ERROR: ingestion failed\n");
        return 1;
    }

    if (docs.count == 0) {
        printf("ERROR: no valid files found\n");
        return 1;
    }

    /* --- PHASE 3: PARSING --- */
    if (k501_parse_batch(&docs, &norm) != 0) {
        printf("ERROR: parsing failed\n");
        return 1;
    }

    if (norm.count == 0) {
        printf("ERROR: parsing produced no data\n");
        return 1;
    }

    /* --- PHASE 4: STRUCTURING (FRAME BUILD) --- */
    if (k501_frame_build(&norm, &state) != 0) {
        printf("ERROR: frame build failed\n");
        return 1;
    }

    if (state.count == 0) {
        printf("ERROR: no frames built\n");
        return 1;
    }

    /* --- PHASE 5: ITERATION (FIXPOINT) --- */
    if (k501_iterate_fixpoint(&state, &final, 10) != 0) {
        printf("ERROR: iteration failed\n");
        return 1;
    }

    if (final.count == 0) {
        printf("ERROR: empty final state\n");
        return 1;
    }

    /* --- PHASE 6: OUTPUT --- */
    if (k501_write_frames_ndjson(&final, "output.ndjson") != 0) {
        printf("ERROR: write failed\n");
        return 1;
    }

    /* --- SUCCESS --- */
    printf("SUCCESS: %zu frames\n", final.count);

    return 0;
}