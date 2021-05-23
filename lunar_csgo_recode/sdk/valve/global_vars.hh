#pragma once

namespace sdk::valve {
	class c_global_vars {
	public:
		float m_realtime;
		int m_framecount;
		float m_abs_frametime;
		float m_abs_framestarttimestddev;
		float m_curtime;
		float m_frametime;
		int m_max_clients;
		int m_tickcount;
		float m_ipt;
		float m_interp;
	};
}