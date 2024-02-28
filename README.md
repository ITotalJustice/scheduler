# scheduler

simple, fast and generic scheduler implementation for emulators, writtin in c89.

## how to add this to your emulator

1. copy the 2 files from `src/` to your project (scheduler.c and scheduler.h)
2. add any events you need to `enum SchedulerID`.
3. call `scheduler_add()` to add events, `scheduler_tick()` to tick the scheduler, `scheduler_should_fire()` to check if an event is ready to fire, `scheduler_fire()` to fire all expired.
4. read the comments in the header file for futher documentation.

## EXAMPLE: how to implement save/load state

```c
struct StateEventEntry {
    int32_t enabled; // don't use bool here because padding!
    int32_t cycles;
};

struct StateTiming {
    int32_t scheduler_cycles;
    struct StateEventEntry events[SchedulerID_MAX];
};

void savestate(void) {
    struct StateTiming state_timing;
    state_timing.scheduler_cycles = scheduler_get_ticks(&s);

    for (int i = 0; i < SchedulerID_MAX; i++) {
        struct StateEventEntry* e = &state_timing.events[i];
        // see if we have this event in queue, if we do, it's enabled
        if (scheduler_has_event(&s, i)) {
            e->enabled = 1;
            e->cycles = scheduler_get_event_cycles_absolute(&s, i);
        } else {
            e->enabled = 0;
        }
    }

    // write state data
    write(&state_timing, sizeof(state_timing));
}

void loadstate(void) {
    struct StateTiming state_timing = {0};

    // read state data
    read(&state_timing, sizeof(state_timing));

    // need to reset scheduler to remove all events and reset
    // to the saved time.
    scheduler_reset(&s, state_timing.scheduler_cycles, NULL, NULL);

    for (int i = 0; i < SchedulerID_MAX; i++) {
        const struct StateEventEntry* e = &state_timing.events[i];
        if (e->enabled) {
            switch (i) {
                case SchedulerID_APU:
                    scheduler_add_absolute(&s, i, e->cycles, on_apu_event, user);
                    break;

                case SchedulerID_PPU:
                    scheduler_add_absolute(&s, i, e->cycles, on_ppu_event, user);
                    break;

                case SchedulerID_TIMER:
                    scheduler_add_absolute(&s, i, e->cycles, on_timer_event, user);
                    break;
            }
        }
    }
}
```

## projects using scheduler

- https://github.com/ITotalJustice/notorious_beeg (not upstream)
- https://github.com/ITotalJustice/TotalSMS (not upstream)
- https://github.com/ITotalJustice/TotalGB (not upstream)
