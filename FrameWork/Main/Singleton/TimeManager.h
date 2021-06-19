#pragma once

class TimeManager : public SingletonBase<TimeManager>
{
private:
	bool _isHardware;
	__int64 _curTime;
	__int64 _lastTime;

	float _FPStimeElapsed;
	float _timeScale;
	DWORD _FPSFrameCount;

	DWORD _frameRate;
	float _timeElapsed;
	float _worldTime;
public:
	TimeManager();
	~TimeManager();
	void SetFrame(UINT lock = 60);
	float GetElapsedTime() const { return _timeElapsed; }
	float GetWorldTime() const { return _worldTime; }
	unsigned long GetFrameRate() { return _frameRate; }

	void Render();
};