
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define W 99
#define H 99
#define IPT "input.txt"

char data[W * H];

typedef bool (*Payload)(int, void*);

bool scanLeft(Payload p, int x, int y, void* state) {
  int k;
  for (k = x - 1; k >= 0; --k) {
    if (!p(data[y * W + k], state)) {
      return false;
    }
  }
  return true;
}

bool scanRight(Payload p, int x, int y, void* state) {
  int k;
  for (k = x + 1; k < W; ++k) {
    if (!p(data[y * W + k], state)) {
      return false;
    }
  }
  return true;
}

bool scanAbove(Payload p, int x, int y, void* state) {
  int k;
  for (k = y - 1; k >= 0; --k) {
    if (!p(data[k * W + x], state)) {
      return false;
    }
  }
  return true;
}

bool scanBelow(Payload p, int x, int y, void* state) {
  int k;
  for (k = y + 1; k < H; ++k) {
    if (!p(data[k * W + x], state)) {
      return false;
    }
  }
  return true;
}

typedef struct {
  int val;
} CoverScanner;

bool coverPayload(int val, void* state) { return val < ((CoverScanner*)state)->val; }

int coverCompute() {
  CoverScanner s;
  int x, y, num;

  num = W * 2 + H * 2 - 4;  // edges
  for (y = 1; y < H - 1; ++y) {
    for (x = 1; x < W - 1; ++x) {
      s.val = data[y * W + x];
      num += 
        !scanLeft(coverPayload, x, y, &s) && 
        !scanRight(coverPayload, x, y, &s) && 
        !scanAbove(coverPayload, x, y, &s) && 
        !scanBelow(coverPayload, x, y, &s) 
        ? 0 : 1;
    }
  }
  return num;
}

typedef struct {
  int val, idx, vis[4];
} SceneScanner;

bool scenePayload(int val, void* state) {
  SceneScanner* s = (SceneScanner*)state;
  s->vis[s->idx]++;
  return val < s->val;
}

int sceneCompute() {
  SceneScanner s;
  int x, y, val, max, buf[W * H];

  memset(buf, 0, W * H);
  for (y = 0; y < H; ++y) {
    for (x = 0; x < W; ++x) {
      memset(&s, 0, sizeof(SceneScanner));
      s.val = data[y * W + x];

      s.idx = 0;
      scanLeft(scenePayload, x, y, &s);

      s.idx = 1;
      scanRight(scenePayload, x, y, &s);

      s.idx = 2;
      scanAbove(scenePayload, x, y, &s);

      s.idx = 3;
      scanBelow(scenePayload, x, y, &s);

      buf[y * W + x] = s.vis[0] * s.vis[1] * s.vis[2] * s.vis[3];
    }
  }

  max = 0;
  for (y = 0; y < H; ++y) {
    for (x = 0; x < W; ++x) {
      val = buf[y * W + x];
      max = max > val ? max : val;
    }
  }

  return max;
}

int main(int argc, char** argv) {
  int y, x;
  FILE* f;

  f = fopen(IPT, "r");
  for (y = 0; y < H; ++y) {
    for (x = 0; x < W; ++x) {
      data[y * W + x] = fgetc(f) - 48;
    }
    fgetc(f);  // strip
  }

  printf("cover %i\n", coverCompute());
  printf("scene %i\n", sceneCompute());
}
