//
// Time Cmd
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "time-cmd-version.hpp"

namespace TimeCmd {
	namespace Version {

		static const char *version_ = "1.2.0";
		static const char *build_ = "4";
		static const char *versionWithBuild_ = "1.2.0.4";
		static const char *datetime_ = "2020-09-21 17:38:56";

		const char *version() {
			return version_;
		};
		const char *build() {
			return build_;
		};
		const char *versionWithBuild() {
			return versionWithBuild_;
		};
		const char *datetime() {
			return datetime_;
		};

	};
};

