//
// Time Cmd
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xyo.hpp"
#include "time-cmd.hpp"
#include "time-cmd-copyright.hpp"
#include "time-cmd-license.hpp"
#ifndef TIME_CMD_NO_VERSION
#include "time-cmd-version.hpp"
#endif

namespace TimeCmd {

	using namespace XYO;

	void Application::showUsage() {
		printf("time-cmd - Show execution time of command for benchmark purposes\n");
		showVersion();
		printf("%s\n\n", TimeCmd::Copyright::fullCopyright());

		printf("%s",
			"options:\n"
			"    --usage             this info\n"
			"    --license           show license\n"
			"    --version           show version\n"
			"example:\n"
			"    time-cmd [command line]\n"
		);
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", TimeCmd::License::content());
	};

	void Application::showVersion() {
#ifndef TIME_CMD_NO_VERSION
		printf("version %s build %s [%s]\n", TimeCmd::Version::version(), TimeCmd::Version::build(), TimeCmd::Version::datetime());
#endif
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
				if(String::indexOf(opt, "=", 0, optIndex)) {
					optValue = String::substring(opt, optIndex + 1);
					opt = String::substring(opt, 0, optIndex);
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

		for(i = 1; i < cmdN; ++i) {
			cmdLine << cmdS[i];
			if(i + 1 < cmdN) {
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

#ifndef TIME_CMD_LIBRARY
XYO_APPLICATION_MAIN_STD(TimeCmd::Application);
#endif
