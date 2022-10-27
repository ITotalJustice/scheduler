# scheduler
simple-ish, fast-ish, generic-ish scheduler implementation (for emulators).

requires `C++20` as everything is `constexpr`, but the `constexpr` keyword can be removed, which drops
the required version to `C++14`.

## EXAMPLE: how to keep track of delta

```c++
struct DeltaManager
{
    s32 deltas[ID::END]{};

    constexpr void reset()
    {
        for (auto& delta : deltas) { delta = 0; }
    }

    constexpr void add(s32 id, s32 delta)
    {
        deltas[id] = delta;
    }

    constexpr void remove(s32 id)
    {
        deltas[id] = 0;
    }

    [[nodiscard]] constexpr auto get(s32 id, s32 time) -> s32
    {
        return time + deltas[id];
    }
};

DeltaManager deltas;

void event_callback(void* user, s32 id, s32 late)
{
    deltas.add(id, late);
    // ... do stuff here ...
    scheduler.add(id, deltas.get(id, 100), event_callback, user);
}

void on_disable_apu()
{
    deltas.remove(ID::APU);
    scheduler.remove(ID::APU);
}
```

## EXAMPLE: how to implement save/load state

```c++
enum ID {
    PPU,
    APU,
    TIMER,
    DMA,
    MAX,
};

static_assert(ID::MAX < scheduler::RESERVED_ID);

struct EventEntry {
    std::int32_t enabled; // don't use bool here because padding!
    std::int32_t time;
};

void savestate()
{
    std::array<EventEntry, ID::MAX> events{};
    s32 scheduler_cycles = scheduler.get_ticks();

    for (std::size_t i = 0; i < events.size(); i++)
    {
        // see if we have this event in queue, if we do, it's enabled
        if (scheduler.has_event(i))
        {
            events[i].enabled = true;
            events[i].cycles = scheduler.get_event_cycles_absolute(i);
        }
    }

    // write state data
    write(events.data(), events.size());
    write(&scheduler_cycles, sizeof(scheduler_cycles));
}

void loadstate()
{
    std::array<EventEntry, ID::MAX> events;
    s32 scheduler_cycles;

    // read state data
    read(&scheduler_cycles, sizeof(scheduler_cycles));
    read(events.data(), events.size());

    // need to reset scheduler to remove all events and reset
    // to the saved time.
    scheduler.reset(scheduler_cycles);

    for (std::size_t i = 0; i < events.size(); i++)
    {
        if (events[i].enabled)
        {
            scheduler.add_absolute(i, events[i].cycles, callback);
        }
    }
}
```

## projects using scheduler.hpp

https://github.com/ITotalJustice/notorious_beeg

## credits

implementation is based on discussion <https://github.com/dolphin-emu/dolphin/pull/4168>
