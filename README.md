# benchtools

- Simple code benchmarking tools for C++

## Menu

- [Features](#features)
- [How to Setup](#how-to-setup)
- [Usage](#how-to-use)
- [TODO](#todo)

---

## Features

- 2 base timer types `WallTimer` for system time, `ClockTimer` for CPU time.
- Wrappers for base timer types `ScopedTimer`, `LoggingTimer` etc.
- For logging a `spdlog` wrapper `Logger` and `FileLogger` for file logging
- For `.csv` file handling `CSVStream` and `CSVParser`
- benchmarking functionality with CPU or System time policies
- Plotting benchmarking results with **ImPlot**

---

## How to Setup

---

## How to Use

---

## TODO

- [x] File logging
- [x] File logging timer
  - [x] CSV writer
  - [x] A system to add logging timers so that it includes which lines/timers took what {time}
  - [x] A system to see which timer took what time, store the line it started at display the duration of the timer in which line it was started?
- [x] A benchmarking system/task runner whatever
  - [x] Polish benchmarking module code
- [x] CSV Output of benchmark results
- [x] CSV Parsing of benchmark results
- [x] ImPlot of results
  - [x] Base code
  - [x] Make the plotting into a global static method/singleton in a separate module
  - [x] Polish the code
    - [x] Graph should display the time unit of timers
    - [x] Graph should display the timer type
- [ ] More polishing
- [ ] Flame graph functionality?
  - [ ] Timers saved to a global thing
  - [ ] that writes out shit to csv when destroyed
- [ ] Proper documentation for each timer + README
