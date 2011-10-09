

#include "MyWindows.h"

#include "nmutil/OpenGlExtensionManager.h"

#include "TextureUnitManager_inl.h"
#include "ImageDecoder_inl.h"
#include "TextureManager.h"
#include "ImageLoader.h"

#pragma warning( push, 0)
#include <usystem.h>
#include <ubinaryio.h>
#include <usystem.h>
#include <uinit.h>
#pragma warning( pop )



// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "Setup.h"


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>


#include <d3d9.h>


#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>

#include "my/gl/shapes/Nothing_inl.h"
#include "my/gl/textures/TextureUnitWrapper_inl.h"
#include "my/gl/textures/TextureUnit_inl.h"
#include "my/gl/textures/TextureUnitManager_inl.h"
#include <my/gl/shapes/ShapeCompositionFactory_inl.h>
#include <my/gl/shapes/NShapesComposition_inl.h>
#include <my/gl/shapes/Point.h>
#include <my/gl/math/Transformations.h>
#include <my/gl/math/Vector4_inl.h>


#include "my/gl/mygl.h"
#include "glt.h"
#include "MyUtil.h"
#include "MyLogger.h"
#include "MyWinUtils.h"
#include "MyList.h"
#include "MyOpenGLUtils.h"
#include "MyGlobalInstances.h"


#include "MyWindow.h"
#include "MyGlobalInstances.h"
#include "MyDirect3D9.h"
#include "MyOpenGL.h"
#include "MyConsole.h"
#include "MyLocalGlobalInstances.h"


#include "resource.h"



#include "PPointerUtilities.h"
#include "PCString.h"
#include "PSingleAllocationBuffer.h"
#include "PUtilitiesDSO.h"
#include "Pnewdelete.h"
#include "PMemory_inl.h"
#include "PFunctors_inl.h"
#include "PProtectedClass_inl.h"
#include "PSystem.h"
#include "PGlobalSingleAllocationBuffer.h"
#include "PPlaceholder_inl.h"

namespace d3dtost {
	extern LPWSTR ConvertErrorMessage (char const* const msg);
}


#include <Trig.h>
#include <MathTypes.h>
