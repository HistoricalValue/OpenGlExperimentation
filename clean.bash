
	find . \(						\
		   -iname Debug				\
		-o -iname Release			\
		-o -name ipch				\
		-o -name '*.sdf'			\
		-o -name '*.asm'			\
		-o -name '*.obj'			\
		-o -name 'log.txt'			\
		-o -name 'dallocstats.txt'	\
		-o -name '*.ncb'			\
		-o -name '*.pdb'			\
		-o -name '*.vcproj.*.user'	\
		-o -name '*.aps'			\
		-o -path '*DevIL/DevIL/lib/debug'	\
		-o -path '*DevIL/DevIL/lib/vc9'		\
	\) -prune -print0 | xargs -0 rm -r -f -v

