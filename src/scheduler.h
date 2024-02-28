#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/* this is the number of cycles until scheduler_reset_event is called. */
enum { SCHEDULER_TIMEOUT_CYCLES = (INT_MAX / 2) };
/* this is the value that events are set to when disabled. */
enum { SCHEDULER_DISABLE_VALUE = INT_MAX };

enum SchedulerID {
    /* start of the events, do not remove. */
    SchedulerID_TIMEOUT = 0,

    /*---CUSTOM EVENTS---*/
    /*---END OF CUSTOM EVENTS---*/

    /* end of the events, do not remove. */
    SchedulerID_MAX,
};

typedef void(*SchedulerCallback)(
    void* user, /* your userdata. */
    enum SchedulerID id, /* the unique id of your event. */
    unsigned cycles_late /* how many cycles late the event is. */
);

struct SchedulerEvent {
    int time; /* time until event expires (scheduler.cycles + event.cycle). */
};

struct SchedulerEventCallback {
    SchedulerCallback callback; /* function to call on event expire. */
    void* user; /* user data passed to the callback. */
};

struct Scheduler {
    int cycles;
    enum SchedulerID next_event_id;
    struct SchedulerEvent events[SchedulerID_MAX];
    struct SchedulerEventCallback callbacks[SchedulerID_MAX];
};

/* resets queue and cycles, adds reset event, optional custom callback. */
void scheduler_reset(struct Scheduler* s, int starting_cycles, SchedulerCallback reset_cb, void* user);
/* fires all expired events. */
void scheduler_fire(struct Scheduler* s);
/* adds relative new / existing event. updates time,cb,user if existing. */
void scheduler_add(struct Scheduler* s, enum SchedulerID id, int event_time, SchedulerCallback cb, void* user);
/* adds new / existing event. updates time,cb,user if existing. */
void scheduler_add_absolute(struct Scheduler* s, enum SchedulerID id, int event_time, SchedulerCallback cb, void* user);
/* removes an event, does nothing if event not enabled. */
void scheduler_remove(struct Scheduler* s, enum SchedulerID id);
/* advances scheduler so that get_ticks() == get_next_event_cycles() if event has greater cycles. */
void scheduler_advance_to_next_event(struct Scheduler* s);
/* call this during reset_event if you override the event in scheduler_reset() */
void scheduler_reset_event(struct Scheduler* s);
/* use this for empty events, such as signaling to break from a loop. */
void scheduler_dummy_event(void* user, enum SchedulerID id, unsigned cycles_late);

/* advance scheduler by number of ticks. */
static inline void scheduler_tick(struct Scheduler* s, int ticks) {
    s->cycles += ticks;
}

/* return true if fire() should be called. */
static inline bool scheduler_should_fire(const struct Scheduler* s) {
    return s->events[s->next_event_id].time <= s->cycles;
}

/* returns current time of the scheduler. */
static inline int scheduler_get_ticks(const struct Scheduler* s) {
    return s->cycles;
}

/* returns if an event is found with matching id. */
static inline bool scheduler_has_event(const struct Scheduler* s, enum SchedulerID id) {
    return s->events[id].time != SCHEDULER_DISABLE_VALUE;
}

/* returns event cycles - get_ticks(), call has_event() first. */
static inline int scheduler_get_event_cycles(const struct Scheduler* s, enum SchedulerID id) {
    assert(scheduler_has_event(s, id) && "event isn't enabled");
    return s->events[id].time - scheduler_get_ticks(s);
}

/* returns event cycles, call has_event() first. */
static inline int scheduler_get_event_cycles_absolute(const struct Scheduler* s, enum SchedulerID id) {
    assert(scheduler_has_event(s, id) && "event isn't enabled");
    return s->events[id].time;
}

/* return cycles - get_ticks() of next event. */
static inline int scheduler_get_next_event_cycles(const struct Scheduler* s) {
    return s->events[s->next_event_id].time - scheduler_get_ticks(s);
}

/* return cycles of next event. */
static inline int scheduler_get_next_event_cycles_absolute(const struct Scheduler* s) {
    return s->events[s->next_event_id].time;
}

#ifdef __cplusplus
}
#endif
#endif /* _SCHEDULER_H_ */
