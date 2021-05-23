/* multi purpose hooking class written by swoopae */
/* includes the ability to vmt and detour hook */

#pragma once

#include <Windows.h>
#include <memory>

namespace multihook {
	enum e_hooktype {
		HOOK_VMT = 0,
		HOOK_DETOUR
	};

	struct vmt_t {
		std::uintptr_t ** m_vmt_base , m_table_length , * original;
	};

	class c_hook {
	private:
		/* members */
		/* used for vmt */

		/* used for detour */
	};
}