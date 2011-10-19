#include "stdafx.h"
#include "TheCursed.h"

#define __NEW_ERROR(MSG) \
		(new ::codeshare::utilities::Error(_T(__FILE__), psafecast<size_t>(__LINE__), codeshare::utilities::String(_T(MSG))))

#define __POST_PRIMARY(MSG) \
	( my::global::errorHolder::Get().PostPrimary(__NEW_ERROR(MSG)) )

#define __POST_PRIMARY_UNLESS(COND, MSG) \
		if (!(COND)) __POST_PRIMARY(MSG); else{}

#define __POST_PRIMARY_IF(COND, MSG) \
		__POST_PRIMARY_UNLESS(!(COND), MSG)

#define __IS_ERROR_POSTED \
		(my::global::errorHolder::Get().IsErrorPosted())

namespace my {

	namespace { namespace _ {
		static LPDIRECT3D9 CreateMyDirect3D9Object (void) {
			LPDIRECT3D9 const result(Direct3DCreate9(D3D_SDK_VERSION));
			__POST_PRIMARY_IF(result == NULL, "could not create a Direct3D9 object")
			PASSERT(result);
			return result;
		}

		static P_INLINE LPDIRECT3D9 GetMyDirect3D9Object (void) {
			return __IS_ERROR_POSTED? NULL : CreateMyDirect3D9Object();
		}
	} } // namespace <anonymous>::_


	Direct3D9::Direct3D9 (void):
		direct3d9(_::GetMyDirect3D9Object())
	{ }


	Direct3D9::~Direct3D9 (void) {
		if (direct3d9)
			direct3d9->Release();
	}


	bool Direct3D9::IsValid (void) const {
		return direct3d9 != NULL;
	}

} // namespace my
