#ifndef TIME_H
#define TIME_H

class Time
{
public:
	inline static void SetTimeElapsed(float delta){ m_fFrameTime = delta; m_fTimeSinceStartup += delta; }
	inline static float GetTimeElapsed(){ return m_fFrameTime; }
	inline static float GetTimeSinceStartup(){ return m_fTimeSinceStartup; }

private:
	static float m_fTimeSinceStartup;
	static float m_fFrameTime;
};


#endif