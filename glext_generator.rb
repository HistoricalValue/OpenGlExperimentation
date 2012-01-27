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

#include <stdlib.h>

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

DEFINE = /^\#define\s+([\w_]+)\s+([^\s]+)\s*$/

def find_value (fin, looking4)
	results = []
	
	fin.each_line { |line|
		if md = DEFINE.match(line) then
			begin
				name = md[1]
				val = Integer(md[2])
				(results << name) if val == looking4
			rescue ArgumentError
			end
		end
	}
	
	results
end

def parse_gl_ext (fin, requested_result)
	enums = { :num => [], :str => [] }
	
	requested = requested_result[:requested]
	
	fin.each_line { |line|
		line.strip!
		case
			when line.empty?
			when md = DEFINE.match(line)
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
	
	enums[:str].concat(requested_result[:custom].to_a)
	enums
end

def parse_requested_exts (fin)
	result = { :requested => {}, :custom => {} }
	requested = result[:requested]
	custom = result[:custom]
	
	fin.each_line { |line|
		line.strip!
		case
			when line.empty?
			when requested.has_key?(line)
				raise "hell (already added): #{line.inspect}"
			when line =~ /^\/\//
			when md = /^\#define\s+([\w_]+)\s+([^\s]*)\s*$/.match(line)
				name = md[1]
				val = md[2]
				raise "hell/double custom: #{name}:#{val}" if custom.has_key?(name) || requested.has_key?(name)
				custom[name] = val
			else
				requested[line] = true
		end
	}
	result
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
		nil
	end
	
	def rewind
		@streams.each(&:rewind)
		nil
	end
end

File.open(ARGV[4], IO::CREAT | IO::TRUNC | IO::WRONLY) { |fout|
	File.open(ARGV[0], IO::RDONLY) { |fin_gl|
	File.open(ARGV[1], IO::RDONLY) { |fin_glext|
	File.open(ARGV[2], IO::RDONLY) { |fin_requested|
		fin_concat = StreamConcatenation.new(fin_gl, fin_glext)
		# Find extensions
		requestedExtensions = parse_requested_exts(fin_requested)
		enums = parse_gl_ext(fin_concat, requestedExtensions)
		fout.puts(header, enums_to_s(enums), typedefs_to_s(typedefs), footer)
		# Reserve find (value -> name)
		fin_concat.rewind
		looking4 = Integer(ARGV[3])
		reverseLookuped = find_value(fin_concat, looking4)
		print("Looking for " + looking4.to_s + ": \n" + reverseLookuped.map { |s| " * " + s }.join("\n"))
	}}}
}
