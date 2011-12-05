require 'pp'

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

class IterableConcatenation
	def initialize (*iterables)
		@iterables = iterables
	end
	
	def each (&b)
		iterables = @iterables.dup
		until iterables.empty?
			iterable = iterables.shift
			iterable.each(&b)
		end
	end
end

def enums_to_s (enums)
	enums[:num].sort! { |a, b|
		if (cmp = a.last <=> b.last) != 0
			cmp
		else
			a.first <=> b.first
		end
	}
	enums[:str].sort! { |a, b| a.first <=> a.first }

	maxname = 0
	for enum_pair in IterableConcatenation.new(enums[:num], enums[:str])
		maxname = enum_pair.first.length if enum_pair.first.length > maxname
	end

	lines = []
	for enum_pair in enums[:num]
		lines << sprintf('#define %-*s %#06x', maxname, enum_pair.first, enum_pair.last)
	end
	for enum_pair in enums[:str]
		lines << sprintf('#define %-*s %s', maxname, enum_pair.first, enum_pair.last)
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

def parse_gl_ext (fin, requested)
	enums = { :num => [], :str => [] }
	
	fin.each_line { |line|
		line.strip!
		case
			when line.empty?
			when md = /^\#define\s+([\w_]+)\s+([^\s]+)\s*$/.match(line)
				name = md[1]
				begin
					val = Integer(md[2])
					append_to = :num
				rescue ArgumentError => e
					val = md[2]
					append_to = :str
				end
				
				if requested.has_key?(name) then	
					enums[append_to] << [name, val]
					requested.delete(name)
				end
		end
	}
	
	raise "shithell, not all requested requests met: #{requested.size}: #{requested.inspect}" unless requested.empty?

	enums
end

def parse_requested_exts (fin)
	requested = {}
	fin.each_line { |line|
		line.strip!
		case
			when line.empty?
			when requested.has_key?(line)
				raise "hell: #{line.inspect}"
			when line =~ /^\/\//
			else
				requested[line] = true
		end
	}
	requested 
end

class StreamConcatenation
	def initialize (*streams)
		@streams = streams
	end
	
	def each_line (&b)
		streams = @streams.dup
		until streams.empty?
			stream = streams.shift
			stream.each_line(&b)
		end
	end
	
end

File.open(ARGV[3], IO::CREAT | IO::TRUNC | IO::WRONLY) { |fout|
	File.open(ARGV[0], IO::RDONLY) { |fin_gl|
	File.open(ARGV[1], IO::RDONLY) { |fin_glext|
	File.open(ARGV[2], IO::RDONLY) { |fin_requested|
		fout.puts header, enums_to_s(parse_gl_ext(StreamConcatenation.new(fin_gl, fin_glext), parse_requested_exts(fin_requested))), typedefs_to_s(typedefs), footer
	}}}
}
