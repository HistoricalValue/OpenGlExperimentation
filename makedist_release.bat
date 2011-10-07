md dist & md dist\bin & md dist\meshes & md dist\shaders & md dist\textures &\
copy Release\Codeshare_Utilities.dll dist\bin &
copy Release\ImageLoader.dll dist\bin &
copy Release\MyUtil.dll dist\bin &
copy Release\nmDevIL.dll dist\bin &
copy Release\NURBS.dll dist\bin &
copy Release\TextureManager.dll dist\bin &
copy Release\UtilAndDefensiveHeapD.dll dist\bin &
copy Release\d3dtost.exe dist\bin &

copy meshes\moon_valley_bumps_12610_with_ao9_aotime345431ms.msh dist\meshes &

copy shaders\fragment.c dist\shaders &
copy shaders\vertex2.txt dist\shaders &
pause
