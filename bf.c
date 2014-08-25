#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bf_state {
  int index;
  int codeindex;
  int loopdepth;
  int loopskip;
  char memory[1024];
  char code[65536];
  int loop[256];
} bf_state;

void incr(bf_state* state) {
  if (!state->loopskip) {
    ++state->memory[state->index];
  }
}

void decr(bf_state* state) {
  if (!state->loopskip) {
    --state->memory[state->index];
  }
}

void next(bf_state* state) {
  if (!state->loopskip) {
    if (++state->index >= sizeof(state->memory)) {
      fprintf(stderr, "bf: out of memory");
      exit(EXIT_FAILURE);
    }
  }
}

void prev(bf_state* state) {
  if (!state->loopskip) {
    if (--state->index < 0) {
      fprintf(stderr, "bf: negative address access");
      exit(EXIT_FAILURE);
    }
  }
}

void put(bf_state* state) {
  if (!state->loopskip) {
    putchar(state->memory[state->index]);
    fflush(stdout);
  }
}

void get(bf_state* state) {
  if (!state->loopskip) {
    state->memory[state->index] = getchar();
  }
}

void loopstart(bf_state* state) {
  if (!state->loopskip) {
    if (state->loopdepth >= sizeof(state->loop) - 1) {
      fprintf(stderr, "bf: too deep loops");
      exit(EXIT_FAILURE);
    }
  }
  state->loop[state->loopdepth++] = state->codeindex;
  if (!state->loopskip) {
    if (state->memory[state->index] == 0) {
      state->loopskip = state->loopdepth;
    }
  }
}

void loopend(bf_state* state) {
  if (--state->loopdepth < 0) {
    fprintf(stderr, "bf: too much loop close");
    exit(EXIT_FAILURE);
  }
  if (!state->loopskip) {
    if (state->memory[state->index] != 0) {
      state->codeindex = state->loop[state->loopdepth] - 1;
    }
  } else {
    if (state->memory[state->index] == 0) {
      if (state->loopskip == state->loopdepth + 1) {
        state->loopskip = 0;
      }
    }
  }
}

void clean(bf_state* state) {
  if (state->loopdepth == 0) {
    state->code[0] = '\0';
    state->codeindex = 0;
  }
}

void run(bf_state* state) {
  while (state->codeindex < sizeof(state->code)) {
    switch (state->code[state->codeindex++]) {
      case '+': incr(state); break;
      case '-': decr(state); break;
      case '>': next(state); break;
      case '<': prev(state); break;
      case '.': put(state);  break;
      case ',': get(state);  break;
      case '[': loopstart(state); break;
      case ']': loopend(state); break;
      case '\0': --state->codeindex; clean(state); return;
    }
  }
}

void cat(bf_state* state, char* buffer) {
  if (state->codeindex + strlen(buffer) < sizeof(state->code)) {
    strcat(state->code, buffer);
  } else {
    fprintf(stderr, "bf: input code too long");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  char buffer[256];
  int i, bufferlen;
  bf_state state = { 0, 0, 0, 0, { 0 }, { 0 }, { 0 } };
  while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    cat(&state, buffer);
    run(&state);
  }
  return 0;
}
