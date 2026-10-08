# Time Cmd

Show execution time of command for benchmark purposes
- `time-cmd <command line>` runs the command through the system shell
(`cmd.exe` / `/bin/sh`), shows its output as usual, then prints
`Execution time: <N> ms`.
- Same tool and same output on Windows and Linux; the exit code of the
command is passed through.
- Quote the whole command line to time pipes, `&&` or redirections:
`time-cmd "make && make test"`.

Built on `xyo-system`.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - download or build, first runs, use in scripts
- [Command line](docs/command-line.md) - options, quoting, shell operators, output, exit codes
- [Benchmarking](docs/benchmarking.md) - what is measured, overhead, reliable numbers, known issues
- [Reference](docs/reference.md) - source layout, `Application` class, macros, metadata

A Claude Code skill for this tool is in
[.claude/skills/time-cmd](.claude/skills/time-cmd/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
