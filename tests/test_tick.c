#include "../src/scheduler.h"
#include <stddef.h>
#include <stdbool.h>

static struct Scheduler s;
static int ticker = 0;

static bool tick_compare(int cycles) {
    ticker += cycles;
    scheduler_tick(&s, cycles);
    return scheduler_get_ticks(&s) == ticker;
}

int main() {
    scheduler_reset(&s, 0, NULL, NULL);

    for (int i = 0; i < 1000; i++) {
        if (!tick_compare(i)) {
            return 1;
        }
    }

    return 0;
}
