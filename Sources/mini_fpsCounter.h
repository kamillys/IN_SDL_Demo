#pragma once

namespace mini
{
	class FPSCounter
	{
	public:
		FPSCounter(float freq = 0.05f) : m_count(0), m_time(0), m_last(0), m_freq(freq) { }
		
		void NextFrame(float dt)
		{
			m_count++;
			m_time += dt;
			while (m_time > m_freq)
			{
				m_time -=m_freq;
				m_last = m_count;
				m_count = 0;
			}
		}
		float getCount() { return m_last/m_freq; }

	private:
		float m_time;
		float m_freq;
		int m_count;
		int m_last;
	};
}