# Benchmarking

## What is measured

```
begin = DateTime::timestampInMilliseconds();
exit  = system(commandLine);
end   = DateTime::timestampInMilliseconds();
print end - begin
```

The number is the **elapsed wall clock time** of `system()`:

- **included:** starting the shell (`cmd.exe` / `/bin/sh`), the command,
  every process the command starts and waits for, time spent waiting for
  disk, network or a busy CPU;
- **not included:** the start and exit of `time-cmd` itself, processes the
  command leaves running in the background;
- **not measured at all:** CPU time (user / system), memory, I/O counts.
  For those use `/usr/bin/time -v` on Linux or a profiler.

The clock is `XYO::System::DateTime::timestampInMilliseconds()`, the
**local** date and time in milliseconds (Windows `GetLocalTime`, Linux
`gettimeofday` + `localtime_r`). It is not a monotonic clock; see
[Known issues](#known-issues).

## Resolution

The result is a whole number of milliseconds, truncated. The real
resolution is the update interval of the system clock: 1 ms on Linux; on
Windows usually 1 ms as well, but up to about 16 ms on systems where the
timer runs at its default rate. Treat differences of a few milliseconds as
noise.

## Overhead

Run `time-cmd` with no command to see the fixed cost of starting the
shell on the machine:

| System | `time-cmd` (empty command) | Measured with |
|--------|----------------------------|---------------|
| Windows 11, `cmd.exe` | 15 – 25 ms | `time-cmd` 5.9.0 |
| Ubuntu 24.04 (WSL), `dash` | 1 – 3 ms | `time-cmd` 5.9.0 |

The overhead is the same for every run, so it cancels out when two
versions of a program are compared. For an absolute number, subtract it.
Commands shorter than about 100 ms are dominated by overhead and
resolution: time a loop of many runs instead (inside the command, so the
shell starts only once).

## Getting reliable numbers

1. **Warm up.** The first run reads files from disk; later runs find them
   in the file cache. Run once and discard the result, unless the cold
   start is what you want to measure (then flush the cache or reboot
   between runs).
2. **Repeat** at least 5 times and use the **median** (or the minimum for
   pure CPU work); one run says little.
3. **Keep the machine quiet.** Close other programs, wait for indexing,
   updates and antivirus scans to finish, use the same power plan and keep
   the laptop on mains power.
4. **Compare like with like.** Same machine, same input, same build type
   (release vs debug), same command line, one change at a time.
5. **Do not let the console slow the command down.** Printing a lot of
   output to a terminal can take longer than the work itself. Redirect the
   command's output (inside the quotes) when you measure the work, not the
   printing: `time-cmd "tool > nul"` / `time-cmd 'tool > /dev/null'`.

Repeat on Windows (`cmd.exe`, use `%%i` inside a `.cmd` file):

```bat
for /l %i in (1,1,5) do @time-cmd "fabricare clean > nul && fabricare make > nul"
```

Repeat on Linux and print the median:

```bash
for i in 1 2 3 4 5; do
	time-cmd 'make -B > /dev/null' | sed -n 's/^Execution time: \([0-9]*\) ms$/\1/p'
done | sort -n | sed -n 3p
```

For statistical analysis (mean, deviation, outliers, comparison of several
commands) a dedicated tool such as
[hyperfine](https://github.com/sharkdp/hyperfine) does more; `time-cmd`
is for the quick, same-on-every-system number.

## Known issues

| Issue | Effect | Work-around |
|-------|--------|-------------|
| **Wall clock, local time.** The clock follows changes of the system time: NTP corrections, a manual change, and the daylight saving switch | a run across the switch is off by one hour; if the clock goes **back** during the run, `end - begin` wraps around and a huge number is printed | do not benchmark across the DST switch or while the clock is being set; repeat runs and drop outliers |
| **Quotes are not kept.** Arguments are joined with spaces, the quotes removed by the calling shell are not restored | a path or argument with spaces is split | pass the whole command as one quoted argument ([Command line](command-line.md#quoting-and-shell-operators)) |
| **Unknown `--options` are run.** Only `--help`, `--usage`, `--license`, `--version` are options | `time-cmd --verbose make` runs the command `--verbose make` | put options of the timed command after its name |
| **Only the first argument is checked for options.** | `time-cmd make --version` times `make --version` (correct); `time-cmd --version make` prints the version and runs nothing | — |
