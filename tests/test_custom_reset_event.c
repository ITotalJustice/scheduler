#include "../src/scheduler.h"
#include <stdbool.h>

static struct Scheduler s;
int dummy_user_data = 5;
int result = 255;

static void custom_reset_event(void* user, enum SchedulerID id, unsigned cycles_late) {
    if (id != SchedulerID_TIMEOUT) {
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
    scheduler_reset(&s, 0, custom_reset_event, &dummy_user_data);
    scheduler_tick(&s, SCHEDULER_TIMEOUT_CYCLES);
    if (!scheduler_should_fire(&s)) {
        return 1;
    }
    scheduler_fire(&s);
    return result;
}
