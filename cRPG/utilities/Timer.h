#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
public:
	Timer();
	~Timer() = default;

	void Start();
	void Stop();
	void Pause();
	void Resume();

	const int64_t ElapsedMS();
	const int64_t ElapsedSec();

	const bool IsRunning() const { return m_bIsRunning; }
	const bool IsPaused() const { return m_bIsPaused; }


private:
	time_point<steady_clock> m_StartPoint, m_PausedPoint;

	bool m_bIsRunning, m_bIsPaused;
};