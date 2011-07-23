
	find . \(						\
		-name Debug					\
		-o -name Release			\
		-o -name ipch				\
		-o -name '*.sdf'			\
		-o -name '*.asm'			\
		-o -name '*.obj'			\
		-o -name 'log.txt'			\
		-o -name 'dallocstats.txt'	\
		-o -name '*.pdb'			\
		-o -path '*DevIL/DevIL/lib/debug'	\
		-o -path '*DevIL/DevIL/lib/vc9'		\
	\) -prune -print0 | xargs -0 rm -r -f -v

