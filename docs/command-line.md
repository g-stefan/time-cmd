# Command line

```
time-cmd [command line]
time-cmd --help | --usage | --license | --version
```

## Options

| Option | Effect |
|--------|--------|
| `--help`, `--usage` | print usage, exit `0` |
| `--license` | print the MIT license, exit `0` |
| `--version` | print `version X.Y.Z build N [date time]`, exit `0` |

Options are recognized **only as the first argument**. Everything after an
option is ignored (`time-cmd --version extra` prints the version). A value
after `=` is ignored too (`--help=1` is `--help`).

An unknown option is **not** an error: it is taken as the first word of the
command line and run. `time-cmd --foo` runs `--foo` through the shell,
which reports that the command does not exist.

To time a program whose name starts with `--`, put something in front of
it, for example `time-cmd ./--tool` (Linux) or `time-cmd .\--tool`
(Windows).

## How the command line is built

Every argument after `time-cmd` is joined with **one space** into a single
string, which is passed to the C `system()` function:

```
time-cmd fabricare make --debug   ->   system("fabricare make --debug")
```

`system()` runs it through the system shell: `cmd.exe /c` on Windows
(`%COMSPEC%`), `/bin/sh -c` on Linux. So the command can be a built-in of
that shell (`dir`, `echo`, `exit 3`), a batch file or script, a pipe, or
several commands joined with `&&`, `||`, `;` (Linux) or `&` (Windows).

Two things follow from the join:

1. **Quotes are lost.** The shell you type in (or the C runtime on
   Windows) removes the quotes before `time-cmd` sees its arguments, and
   the join does not put them back. `time-cmd ls "/tmp/a b"` runs
   `ls /tmp/a b`, two arguments.
2. **Spacing is normalized.** Several spaces between arguments become one.

## Quoting and shell operators

Pass the whole command line as **one quoted argument** whenever it
contains quotes, spaces inside a name, or shell operators:

| You want to time | Type (Windows `cmd.exe`) | Type (Linux `sh` / `bash`) |
|------------------|--------------------------|----------------------------|
| `make && make test` | `time-cmd "make && make test"` | `time-cmd 'make && make test'` |
| `build > build.log` (only the build's output in the file) | `time-cmd "build > build.log"` | `time-cmd 'build > build.log'` |
| `ls "/tmp/a b"` | — | `time-cmd 'ls "/tmp/a b"'` |
| `"C:\Program Files\Tool\tool.exe" --run` | `time-cmd "\"C:\Program Files\Tool\tool.exe\" --run"` | — |
| `"C:\Program Files\Tool\tool.exe" "in file.txt"` | `time-cmd "\"\"C:\Program Files\Tool\tool.exe\" \"in file.txt\"\""` | — |

Without the quotes the operators belong to **your** shell, not to the timed
command:

| Typed | What happens |
|-------|--------------|
| `time-cmd make > build.log` | your shell redirects `time-cmd`: the build output **and** the `Execution time:` line go to `build.log` |
| `time-cmd make && make test` | only `make` is timed; `make test` runs after `time-cmd`, if `time-cmd` exited `0` |
| `time-cmd make \| tee log` | only `make` is timed; `tee` receives the build output and the `Execution time:` line |

Sometimes this is what you want: `time-cmd make > build.log` keeps the
time in the log.

### Windows: `cmd.exe /c` and quotes

On Windows, inside a quoted argument write `\"` for a quote. `cmd.exe /c`
then has its own rule: when the command line **starts with a quote** and
contains **more than two** quotes, it removes the first and the last quote
character. That breaks a quoted program path followed by quoted
arguments:

```
time-cmd "\"C:\Program Files\Tool\tool.exe\" \"in file.txt\""
'C:\Program' is not recognized as an internal or external command, ...
```

Wrap the whole command in one more pair of quotes, which `cmd.exe` then
removes:

```
time-cmd "\"\"C:\Program Files\Tool\tool.exe\" \"in file.txt\"\""
```

With exactly two quotes (only the program path quoted) no extra pair is
needed.

### PowerShell

PowerShell 5.1 changes embedded quotes when it starts a native program.
Use the stop-parsing token `--%`, after which the rest of the line is
passed as typed, with the `cmd.exe` rules above:

```powershell
time-cmd --% "\"C:\Program Files\Tool\tool.exe\" --run"
```

Simple commands and operators inside one quoted string work without it:

```powershell
time-cmd "fabricare clean && fabricare make"
```

## Output

1. Everything the command writes, on the console, while it runs (standard
   output and standard error, not captured or changed).
2. Then, on standard output, one line:

```
Execution time: <N> ms
```

`<N>` is a whole number of milliseconds. The line is always printed, also
when the command failed or does not exist:

```
time-cmd nosuchcmd
'nosuchcmd' is not recognized as an internal or external command,
operable program or batch file.
Execution time: 24 ms
```

To read the number from a script, take the last line that starts with
`Execution time:`.

With no command (`time-cmd` alone) the shell runs an empty command and the
line shows the overhead of `time-cmd` plus the shell start; see
[Benchmarking](benchmarking.md#overhead).

## Exit codes

| Case | Windows | Linux |
|------|---------|-------|
| `--help`, `--usage`, `--license`, `--version` | `0` | `0` |
| command exits with code `N` | `N` (full 32 bit value, `exit 300` → `300`) | `N` modulo 256 (`exit 300` → `44`, as in any shell) |
| command not found | `1` (from `cmd.exe`) | `127` (from `sh`) |
| shell killed by signal `S` | — | `128 + S` (e.g. `143` for `SIGTERM`) |
| shell could not be started | `-1` | `255` |
| no command | `0` | `0` |

`time-cmd` is transparent: `time-cmd make && next`, `if errorlevel 1`
(Windows) and `if ! time-cmd make; then ...` (Linux) behave as without
`time-cmd`. The command is run with `XYO::System::Shell::system`, which
on Linux turns the wait status of `system()` into an exit code the way a
shell does.

`time-cmd` 5.9.0 build 10 and earlier (the 5.9.0 release archives) used
the C `system()` directly: on Linux they exit `0` for every normal exit
of the command, including failures, and with the bare signal number when
the shell is killed. Windows is not affected.

## Examples

```
time-cmd fabricare make
time-cmd "fabricare clean && fabricare make"
time-cmd "7z a -mx9 archive.7z data\* > nul"
time-cmd python bench.py --size 1000000
time-cmd 'find / -name "*.so" 2>/dev/null | wc -l'
time-cmd ctest -j8
```
