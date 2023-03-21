// Time CMD
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/TimeCMD/Copyright.hpp>
#include <XYO/TimeCMD/Copyright.rh>

namespace XYO::TimeCMD::Copyright {

	static const char *copyright_ = XYO_TIMECMD_COPYRIGHT;
	static const char *publisher_ = XYO_TIMECMD_PUBLISHER;
	static const char *company_ = XYO_TIMECMD_COMPANY;
	static const char *contact_ = XYO_TIMECMD_CONTACT;

	std::string copyright() {
		return copyright_;
	};

	std::string publisher() {
		return publisher_;
	};

	std::string company() {
		return company_;
	};

	std::string contact() {
		return contact_;
	};

};
