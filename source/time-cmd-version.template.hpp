//
// Time Cmd
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef TIME_CMD_VERSION_HPP
#define TIME_CMD_VERSION_HPP

#define TIME_CMD_VERSION_ABCD                $VERSION_ABCD
#define TIME_CMD_VERSION_STR                 "$VERSION_VERSION"
#define TIME_CMD_VERSION_STR_BUILD           "$VERSION_BUILD"
#define TIME_CMD_VERSION_STR_DATETIME        "$VERSION_DATETIME"

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
