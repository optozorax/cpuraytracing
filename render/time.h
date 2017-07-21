#ifndef GUI_TIME_H
#define GUI_TIME_H

#include <string>

namespace gui {
	class Time {
	public:
		static float getCurrentTime(void);
		static float getTimePassed(float pastTime);
		static float getApproxTime(float pastTime, float percent);
		static float getLeftTime(float pastTime, float percent);
		static std::string getTimeString(float time);
		static void writeTimeStatus(float pastTime, float percent);

		static bool formatTime;
	};
}

#endif
