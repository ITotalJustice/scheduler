#include "scheduler.h"
#include <stddef.h>
#include <stdbool.h>

static struct Scheduler s;
static int ticker = 0;

enum Event {
    Event_MAX,
};

static bool tick_compare(int cycles) {
    ticker += cycles;
    scheduler_tick(&s, cycles);
    return scheduler_get_ticks(&s) == ticker;
}

int main() {
    scheduler_init(&s, Event_MAX);
    scheduler_reset(&s, 0, NULL, NULL);

    for (int i = 0; i < 1000; i++) {
        if (!tick_compare(i)) {
            return 1;
        }
    }
    scheduler_quit(&s);
    return 0;
}
