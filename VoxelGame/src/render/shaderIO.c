#include <stdio.h>
#include <stdlib.h>

static size_t bufferaloc = 0;
static char* buffer = NULL;

char* GetShaderSource(const char* path)
{
	FILE* fp;
	fopen_s(&fp, path, "rb");
	if (fp == NULL) { puts("File not found!"); return NULL; }

	fseek(fp, 0L, SEEK_END);
	size_t filesize = ftell(fp) + 1;
	fseek(fp, 0L, SEEK_SET);

	if (filesize > bufferaloc)
	{
		if (buffer != NULL)
			free(buffer);

		buffer = malloc( sizeof(char) * filesize );
		bufferaloc = filesize;

		if (buffer == NULL)
		{
			bufferaloc = 0;
			return NULL;
		}
	}

	fread_s(buffer, bufferaloc, sizeof(char), filesize - 1, fp);
	buffer[filesize - 1] = '\0';

	fclose(fp);

	return buffer;

}

void ClearShaderSourceBuffer()
{
	if (buffer != NULL)
	{
		bufferaloc = 0;
		free(buffer);
		buffer = NULL;
	}
}