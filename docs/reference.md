# Reference

For people working on `time-cmd` itself. Using the tool is covered in
[Command line](command-line.md).

## fabricare project

```json
{
	"name" : "time-cmd",
	"make" : "exe",
	"SPDX-License-Identifier": "MIT",
	"sourcePath" : "XYO/TimeCMD",
	"dependency" : [ "xyo-system" ]
}
```

One project, an executable built from every source in
`source/XYO/TimeCMD/`. `xyo-system` brings in `xyo-encoding`,
`xyo-multithreading`, `xyo-data-structures`, `xyo-managed-memory` and
`xyo-platform`. The version lives in `version.json` under the key
`time-cmd`; `Version.rh` is generated from `Version.Template.rh` by
xyo-version (`fabricare version` bumps the build number).

## Headers

| Header | Contents |
|--------|----------|
| `<XYO/TimeCMD/Dependency.hpp>` | includes `<XYO/System.hpp>`; `namespace XYO::TimeCMD { using namespace XYO::System; }` |
| `<XYO/TimeCMD/Application.hpp>` | `XYO::TimeCMD::Application` |
| `<XYO/TimeCMD/Copyright.hpp>`, `License.hpp`, `Version.hpp` | tool metadata |
| `Application.rh`, `Copyright.rh`, `Version.rh` | macros shared by the C++ code and the Windows resource script |

## Macros

| Macro | Meaning |
|-------|---------|
| `XYO_TIMECMD_LIBRARY` | if defined, `Application.cpp` does not define `main` (`XYO_APPLICATION_MAIN` is skipped), so the class can be compiled into another program; no fabricare project sets it |
| `XYO_TIMECMD_NO_VERSION` | `Version.rh` gives `0.0.0` / build `0` instead of the generated version |
| `XYO_TIMECMD_VERSION_ABCD`, `_STR`, `_STR_BUILD`, `_STR_DATETIME`, `_STR_WITH_BUILD` | version, from `version.json` |
| `XYO_TIMECMD_COPYRIGHT`, `_PUBLISHER`, `_COMPANY`, `_CONTACT` | copyright strings |

## class XYO::TimeCMD::Application

```cpp
class Application : public virtual IApplication {
		XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);

	public:
		inline Application(){};

		void showUsage();
		void showLicense();
		void showVersion();

		int main(int cmdN, char *cmdS[]);

		static void initMemory();
};

XYO_APPLICATION_MAIN(XYO::TimeCMD::Application);   // unless XYO_TIMECMD_LIBRARY
```

| Member | Does |
|--------|------|
| `showUsage()` | prints the title, `showVersion()`, the copyright and the option list |
| `showLicense()` | prints `License::license()` |
| `showVersion()` | prints `version <version> build <build> [<datetime>]` |
| `initMemory()` | initializes the managed memory of `String` and `TDynamicArray<String>` before `main` (called by `XYO_APPLICATION_MAIN` through `TIfHasInitMemory`) |
| `main(cmdN, cmdS)` | the tool; returns the process exit code |

`main`, step by step:

1. If `cmdS[1]` starts with `--`: split it at the first `=`; `help` /
   `usage` → `showUsage()`, `license` → `showLicense()`, `version` →
   `showVersion()`, then return `0`. Any other name falls through.
2. Join `cmdS[1]` ... `cmdS[cmdN - 1]` with single spaces into a `String`.
3. `DateTime::timestampInMilliseconds()`, `Shell::system(cmdLine)`,
   `DateTime::timestampInMilliseconds()`.
4. `printf("Execution time: %zu ms\n", end - begin)` (the format comes
   from `XYO_PLATFORM_FORMAT_SIZET`).
5. Return the value of `Shell::system()`.

`Shell::system` (`XYO::System::Shell`, from `xyo-system`) must be called
qualified: `Shell` is a nested namespace and is not brought in by
`using namespace XYO::System`, so a bare `system(cmdLine)` silently
resolves to the C library `::system` (the `String` converts to
`const char *`). On Windows both return the exit code of `cmd.exe`; on
Linux `::system` returns a wait status (`exit code × 256`), whose low 8
bits are `0` — the process would always exit `0`. `Shell::system`
converts it the way a shell does: the exit code, or `128 + signal`.
Versions up to 5.9.0 build 10 had the bare call.

## namespace XYO::TimeCMD::Version / Copyright / License

```cpp
const char *XYO::TimeCMD::Version::version();          // "5.9.0"
const char *XYO::TimeCMD::Version::build();            // "10"
const char *XYO::TimeCMD::Version::versionWithBuild(); // "5.9.0.10"
const char *XYO::TimeCMD::Version::datetime();         // "2026-09-16 23:03:08"

const char *XYO::TimeCMD::Copyright::copyright();
const char *XYO::TimeCMD::Copyright::publisher();
const char *XYO::TimeCMD::Copyright::company();
const char *XYO::TimeCMD::Copyright::contact();

std::string XYO::TimeCMD::License::license();      // MIT header + copyright + MIT text
std::string XYO::TimeCMD::License::shortLicense(); // copyright + short MIT notice
```

## Windows resources

`Application.rc` adds the icon (`Application.ico`), the version
information (`XYO_PLATFORM_VERSION_INFO`, file name `time-cmd.exe`,
description "Show execution time of command for benchmark purposes") and
`Application.manifest`, which sets the active code page to **UTF-8**: on
Windows 10 1903 and later the arguments and the command line passed to
`system()` are UTF-8, so non-ASCII file names survive.

## Command line summary

| Input | Result | Exit code |
|-------|--------|-----------|
| `--help`, `--usage` (first argument) | usage | `0` |
| `--license` | license | `0` |
| `--version` | `version X.Y.Z build N [date]` | `0` |
| anything else | run through the shell, then `Execution time: N ms` | the command's exit code (Linux: `128 + S` if the shell is killed by signal `S`) |
| nothing | empty command, `Execution time: N ms` (the overhead) | `0` |
