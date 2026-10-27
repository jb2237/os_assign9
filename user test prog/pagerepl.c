#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_FRAMES 10

// FIFO Implementation
int run_fifo(int frames_count, int pages[], int n, int verbose) {
  int frames[MAX_FRAMES];
  int faults = 0;
  int head = 0;

  for (int i = 0; i < frames_count; i++) frames[i] = -1;

  for (int i = 0; i < n; i++) {
    int page = pages[i];
    int hit = 0;

    for (int j = 0; j < frames_count; j++) {
      if (frames[j] == page) {
        hit = 1;
        break;
      }
    }

    if (!hit) {
      faults++;
      frames[head] = page;
      head = (head + 1) % frames_count;
    }

    if (verbose) {
      printf(1, "Ref %d: Frames [", page);
      for (int j = 0; j < frames_count; j++) {
        if (frames[j] == -1) printf(1, " -");
        else printf(1, " %d", frames[j]);
      }
      printf(1, " ] -> %s\n", hit ? "HIT" : "FAULT");
    }
  }
  return faults;
}

// LRU Implementation
int run_lru(int frames_count, int pages[], int n, int verbose) {
  int frames[MAX_FRAMES];
  int last_used[MAX_FRAMES];
  int faults = 0;

  for (int i = 0; i < frames_count; i++) {
    frames[i] = -1;
    last_used[i] = -1;
  }

  for (int i = 0; i < n; i++) {
    int page = pages[i];
    int hit = 0;

    for (int j = 0; j < frames_count; j++) {
      if (frames[j] == page) {
        hit = 1;
        last_used[j] = i;
        break;
      }
    }

    if (!hit) {
      faults++;
      int lru_idx = 0;
      int min_time = 999999;

      for (int j = 0; j < frames_count; j++) {
        if (frames[j] == -1) {
          lru_idx = j;
          break;
        }
        if (last_used[j] < min_time) {
          min_time = last_used[j];
          lru_idx = j;
        }
      }

      frames[lru_idx] = page;
      last_used[lru_idx] = i;
    }

    if (verbose) {
      printf(1, "Ref %d: Frames [", page);
      for (int j = 0; j < frames_count; j++) {
        if (frames[j] == -1) printf(1, " -");
        else printf(1, " %d", frames[j]);
      }
      printf(1, " ] -> %s\n", hit ? "HIT" : "FAULT");
    }
  }
  return faults;
}

int main(int argc, char *argv[]) {
  int test_ref[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 7, 0, 1};
  int n = sizeof(test_ref) / sizeof(test_ref[0]);

  printf(1, "=== Task (b): Test with 3 frames ===\n");
  printf(1, "--- FIFO ---\n");
  int fifo_faults = run_fifo(3, test_ref, n, 1);
  printf(1, "--- LRU ---\n");
  int lru_faults = run_lru(3, test_ref, n, 1);

  printf(1, "\nSummary (3 Frames):\n");
  printf(1, "FIFO Faults: %d | LRU Faults: %d\n\n", fifo_faults, lru_faults);

  printf(1, "=== Task (c): Comparison Table (3, 4, 5 frames) ===\n");
  printf(1, "+--------+-------------+------------+------------+------------+\n");
  printf(1, "| Frames | FIFO Faults | FIFO Hit%%  | LRU Faults |  LRU Hit%%  |\n");
  printf(1, "+--------+-------------+------------+------------+------------+\n");

  for (int f = 3; f <= 5; f++) {
    int f_faults = run_fifo(f, test_ref, n, 0);
    int l_faults = run_lru(f, test_ref, n, 0);
    int f_hits = n - f_faults;
    int l_hits = n - l_faults;

    int f_hit_ratio = (f_hits * 100) / n;
    int l_hit_ratio = (l_hits * 100) / n;

    printf(1, "|   %d    |     %2d      |    %2d%%    |     %2d     |    %2d%%    |\n",
           f, f_faults, f_hit_ratio, l_faults, l_hit_ratio);
  }
  printf(1, "+--------+-------------+------------+------------+------------+\n");

  exit();
}