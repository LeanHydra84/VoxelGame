#ifndef _TEXT_RENDER_HPP_
#define _TEXT_RENDER_HPP_

void initialize_freetype();
void draw_text(const char* text, int x, int y);
void draw_short(short num, int x, int y);

#endif