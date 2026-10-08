---
name: time-cmd
description: >-
  How to use time-cmd, the XYO command line tool (C++, namespace
  XYO::TimeCMD, on top of xyo-system) that runs a command line through the
  system shell (cmd.exe /c, /bin/sh -c) and prints "Execution time: <N> ms"
  for benchmark purposes, the same on Windows and Linux: options (--help,
  --usage, --license, --version, only as the first argument; unknown
  --options are run as the command); how the arguments are joined with
  spaces (quotes are lost) and how to quote the whole command line for
  &&, pipes, redirection and paths with spaces (cmd.exe /c quote stripping,
  PowerShell --%); output and exit codes (the command's exit code is passed
  through via Shell::system; Linux builds up to 5.9.0 build 10 always
  exited 0); what is measured (wall clock local time, shell start
  overhead ~20 ms on Windows, 1-3 ms on Linux, ms resolution) and how to get
  reliable numbers. Use when timing or benchmarking a command or build with
  time-cmd, writing scripts that call time-cmd or parse its output, or when
  working inside the time-cmd repository.
---

# time-cmd

Runs a command line and prints how long it took:

```
time-cmd fabricare make
...the command's own output, live...
Execution time: 48213 ms
```

A tool, not a library: there is nothing to link. Built on `xyo-system`
(see the `xyo-system` skill when changing the source).

Full documentation: `docs/` in the time-cmd repository
(`X:\Storage\XYO\Gitea\CPP\time-cmd\docs` on this machine): README,
getting-started, **command-line**, **benchmarking**, reference. The whole
implementation is `source/XYO/TimeCMD/Application.cpp` (about 100 lines).

## Pick a form

| Want to time | Command |
|--------------|---------|
| One program | `time-cmd tool arg1 arg2` |
| Several commands, a pipe, a redirect of the **timed** command | one quoted argument: `time-cmd "make && make test"`, `time-cmd "tool > nul"`, `time-cmd 'tool > /dev/null'` |
| A Windows program path with spaces | `time-cmd "\"C:\Program Files\T\t.exe\" --run"` |
| ... and quoted arguments too | add one outer pair: `time-cmd "\"\"C:\Program Files\T\t.exe\" \"in file.txt\"\""` |
| From PowerShell with embedded quotes | `time-cmd --% "\"C:\Program Files\T\t.exe\" --run"` |
| The fixed overhead (shell start) | `time-cmd` with no command |
| Several runs, median (Linux) | loop, extract with `sed -n 's/^Execution time: \([0-9]*\) ms$/\1/p'`, `sort -n` |

## Hard rules

1. **Arguments are joined with one space** and passed to C `system()`
   (`cmd.exe /c` on Windows, `/bin/sh -c` on Linux). Quotes removed by the
   caller's shell are **not** restored: `time-cmd ls "/tmp/a b"` runs
   `ls /tmp/a b`. Quote the whole command line as one argument instead.
2. **Unquoted operators belong to the caller's shell.**
   `time-cmd make > log` puts the build output **and** the
   `Execution time:` line in `log`; `time-cmd a && b` times only `a`.
3. **cmd.exe /c strips the first and last quote** when the line starts
   with a quote and has more than two: wrap such a command in one more
   pair of `\"`.
4. **Options only as the first argument**: `--help`, `--usage`,
   `--license`, `--version` (value after `=` ignored, rest of the line
   ignored, exit `0`). Any other `--x` is run as a command.
5. **Output**: the command writes directly to the console (not
   captured); then `time-cmd` prints exactly one line on stdout,
   `Execution time: <N> ms` — always, also when the command failed or
   does not exist. Parse the last line matching `^Execution time: (\d+) ms$`.
6. **Exit code = the command's exit code**, so `time-cmd x && y`,
   `if errorlevel 1`, `if ! time-cmd x` behave as without it. Windows:
   via `cmd.exe`, full 32 bit, not found → `1`. Linux: like a shell,
   modulo 256, not found → `127`, killed by signal → `128 + S`.
   Exception: Linux builds up to **5.9.0 build 10** (the 5.9.0 release
   archives) exit `0` for every normal exit; with those, use
   `time-cmd 'cmd || touch failed'` and test the file.
7. **What is measured**: wall clock of `system()` — shell start + the
   command + the children it waits for. Not CPU time, not memory. Clock:
   `DateTime::timestampInMilliseconds()`, **local time, not monotonic**:
   a DST switch or clock change during the run corrupts the result (a
   backward jump wraps to a huge number).
8. **Overhead and resolution**: empty command ≈ 15–25 ms on Windows
   (`cmd.exe`), 1–3 ms on Linux; ms resolution (up to ~16 ms clock tick on
   some Windows systems). Below ~100 ms the number is mostly noise: loop
   inside the command. Warm up once, repeat ≥ 5, take the median,
   redirect heavy console output inside the quotes.
9. **Linux build needs the XYO `.so`s** (`xyo-system.so`, ...) on the
   library path, else exit `127`; the Windows `.static` build has no DLL
   dependencies, the normal one needs `xyo-system.dll` and the layers
   below.

## In scripts

```bat
rem Windows batch
time-cmd "fabricare clean && fabricare make"
if errorlevel 1 exit /b 1
```

```bash
# Linux: median of 5
for i in 1 2 3 4 5; do
	time-cmd 'make -B > /dev/null' | sed -n 's/^Execution time: \([0-9]*\) ms$/\1/p'
done | sort -n | sed -n 3p
```

```js
// fabricare script, time-cmd installed in the SDK bin
exitIf(Shell.system("time-cmd \"fabricare make\""));
```

## Working inside this repository

- One fabricare project, `time-cmd` (`make: exe`, source
  `source/XYO/TimeCMD`, depends on `xyo-system`). `fabricare make`,
  `fabricare install`, `fabricare release`; there is no test project:
  try changes with `output/bin/time-cmd` on Windows and on Linux (WSL,
  see the `fabricare` skill), checking `exit 3`, a missing command, an
  empty command, quoted paths with spaces, and `--version`.
- `Application::main`: options from `cmdS[1]`, join, time `system()`,
  print, return. Keep the call qualified as `Shell::system(cmdLine)`:
  `XYO::System::Shell` is not pulled in by `using namespace XYO::System`,
  so a bare `system(...)` compiles to C `::system`, whose Linux wait
  status makes the tool always exit `0`.
- `XYO_TIMECMD_LIBRARY` leaves out `main` (`XYO_APPLICATION_MAIN`).
  `Version.rh` is generated from `Version.Template.rh` and `version.json`.
- Keep `docs/` and this skill in step with `Application.cpp` when the
  behavior changes (the outputs in the docs are real tool output).
- Licensing follows REUSE: `source/`, `docs/` and `README.md` are MIT
  (source files also carry SPDX headers); config, `fabricare.json`,
  `version.json` and `.claude/` are Unlicense. Every new top level file or
  folder needs a `Files:` entry in `.reuse/dep5` (check with
  `python -m reuse lint`).
