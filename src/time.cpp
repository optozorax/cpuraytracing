#include <windows.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <math.h>
#include "render/time.h"

namespace gui {
	extern bool Time::formatTime = false;

	float Time::getCurrentTime(void) {
		return GetTickCount()/1000.0f;
	}

	float Time::getTimePassed(float pastTime) {
		return getCurrentTime() - pastTime;
	}

	float Time::getApproxTime(float pastTime, float percent) {
		if (percent == 0)
			return 0;
		else
			return getTimePassed(pastTime)/percent;
	}

	float Time::getLeftTime(float pastTime, float percent) {
		if (percent == 0)
			return 0;
		else
			return getApproxTime(pastTime, percent) - getTimePassed(pastTime);
	}

	std::string Time::getTimeString(float time) {
		char s[25] = {};
		if (formatTime) {
			if (time > 86400)
				sprintf(s, "%2dd %2dh %2dm %2ds", int(time/86400), int(time/3600)%24, int(time/60)%60, int(time)%60);
			else
			if (time > 3600)
				sprintf(s, "    %2dh %2dm %2ds", int(time/3600)%24, int(time/60)%60, int(time)%60);
			else
			if (time > 60)
				sprintf(s, "        %2dm %2ds", int(time/60)%60, int(time)%60);
			else
				sprintf(s, "            %2ds", int(time)%60);
		} else {
			if (time > 86400)
				sprintf(s, "%2dd %2dh %2dm %2ds", int(time/86400), int(time/3600)%24, int(time/60)%60, int(time)%60);
			else
			if (time > 3600)
				sprintf(s, "%2dh %2dm %2ds", int(time/3600)%24, int(time/60)%60, int(time)%60);
			else
			if (time > 60)
				sprintf(s, "%2dm %2ds", int(time/60)%60, int(time)%60);
			else
				sprintf(s, "%2ds", int(time)%60);
		}
		return std::string(s);
	}

	void Time::writeTimeStatus(float pastTime, float percent) {
		std::cout.precision(2);
		std::cout 
			<< "\r" << std::setfill(' ') << std::setw(80) << ' ' << std::setw(1) << "\r"
			<< "Time passed: "
			<< Time::getTimeString(Time::getTimePassed(pastTime))
			<< "; Approximate time: "
			<< Time::getTimeString(Time::getApproxTime(pastTime, percent))
			<< "; Time left: "
			<< Time::getTimeString(Time::getLeftTime(pastTime, percent))
			<< ";\t"<< 100.0f * percent << '%';
	}

	void Time::writeTotalTime(float pastTime) {
		bool oldFormat = Time::formatTime;
		Time::formatTime = false;
		std::cout 
			<< "\r" << std::setfill(' ') << std::setw(80) << ' ' << std::setw(1) << "\r"
			<< "Total time: "
			<< Time::getTimeString(Time::getTimePassed(pastTime));
		Time::formatTime = oldFormat;
	}
}