//
// Time Cmd
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef TIME_CMD_VERSION_HPP
#define TIME_CMD_VERSION_HPP

#define TIME_CMD_VERSION_ABCD                1,13,0,28
#define TIME_CMD_VERSION_STR                 "1.13.0"
#define TIME_CMD_VERSION_STR_BUILD           "28"
#define TIME_CMD_VERSION_STR_DATETIME        "2022-01-09 01:13:04"

#ifndef XYO_RC

namespace TimeCmd {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif

