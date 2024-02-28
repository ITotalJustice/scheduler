#include "scheduler.h"

static void find_next_event(struct Scheduler* s) {
    int i;
    int new_id = 0;
    for (i = 1; i < SchedulerID_MAX; i++) {
        /* this implicitly checks if the event is enabled :big_brain: */
        if (s->events[i].time < s->events[new_id].time) {
            new_id = i;
        }
    }
    s->next_event_id = new_id;
}

static void default_reset_event(void* user, enum SchedulerID id, unsigned cycles_late_unused) {
    struct Scheduler* s = (struct Scheduler*)user;
    scheduler_reset_event(s);
    scheduler_add_absolute(s, id, SCHEDULER_TIMEOUT_CYCLES, default_reset_event, user);
}

void scheduler_reset(struct Scheduler* s, int starting_cycles, SchedulerCallback reset_cb, void* user) {
    int i;
    for (i = 0; i < SchedulerID_MAX; i++) {
        s->events[i].time = SCHEDULER_DISABLE_VALUE;
    }

    s->next_event_id = 0;
    s->cycles = starting_cycles < SCHEDULER_TIMEOUT_CYCLES ? starting_cycles : SCHEDULER_TIMEOUT_CYCLES;
    scheduler_add_absolute(s, SchedulerID_TIMEOUT, SCHEDULER_TIMEOUT_CYCLES, reset_cb ? reset_cb : default_reset_event, user ? user : s);
}

void scheduler_fire(struct Scheduler* s) {
    while (scheduler_should_fire(s)) {
        /* store local copy. */
        const enum SchedulerID id = s->next_event_id;
        const struct SchedulerEvent event = s->events[id];

        /* mark as disabled and find next event. */
        s->events[id].time = SCHEDULER_DISABLE_VALUE;
        find_next_event(s);

        /* call the expired event. */
        s->callbacks[id].callback(s->callbacks[id].user, id, s->cycles - event.time);
    }
}

void scheduler_add(struct Scheduler* s, enum SchedulerID id, int event_time, SchedulerCallback cb, void* user) {
    scheduler_add_absolute(s, id, s->cycles + event_time, cb, user);
}

void scheduler_add_absolute(struct Scheduler* s, enum SchedulerID id, int event_time, SchedulerCallback cb, void* user) {
    s->events[id].time = event_time;
    s->callbacks[id].callback = cb;
    s->callbacks[id].user = user;

    /* check if we updated the next event. */
    if (id == s->next_event_id) {
        find_next_event(s);
    }
    /* check if new event fires earlier. */
    else if (s->events[id].time < s->events[s->next_event_id].time) {
        s->next_event_id = id;
    }
}

void scheduler_remove(struct Scheduler* s, enum SchedulerID id) {
    s->events[id].time = SCHEDULER_DISABLE_VALUE;
    if (id == s->next_event_id) {
        find_next_event(s);
    }
}

void scheduler_advance_to_next_event(struct Scheduler* s) {
    /* only advance if the next event time is greater than current time. */
    if (s->events[s->next_event_id].time > s->cycles) {
        s->cycles = s->events[s->next_event_id].time;
    }
}

void scheduler_dummy_event(void* user_unused, enum SchedulerID id_unused, unsigned cycles_late_unused) {
}

void scheduler_reset_event(struct Scheduler* s) {
    /* no sort because order remains the same. */
    int i;
    for (i = 0; i < SchedulerID_MAX; i++) {
        struct SchedulerEvent* e = &s->events[i];
        if (e->time != SCHEDULER_DISABLE_VALUE) {
            e->time -= SCHEDULER_TIMEOUT_CYCLES;
        }
    }
    s->cycles -= SCHEDULER_TIMEOUT_CYCLES;
}
