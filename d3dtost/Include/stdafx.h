#include "MyWindows.h"

#include "nmutil/OpenGlExtensionManager.h"

#include "TextureUnitManager_inl.h"
#include "ImageDecoder_inl.h"
#include "TextureManager.h"
#include "ImageLoader.h"






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


#include "MyGL.h"


#include "my/gl/shapes/Nothing_inl.h"
#include "my/gl/textures/TextureUnitWrapper_inl.h"
#include "my/gl/textures/TextureUnit_inl.h"
#include "my/gl/textures/TextureUnitManager_inl.h"
#include <my/gl/shapes/ShapeCompositionFactory.h>


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

#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>

namespace d3dtost {
	extern LPWSTR ConvertErrorMessage (char const* const msg);
}

#define M_E        static_cast<float>(2.71828182845904523536)
#define M_LOG2E    static_cast<float>(1.44269504088896340736)
#define M_LOG10E   static_cast<float>(0.434294481903251827651)
#define M_LN2      static_cast<float>(0.693147180559945309417)
#define M_LN10     static_cast<float>(2.30258509299404568402)
#define M_PI       static_cast<float>(3.14159265358979323846)
#define M_PI_2     static_cast<float>(1.57079632679489661923)
#define M_PI_4     static_cast<float>(0.785398163397448309616)
#define M_1_PI     static_cast<float>(0.318309886183790671538)
#define M_2_PI     static_cast<float>(0.636619772367581343076)
#define M_2_SQRTPI static_cast<float>(1.12837916709551257390)
#define M_SQRT2    static_cast<float>(1.41421356237309504880)
#define M_SQRT1_2  static_cast<float>(0.707106781186547524401)
