// Time CMD
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/TimeCMD/Application.hpp>
#include <XYO/TimeCMD/Copyright.hpp>
#include <XYO/TimeCMD/License.hpp>
#include <XYO/TimeCMD/Version.hpp>

namespace XYO::TimeCMD {

	void Application::showUsage() {
		printf("time-cmd - Show execution time of command for benchmark purposes\n");
		showVersion();
		printf("%s\n\n", TimeCMD::Copyright::copyright());

		printf("%s",
		       "options:\n"
		       "    --help              this info\n"
		       "    --usage             this info\n"
		       "    --license           show license\n"
		       "    --version           show version\n"
		       "example:\n"
		       "    time-cmd [command line]\n");
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", TimeCMD::License::license().c_str());
	};

	void Application::showVersion() {
		printf("version %s build %s [%s]\n", TimeCMD::Version::version(), TimeCMD::Version::build(), TimeCMD::Version::datetime());
	};

	void Application::initMemory() {
		String::initMemory();
		TDynamicArray<String>::initMemory();
	};

	int Application::main(int cmdN, char *cmdS[]) {
		int i;
		String opt;
		size_t optIndex;
		String optValue;

		String cmdLine;
		int retV;

		if (cmdN > 1) {
			if (StringCore::beginWith(cmdS[1], "--")) {
				opt = &cmdS[1][2];
				optValue = "";
				if (opt.indexOf("=", 0, optIndex)) {
					optValue = opt.substring(optIndex + 1);
					opt = opt.substring(0, optIndex);
				};
				if (opt == "help") {
					showUsage();
					return 0;
				};
				if (opt == "usage") {
					showUsage();
					return 0;
				};
				if (opt == "license") {
					showLicense();
					return 0;
				};
				if (opt == "version") {
					showVersion();
					return 0;
				};
			};
		};

		for (i = 1; i < cmdN; ++i) {
			cmdLine << cmdS[i];
			if (i + 1 < cmdN) {
				cmdLine << " ";
			};
		};

		uint64_t beginTimestampInMilliseconds;
		uint64_t endTimestampInMilliseconds;
		uint64_t intervalTimestampInMilliseconds;

		beginTimestampInMilliseconds = DateTime::timestampInMilliseconds();
		retV = system(cmdLine);
		endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
		intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
		printf("Execution time: " XYO_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
		return retV;
	};
};

#ifndef XYO_TIMECMD_LIBRARY
XYO_APPLICATION_MAIN(XYO::TimeCMD::Application);
#endif
