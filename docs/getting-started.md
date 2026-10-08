# Getting started

## 1. Get the executable

### From a release

Each release has these archives (`release/` after `fabricare release`, or
the GitHub releases page):

| Archive | Contents | Needs |
|---------|----------|-------|
| `xyo.time-cmd.vX.Y.Z.win64-msvc-2026.static.bin.zip` | `time-cmd.exe`, static build | nothing, copy it anywhere on the `PATH` |
| `xyo.time-cmd.vX.Y.Z.win64-msvc-2026.bin.zip` | `time-cmd.exe` | `xyo-platform.dll`, `xyo-managed-memory.dll`, `xyo-system.dll` (and the layers they use) next to it or on the `PATH` — the XYO SDK |
| `xyo.time-cmd.vX.Y.Z.ubuntu-24.04.bin.zip`, `ubuntu-26.04` | `time-cmd` | `xyo-system.so` and the layers below on the library path (`~/.fabricare/<platform>/bin` or `LD_LIBRARY_PATH`) |
| `xyo.time-cmd.vX.Y.Z.sha512.json` | SHA-512 of every archive | |

On a machine without the XYO SDK, use the static Windows build. The Linux
build started without its libraries fails with
`error while loading shared libraries: xyo-system.so`, exit code `127`.

### Build it

`time-cmd` is built with [fabricare](https://github.com/g-stefan/fabricare).
`xyo-platform`, `xyo-managed-memory`, `xyo-data-structures`,
`xyo-multithreading`, `xyo-encoding` and `xyo-system` must be installed to
the SDK first. From the repository root:

```bash
fabricare make       # build into output/  (output/bin/time-cmd[.exe])
fabricare install    # copy output/bin to ~/.fabricare/<platform>/bin
fabricare clean      # remove output/ and temp/
```

After `install`, `time-cmd` is in `~/.fabricare/<platform>/bin`, which is
on the `PATH` of a fabricare build (and usually of the developer's shell).
fabricare sets up the compiler environment (MSVC `vcvarsall.bat`) by
itself. `fabricare release` packs `output/` into the `release/` archives
listed above.

## 2. First runs

```
time-cmd --version
```

```
version 5.9.0 build 10 [2026-09-16 23:03:08]
```

Time a command — everything after `time-cmd` is the command line:

```
time-cmd ping -n 2 127.0.0.1
```

```
Pinging 127.0.0.1 with 32 bytes of data:
Reply from 127.0.0.1: bytes=32 time<1ms TTL=128
Reply from 127.0.0.1: bytes=32 time<1ms TTL=128
...
Execution time: 1045 ms
```

On Linux:

```
time-cmd sleep 0.2
```

```
Execution time: 205 ms
```

The output of the command appears as usual, while it runs. The last line
is printed by `time-cmd` after the command has ended.

## 3. Commands with shell operators

`time-cmd` passes the command line to the system shell, so pipes, `&&` and
redirections work — but **the shell you type in sees them first**. Quote
the whole command line to give them to the timed command:

```
time-cmd "make && make test"
time-cmd "build.cmd > build.log"
```

Without the quotes, `time-cmd make > build.log` sends **everything**,
including the `Execution time:` line, to `build.log`, and
`time-cmd make && make test` times only `make`. Details in
[Command line](command-line.md#quoting-and-shell-operators).

## 4. Measure the overhead first

`time-cmd` starts a shell (`cmd.exe /c` or `/bin/sh -c`) to run the
command; that start is part of the measured time:

```
time-cmd
```

```
Execution time: 20 ms        (Windows, cmd.exe)
Execution time: 2 ms         (Linux, /bin/sh)
```

Subtract it, or ignore it when the command takes seconds. More in
[Benchmarking](benchmarking.md).

## 5. Use it in a build script

From a fabricare script (`fabricare/*.js`), with `time-cmd` installed to
the SDK:

```js
exitIf(Shell.system("time-cmd \"fabricare make\""));
```

From a Windows batch file:

```bat
time-cmd "fabricare clean && fabricare make"
if errorlevel 1 exit /b 1
```

From a shell script on Linux:

```bash
time-cmd "make -j8" || exit 1
```

The exit code of `time-cmd` is the exit code of the command, on both
systems (on Linux since the build after 5.9.0 build 10; see
[Exit codes](command-line.md#exit-codes)).
