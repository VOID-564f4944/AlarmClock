// header files
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>

class Time // handles the time from user
{
private:
	int m_hour = 0;
	int m_minute = 0;

public:
	void setTime() // takes valid time from user
	{
		char colon;
		bool valid = false;
		do
		{
			std::cout << "Enter the time in this format hh:mm > ";
			if (std::cin >> m_hour >> colon >> m_minute && colon == ':')
			{
				if (m_hour < 0 || m_hour > 24 || m_minute < 0 || m_minute >= 60) {
					std::cout << "Please enter proper time.";
				}
				else
				{
					if (m_hour == 24)
					{
						m_hour = 0;
					}
					valid = true;
				}
			}
			else {
				std::cout << "Invalid format. Use hh:mm\n";
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		} while (!valid);
	}

	std::chrono::system_clock::time_point toTimePoint() const // hh:mm time into time_point, aligning it with the user's time zone
	{
		const std::chrono::time_zone* timeZone = std::chrono::current_zone();
		std::chrono::local_time<std::chrono::system_clock::duration> localNow = timeZone->to_local(std::chrono::system_clock::now());
		std::chrono::local_days today = std::chrono::floor<std::chrono::days>(localNow);
		std::chrono::local_time<std::chrono::minutes> localAlarm = today + std::chrono::hours(m_hour) + std::chrono::minutes(m_minute);
		std::chrono::sys_seconds target = timeZone->to_sys(localAlarm);

		if (target <= std::chrono::system_clock::now()) {
			target += std::chrono::days(1);
		}
		return target;
	}
};

void remainingTimeLogger(const std::chrono::system_clock::time_point& alarmTime, std::atomic<bool>& stop) // logs remaining time to console every one minute, terminates after alarm sets off
{
	while (!stop)
	{
		std::chrono::minutes diff = std::chrono::duration_cast<std::chrono::minutes>(alarmTime - std::chrono::system_clock::now());
		if (diff < std::chrono::minutes(0)) {
			break;
		}
		std::cout << diff << " remaining\n";
		std::this_thread::sleep_for(std::chrono::minutes(1));
	}
}

void alarm(const std::chrono::system_clock::time_point& alarmTime, std::atomic<bool>& stop) // sets off the alarm after waiting until alarm time
{
	std::this_thread::sleep_until(alarmTime);
	stop = true;
	std::cout << "\a Waky Waky!" << std::endl;
}

int main()
{
	Time userTime;
	userTime.setTime();
	std::chrono::system_clock::time_point alarmTime = userTime.toTimePoint();
	std::atomic<bool> stop = false;

	// puts alarm and remainingTimeLogger in two seperate threads to run parallely
	std::thread t1(remainingTimeLogger, std::ref(alarmTime), std::ref(stop));
	std::thread t2(alarm, std::ref(alarmTime), std::ref(stop));
	t1.join();
	t2.join();
	return 0;
}