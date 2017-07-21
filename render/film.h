#ifndef GUI_FILM_H
#define GUI_FILM_H

namespace gui {
	class Vector3;
	
	class Film {
	public:
		Film(int width = 1024, int height = 512);
		~Film();

		void writePPM(char* name) const;
		void writeBMP(char* name) const;
		void gammaCorrection();

	public:
		int width;
		int height;
		Vector3** pixels;
	};
}

#endif
