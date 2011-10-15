#include <stdafx.h>

#include "TheCursed.h"

static char const* const VertexShaderSourcePath		= "../shaders/vertex2.txt";
static char const* const FragmentShaderSourcePath	= "../shaders/fragment.c";

static void ShaderReport (char const* const shaderType, char const* const sourcePath, char const* const msg) {
	char buf[1 << 16]; // 64KiB

	int const snprintf_result(_snprintf_s(buf, _countof(buf), sizeof(buf)/sizeof(buf[0]),
			"\n\n-------------- Report for %10s shader: --------------\n%15s: %s\n%15s: %s\n", shaderType, "Source", sourcePath, "Message", msg));
	DASSERT(snprintf_result > 0);

	my::global::log::infoA(buf);
}

static void ShaderReportAdapter (char const* const sourcePath, my::gl::shaders::ShaderCompiler const& comp) {
	ShaderReport(
			comp.GetShaderType() == my::gl::shaders::ShaderCompiler::ShaderType_FragmentShader?
					"Fragment"
				:
				comp.GetShaderType() == my::gl::shaders::ShaderCompiler::ShaderType_VertexShader?
					"Vertex"
				:
					"WAT",
			sourcePath,
			comp.GetErrorMessage().c_str());
}

static std::string const LoadSource (char const* const path) {
	std::ifstream		ifs(path);
	std::ostringstream	imp;

	DASSERT(!!ifs);
	DASSERT(!!imp);

	std::string	str;
	while (!ifs.eof()) {
		DASSERT(!ifs.bad());
		getline(ifs, str);
		imp << str << std::endl;
	}

	return imp.str();
}

namespace my {

void TestCompileShaders (void) {
	using my::gl::shaders::ShaderCompiler;

	struct Comp {
		ShaderCompiler vert, frag;
		Comp (void): vert(ShaderCompiler::ShaderType_VertexShader), frag(ShaderCompiler::ShaderType_FragmentShader) { }
	} comp;

	comp.vert.Compile(LoadSource(VertexShaderSourcePath		).c_str());
	comp.frag.Compile(LoadSource(FragmentShaderSourcePath	).c_str());

	ShaderReportAdapter(VertexShaderSourcePath, comp.vert);
	ShaderReportAdapter(FragmentShaderSourcePath, comp.frag);
}

}
