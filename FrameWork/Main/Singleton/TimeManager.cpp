#include "stdafx.h"
#include "TimeManager.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

TimeManager::TimeManager()
{
	__int64 _periodFrequency;
	if (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_periodFrequency)))
	{
		_isHardware = true;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_lastTime));
		_timeScale = 1.0f / _periodFrequency;
	}
	else
	{
		_isHardware = false;
		_lastTime = timeGetTime();
		_timeScale = 0.001f;
	}

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPStimeElapsed = 0.0f;
	_worldTime = 0;
}

TimeManager::~TimeManager()
{
}

void TimeManager::SetFrame(UINT lock)
{
	if (_isHardware)
	{
		QueryPerformanceCounter((reinterpret_cast<LARGE_INTEGER*>(&_curTime)));
	}
	else
	{
		_curTime = timeGetTime();
	}

	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	if (lock > 0.0f)
	{
		while (_timeElapsed < (1.0f / lock))
		{
			if (_isHardware) QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_curTime));
			else _curTime = timeGetTime();

			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;
	_FPSFrameCount++;
	_FPStimeElapsed += _timeElapsed;
	_worldTime += _timeElapsed;

	if (_FPStimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPStimeElapsed = 0.0f;
	}
}

void TimeManager::Render()
{
	char str[256];
	wsprintfA(str, "FPS: %d", _frameRate);
	ImGui::Text(str);

	sprintf_s(str, "WorldTime: %f", _worldTime);
	ImGui::Text(str);

	sprintf_s(str, "SecondPerFrameTime: %f", _timeElapsed);
	ImGui::Text(str);
}