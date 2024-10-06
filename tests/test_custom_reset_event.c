#include "scheduler.h"
#include <stdbool.h>

static struct Scheduler s;
int dummy_user_data = 5;
int result = 255;

enum Event {
    Event_MAX,
};

static void custom_reset_event(void* user, unsigned id, unsigned cycles_late) {
    if (id != Event_MAX) {
        result = 2;
        return;
    }

    if (cycles_late != 0) {
        result = 3;
        return;
    }

    if (user != &dummy_user_data)
    {
        result = 4;
        return;
    }

    if (*(int*)user != dummy_user_data)
    {
        result = 5;
        return;
    }

    scheduler_reset_event(&s);

    if (scheduler_get_ticks(&s))
    {
        result = 6;
        return;
    }

    result = 0;
}

int main() {
    scheduler_init(&s, Event_MAX);
    scheduler_reset(&s, 0, custom_reset_event, &dummy_user_data);
    scheduler_tick(&s, SCHEDULER_TIMEOUT_CYCLES);
    if (!scheduler_should_fire(&s)) {
        return 1;
    }
    scheduler_fire(&s);
    scheduler_quit(&s);
    return result;
}
