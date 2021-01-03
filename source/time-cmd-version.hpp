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

#define TIME_CMD_VERSION_ABCD                1,4,0,5
#define TIME_CMD_VERSION_STR                 "1.4.0"
#define TIME_CMD_VERSION_STR_BUILD           "5"
#define TIME_CMD_VERSION_STR_DATETIME        "2021-01-03 02:30:47"

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

