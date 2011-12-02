header = '// OpenGlExtensions.h
// OpenGL extension enums and functions.
//
// MMOG Project
// Nikos Mouchtaris, 9 October 2011

// Note: file is generated. Do not hope for persistent changes
// if you change things in here.

#ifndef ANKH_IMAGES_OPEN_GL_EXTENSIONS_H
#define ANKH_IMAGES_OPEN_GL_EXTENSIONS_H

'

footer = '
#endif	// Do not add stuff beyond this point.
'

typedefs = []

typedefs << ['unsigned int'		, 'GLenum'		]
typedefs << ['unsigned char'	, 'GLboolean'	]
typedefs << ['unsigned int'		, 'GLbitfield'	]
typedefs << ['signed char'		, 'GLbyte'		]
typedefs << ['char'				, 'GLchar'		]
typedefs << ['short'			, 'GLshort'		]
typedefs << ['int'				, 'GLint'		]
typedefs << ['int'				, 'GLsizei'		]
typedefs << ['unsigned char'	, 'GLubyte'		]
typedefs << ['unsigned short'	, 'GLushort'	]
typedefs << ['unsigned int'		, 'GLuint'		]
typedefs << ['float'			, 'GLfloat'		]
typedefs << ['float'			, 'GLclampf'	]
typedefs << ['double'			, 'GLdouble'	]
typedefs << ['double'			, 'GLclampd'	]
typedefs << ['void'				, 'GLvoid'		]
typedefs << ['ptrdiff_t'		, 'GLintptr'	]
typedefs << ['ptrdiff_t'		, 'GLsizeiptr'	]
typedefs << ['struct __GLsync*'	, 'GLsync'		]

def enums_to_s (enums)
	enums.sort! { |a, b| a.last <=> b.last }
	maxname = 0
	for enum_pair in enums
		maxname = enum_pair.first.length if enum_pair.first.length > maxname
	end
	lines = []
	for enum_pair in enums
		lines << sprintf('#define %-*s %#06x', maxname, enum_pair.first, enum_pair.last)
	end
	lines.join("\n")
end

def typedefs_to_s (typedefs)
	maxname = 0
	for tdpair in typedefs
		maxname = tdpair.first.length if tdpair.first.length > maxname
	end
	
	lines = []
	for tdpair in typedefs
		lines << sprintf('typedef %-*s %s;', maxname, tdpair.first, tdpair.last)
	end
	lines.join("\n")
end

def parse_gl_ext (fin)
	enums = []
	fin.each_line { |line|
		line.chomp!
		case
			when line.empty?
			when md = /^\#define\s+([\w_]+)\s+([^\s]+)\s*$/.match(line)
				name = md[1]
				val = Integer(md[2])
				enums << [name, val]
		end
	}
	enums
end

File.open(ARGV[1], IO::CREAT | IO::TRUNC | IO::WRONLY) { |fout|
	File.open(ARGV.first, IO::RDONLY) { |fin|
		fout.puts header, enums_to_s(parse_gl_ext(fin)), typedefs_to_s(typedefs), footer
	}
}
