#ifndef _LEAN_SHADER_IO_H_
#define _LEAN_SHADER_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

	char* GetShaderSource(const char* path);
	void ClearShaderSourceBuffer();

#ifdef __cplusplus
}
#endif

#endif