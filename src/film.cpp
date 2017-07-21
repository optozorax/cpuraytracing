#include <fstream>

#include "EasyBMP/EasyBMP.h"

#include "render/film.h"
#include "render/vector3.h"

namespace gui {
	Film::Film(int width, int height) : width(width), height(height) {
		pixels = new Vector3*[height];
		for(int i = 0; i < height; ++i) {
			pixels[i] = new Vector3[width];
			for (int j = 0; j < width; ++j) {
				pixels[i][j] = Vector3(0);
			}
		}
	}

	Film::~Film() {
		for(int i = 0; i < height; ++i) {
			delete[] pixels[i];
		}
		delete[] pixels;
	}

	void Film::gammaCorrection() {
		for(int i = 0; i < height; ++i) {
			for(int j = 0; j < width; ++j) {
				Vector3& c = pixels[i][j];
				c.x = sqrtf(c.x);
				c.y = sqrtf(c.y);
				c.z = sqrtf(c.z);

				if (c.x > 1) c.x = 1;
				if (c.y > 1) c.y = 1;
				if (c.z > 1) c.z = 1;
			}
		}
	}

	void Film::writePPM(char* name) const {
		std::ofstream file;
		file.open(name);
		file << "P3\n" << width << ' ' << height << '\n' << "255\n";
		for(int i = 0; i < height; ++i) {
			for(int j = 0; j < width; ++j) {
				file << (int) (pixels[i][j].x * 255) << ' '
					 << (int) (pixels[i][j].y * 255) << ' '
					 << (int) (pixels[i][j].z * 255) << ' ';
			}
			file << '\n';
		}
		file.close();
	}

	void Film::writeBMP(char* name) const {
		BMP AnImage;
		AnImage.SetBitDepth(24);
		AnImage.SetSize(width, height);

		RGBApixel pix;
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				pix.Alpha = 255;
				pix.Red = pixels[j][i].x * 255;
				pix.Green = pixels[j][i].y * 255;
				pix.Blue = pixels[j][i].z * 255;

				AnImage.SetPixel(i, j, pix);
			}
		}

		AnImage.WriteToFile(name);
	}
}
