//
// Time Cmd
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef TIME_CMD_VERSION_HPP
#define TIME_CMD_VERSION_HPP

#define TIME_CMD_VERSION_ABCD                1,5,0,6
#define TIME_CMD_VERSION_STR                 "1.5.0"
#define TIME_CMD_VERSION_STR_BUILD           "6"
#define TIME_CMD_VERSION_STR_DATETIME        "2021-03-15 12:30:20"

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

