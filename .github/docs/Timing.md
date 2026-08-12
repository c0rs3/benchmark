# Timing Documentation

## [Back to Menu](../../README.md#documentation)

## Clocks (`ChronoType`)

### `benchtools::clock::CpuClock<clock::CPU::Process>`

- Clock for timing the CPU (process) time

### `benchtools::clock::CpuClock<clock::CPU::Thread>`

- Clock for timing the CPU (thread) time

### `benchtools::clock::CpuClock<WallClock>`

- Clock for timing the Wall (system) time

## Timers

### `benchtools::timer::Timer<ChronoType>`

- Regular plain-old timer

#### Usage

```cpp
auto timer = Timer<...>; // preferred timer

timer.start(); // start the timing

timer.stop(); // start the timing

timer.duration(); // gets the duration

timer.reset(); // resets the duration

benchtool::time::durationCast(Duration{}, time::unit::microseconds); // cast it to a preferred unit
```

### `benchtools::timer::FileTimer<ChronoType>`

- Timer that logs the line, column, function, date and time when `start()` is called to a file

#### Usage Example

```cpp
auto fileTimer = timer::FileTimer<clock::WallClock>{"..."}; // path

fileTimer.start();
...
fileTimer.reset();
fileTimer.start();
...
fileTimer.stop();

fileTimer.start(1);// reset before starting
...
fileTimer.stop();
```

- Produces:

```txt
[2026-08-12 18:25:06][INFO]LOGGING SESSION STARTED
[2026-08-12 18:25:06][INFO]/home/t/code/projects/benchmark/src/Test/main.cpp:48:19 int main() Started timer

[2026-08-12 18:25:07][INFO]/home/t/code/projects/benchmark/src/Test/main.cpp:50 Timer started at: 48:19 int main(), resulted with: 993880967ns

[2026-08-12 18:25:07][INFO]/home/t/code/projects/benchmark/src/Test/main.cpp:52:19 int main() Started timer

[2026-08-12 18:25:08][INFO]/home/t/code/projects/benchmark/src/Test/main.cpp:54 Timer started at: 52:19 int main(), resulted with: 971936819ns

[2026-08-12 18:25:08][INFO]/home/t/code/projects/benchmark/src/Test/main.cpp:56:19 int main() Started timer

[2026-08-12 18:25:09][INFO]/home/t/code/projects/benchmark/src/Test/main.cpp:58 Timer started at: 56:19 int main(), resulted with: 0ns

[2026-08-12 18:25:09][INFO]LOGGING SESSION ENDED
```

### `benchtools::timer::LoggingTimer<ChronoType>`

- Timer that logs the line, column, function, date and time when `start()` is called to stdout using `spdlog`

#### Usage Exm

```cpp
auto logTimer = timer::LoggingTimer<clock::WallClock>{};
logTimer.start();
...
logTimer.reset();
logTimer.start();
...
logTimer.stop();

logTimer.start(1); // reset before starting
...
logTimer.stop();
```

- Produces:

```txt
[2026-08-12 18:25:03.308] [GLOBAL] [trace] /home/t/code/projects/benchmark/src/Test/main.cpp:31:18 int main() Started timer
[2026-08-12 18:25:04.880] [GLOBAL] [trace] /home/t/code/projects/benchmark/src/Test/main.cpp:33 Timer started at: 31:18 int main(), resulted with: 1571904310ns
[2026-08-12 18:25:04.880] [GLOBAL] [trace] /home/t/code/projects/benchmark/src/Test/main.cpp:35:18 int main() Started timer
[2026-08-12 18:25:05.789] [GLOBAL] [trace] /home/t/code/projects/benchmark/src/Test/main.cpp:37 Timer started at: 35:18 int main(), resulted with: 908464903ns
[2026-08-12 18:25:05.789] [GLOBAL] [trace] /home/t/code/projects/benchmark/src/Test/main.cpp:39:18 int main() Started timer
[2026-08-12 18:25:06.785] [GLOBAL] [trace] /home/t/code/projects/benchmark/src/Test/main.cpp:41 Timer started at: 39:18 int main(), resulted with: 0ns
```

- `benchtools::timer::ScopedTimer<Timer<>>`

```cpp
auto timer = Timer<...>; // preferred timer
{
    ScopedTimer tim{timer}; // timer passed will be CTAD
    // times the scope
}
timer.duration(); // get the duration of that scope
```
