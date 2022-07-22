#pragma once

namespace Vin {
	typedef void (*VINapiproc)(void);
	typedef VINapiproc(*VINgetprocfunc)(const char* name);

	class Renderer {
	public:
		virtual int Init(VINgetprocfunc getprocfunc) = 0;
		virtual void SetViewport(int x, int y, int width, int height) = 0;
		virtual void Clear() = 0;
	};
}
