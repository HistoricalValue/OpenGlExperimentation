md dist
md dist\bin
md dist\meshes
md dist\shaders
md dist\textures

copy Debug\Codeshare_Utilities.dll		dist\bin
copy Debug\ImageLoader.dll				dist\bin
copy Debug\MyUtil.dll					dist\bin
copy Debug\nmDevIL.dll					dist\bin
copy Debug\Surfaces.dll					dist\bin
copy Debug\TextureManager.dll			dist\bin
copy Debug\UtilAndDefensiveHeapD.dll	dist\bin
copy Debug\d3dtost.exe					dist\bin

copy meshes\moon_valley_bumps_1700_with_ao9_aotime455218ms.msh		dist\meshes
copy meshes\moon_valley_bumps_1700_with_ao17_aotime891116ms.msh		dist\meshes

copy shaders\fragment.c		dist\shaders
copy shaders\vertex2.txt	dist\shaders
