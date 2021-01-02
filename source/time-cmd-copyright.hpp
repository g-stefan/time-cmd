//
// Time Cmd
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef TIME_CMD_COPYRIGHT_HPP
#define TIME_CMD_COPYRIGHT_HPP

#define TIME_CMD_COPYRIGHT            "Copyright (c) Grigore Stefan"
#define TIME_CMD_PUBLISHER            "Grigore Stefan"
#define TIME_CMD_COMPANY              TIME_CMD_PUBLISHER
#define TIME_CMD_CONTACT              "g_stefan@yahoo.com"
#define TIME_CMD_FULL_COPYRIGHT       TIME_CMD_COPYRIGHT " <" TIME_CMD_CONTACT ">"

#ifndef XYO_RC

namespace TimeCmd {
	namespace Copyright {
		const char *copyright();
		const char *publisher();
		const char *company();
		const char *contact();
		const char *fullCopyright();
	};
};

#endif
#endif
