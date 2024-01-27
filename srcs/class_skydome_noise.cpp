#include "class_skydome.h"


inline void	interpolate(unsigned char* buffer, int step) noexcept;

void Skydome::NoiseInterpolation(int const seed)
{
	// For every octave fill the texture and interpolate
	srand(seed);
	for (int i = 0 ; i < 8 ; i++) {
		for (int j = 0 ; j < 256*256 ; j++)
			noise[i][j] = rand()&255;
		interpolate(noise[i], 1<<i);
	}

}

inline void	interpolate(unsigned char* buffer, int step) noexcept
{
	for (int y = 0 ; y < 256 ; y += step) {
		for (int x = 0 ; x < 256 ; x += step) {
			float a = buffer[(y<<8)+x];
			float b = buffer[(y<<8)+((x+step)&255)];
			float c = buffer[(((y+step)&255)<<8)+x];
			float d = buffer[(((y+step)&255)<<8)+((x+step)&255)];
			for (int i = 0 ; i < step ; i++) {
				for (int j = 0 ; j < step ; j++) {
					float xf = (float)j / (float)step;
					float yf = (float)i / (float)step;
					xf = xf*xf*(3.f-2.f*xf);
					yf = yf*yf*(3.f-2.f*yf);
					float nx0 = a*(1.f-xf)+b*xf;
					float nx1 = c*(1.f-xf)+d*xf;
					buffer[(((y+i)<<8))+(x+j)] = nx0*(1.f-yf)+nx1*yf;
				}
			}
		}
	}
}

void Skydome::_NoiseComposition(int const frames)
{
		//offsets
		int offsets[8];
		int t = frames;
		offsets[0] = static_cast<int>(t * 0.055f) << 8;
		offsets[1] = static_cast<int>(t * 0.060f) << 8;
		offsets[2] = static_cast<int>(t * 0.065f) << 8;
		offsets[3] = static_cast<int>(t * 0.080f) << 8;
		offsets[4] = static_cast<int>(t * 0.085f) << 8;
		offsets[5] = static_cast<int>(t * 0.010f) << 8;
		offsets[6] = static_cast<int>(t * 0.15f) << 8;
		offsets[7] = static_cast<int>(t * 0.160f) << 8;

		//composition
		for (int i = 0 ; i < 65536 ; i++) {
			int sum = 0;
			for (int k = 0 ; k < 8 ; k++) {
				int index = ((i + offsets[k]) & 0x0000ffff);
				sum += (noise[k][index] << k);
			}
			sum += 128; 
			texture[i] = sum >> 8;
		}
}