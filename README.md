# scheduler

simple, fast and generic scheduler implementation for emulators, writtin in c89.

## how to add this to your emulator

1. copy the 2 files from `src/` to your project (scheduler.c and scheduler.h)
2. call `scheduler_init()` with the maximum number of events.
3. call `scheduler_add()` to add events, `scheduler_tick()` to tick the scheduler, `scheduler_should_fire()` to check if an event is ready to fire, `scheduler_fire()` to fire all expired.
4. read the comments in the header file for futher documentation.

## projects using scheduler

- https://github.com/ITotalJustice/notorious_beeg (not upstream)
- https://github.com/ITotalJustice/TotalSMS (not upstream)
- https://github.com/ITotalJustice/TotalGB (not upstream)
- https://github.com/ITotalJustice/TotalGBS (not upstream)
