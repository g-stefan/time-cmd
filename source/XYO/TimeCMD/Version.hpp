// Time CMD
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_TIMECMD_VERSION_HPP
#define XYO_TIMECMD_VERSION_HPP

#ifndef XYO_TIMECMD_DEPENDENCY_HPP
#	include <XYO/TimeCMD/Dependency.hpp>
#endif

namespace XYO::TimeCMD::Version {

	const char *version();
	const char *build();
	const char *versionWithBuild();
	const char *datetime();

};

#endif
