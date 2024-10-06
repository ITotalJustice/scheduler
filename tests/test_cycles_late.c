#include "scheduler.h"
#include <stdbool.h>

static struct Scheduler s;
static const unsigned tick_cycles = SCHEDULER_TIMEOUT_CYCLES + 33;
int result = 255;

enum Event {
    Event_MAX,
};

static void custom_reset_event(void* user, unsigned id, unsigned cycles_late) {
    if (id != Event_MAX) {
        result = 2;
        return;
    }

    if (cycles_late != tick_cycles - SCHEDULER_TIMEOUT_CYCLES) {
        result = 3;
        return;
    }

    scheduler_reset_event(&s);
    result = 0;
}

int main() {
    scheduler_init(&s, Event_MAX);
    scheduler_reset(&s, 0, custom_reset_event, &s);
    scheduler_tick(&s, tick_cycles);
    if (!scheduler_should_fire(&s)) {
        return 1;
    }
    scheduler_fire(&s);
    scheduler_quit(&s);
    return result;
}
