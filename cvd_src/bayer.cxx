/*                       
	This file is part of the CVD Library.

	Copyright (C) 2005 The Authors

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 
    51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <string.h>
#include <cvd/colourspace.h>

//Written by Ethan

namespace CVD{namespace ColourSpace{

// TYPE is 0, 1, 2, 3 corresponding to the layouts RGGB = 0, GBRG = 1, GRBG = 2, BGGR = 3
// always works in blocks of two output pixels
template <int TYPE>
struct bayer_sample {
};

// RGRGRG
// GBGBGB
// RGRGRG
// GBGBGB
template <>
struct bayer_sample<0> {
	static inline void upper_left(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][0] = row[0];            out[0][1] = (row[1] + next[0])/2; out[0][2] = next[1];
		out[1][0] = (row[0]+row[2])/2; out[1][1] = row[1];               out[1][2] = next[1];
	}
	static inline void upper_row(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][0] = row[0];            out[0][1] = (row[-1]+ row[1] + next[0])/3; out[0][2] = (next[-1]+next[1])/2;
		out[1][0] = (row[0]+row[2])/2; out[1][1] = row[1];               	  out[1][2] = next[1];
	}
	static inline void upper_right(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][0] = row[0]; out[0][1] = (row[-1]+ row[1] + next[0])/3; out[0][2] = (next[-1]+next[1])/2;
		out[1][0] = row[0]; out[1][1] = row[1];               	       out[1][2] = next[1];
	}

	static inline void odd_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][0] = (previous[0] + next[0])/2; out[0][1] = row[0]; out[0][2] = row[1];
		out[1][0] = (previous[0] + next[0] + previous[2] + next[2])/4; out[1][1] = (row[0]+row[2]+previous[0]+next[0])/4;  out[1][2] = row[1];
	}
	static inline void odd_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][0] = (previous[0] + next[0])/2; out[0][1] = row[0]; out[0][2] = (row[-1]+row[1])/2;
		out[1][0] = (previous[0] + next[0] + previous[2] + next[2])/4; out[1][1] = (row[0]+row[2]+previous[1]+next[1])/4;  out[1][2] = row[1];
	}
	static inline void odd_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][0] = (previous[0] + next[0])/2; out[0][1] = row[0]; out[0][2] = (row[-1]+row[1])/2;
		out[1][0] = (previous[0] + next[0])/2; out[1][1] = (row[0]+previous[0]+next[0])/3;  out[1][2] = row[1];
	}

	static inline void even_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][0] = row[0]; out[0][1] = (row[1]+previous[0]+next[0])/3; out[0][2] = (previous[1]+next[1])/2;
		out[1][0] = (row[0]+row[2])/2; out[1][1] = row[1];  out[1][2] = out[0][2];
	}
	static inline void even_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][0] = row[0]; out[0][1] = (row[-1]+row[1]+previous[0]+next[0])/4; out[0][2] = (previous[-1]+next[-1]+previous[1]+next[1])/4;
		out[1][0] = (row[0]+row[2])/2; out[1][1] = row[1];  out[1][2] = (previous[0] + next[0])/2;
	}
	static inline void even_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][0] = row[0]; out[0][1] = (row[-1]+row[1]+previous[0]+next[0])/4; out[0][2] = (previous[-1]+next[-1]+previous[1]+next[1])/4;
		out[1][0] = row[0]; out[1][1] = row[1];  out[1][2] = (previous[0] + next[0])/2;
	}

	static inline void lower_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][0] = previous[0];  out[0][1] = row[0]; out[0][2] = row[1];
		out[1][0] = (previous[0]+previous[2])/2; out[1][1] = (row[0]+row[2]+previous[1])/3; out[1][2] = row[1];
	}
	static inline void lower_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][0] = previous[0]; out[0][1] = row[0]; out[0][2] = (row[-1]+row[1])/2;
		out[1][0] = (previous[0] + previous[2])/2; out[1][1] = (row[0]+row[2]+previous[1])/3;  out[1][2] = row[1];
	}
	static inline void lower_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][0] = previous[0]; out[0][1] = row[0]; out[0][2] = (row[-1]+row[1])/2;
		out[1][0] = previous[0]; out[1][1] = (row[0]+previous[0])/2;  out[1][2] = row[1];
	}
};

// BGBGBG
// GRGRGR
// BGBGBG
// GRGRGR
// swap red and blue
template <>
struct bayer_sample<3> {
	static inline void upper_left(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][2] = row[0];            out[0][1] = (row[1] + next[0])/2; out[0][0] = next[1];
		out[1][2] = (row[0]+row[2])/2; out[1][1] = row[1];               out[1][0] = next[1];
	}
	static inline void upper_row(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][2] = row[0];            out[0][1] = (row[-1]+ row[1] + next[0])/3; out[0][0] = (next[-1]+next[1])/2;
		out[1][2] = (row[0]+row[2])/2; out[1][1] = row[1];               	  out[1][0] = next[1];
	}
	static inline void upper_right(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][2] = row[0]; out[0][1] = (row[-1]+ row[1] + next[0])/3; out[0][0] = (next[-1]+next[1])/2;
		out[1][2] = row[0]; out[1][1] = row[1];               	       out[1][0] = next[1];
	}

	static inline void odd_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][2] = (previous[0] + next[0])/2; out[0][1] = row[0]; out[0][0] = row[1];
		out[1][2] = (previous[0] + next[0] + previous[2] + next[2])/4; out[1][1] = (row[0]+row[2]+previous[0]+next[0])/4;  out[1][0] = row[1];
	}
	static inline void odd_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][2] = (previous[0] + next[0])/2; out[0][1] = row[0]; out[0][0] = (row[-1]+row[1])/2;
		out[1][2] = (previous[0] + next[0] + previous[2] + next[2])/4; out[1][1] = (row[0]+row[2]+previous[1]+next[1])/4;  out[1][0] = row[1];
	}
	static inline void odd_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][2] = (previous[0] + next[0])/2; out[0][1] = row[0]; out[0][0] = (row[-1]+row[1])/2;
		out[1][2] = (previous[0] + next[0])/2; out[1][1] = (row[0]+previous[0]+next[0])/3;  out[1][0] = row[1];
	}

	static inline void even_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][2] = row[0]; out[0][1] = (row[1]+previous[0]+next[0])/3; out[0][0] = (previous[1]+next[1])/2;
		out[1][2] = (row[0]+row[2])/2; out[1][1] = row[1];  out[1][0] = out[0][0];
	}
	static inline void even_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][2] = row[0]; out[0][1] = (row[-1]+row[1]+previous[0]+next[0])/4; out[0][0] = (previous[-1]+next[-1]+previous[1]+next[1])/4;
		out[1][2] = (row[0]+row[2])/2; out[1][1] = row[1];  out[1][0] = (previous[1] + next[1])/2;
	}
	static inline void even_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		out[0][2] = row[0]; out[0][1] = (row[-1]+row[1]+previous[0]+next[0])/4; out[0][0] = (previous[-1]+next[-1]+previous[1]+next[1])/4;
		out[1][2] = row[0]; out[1][1] = row[1];  out[1][0] = (previous[1] + next[1])/2;
	}

	static inline void lower_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][2] = previous[0];  out[0][1] = row[0]; out[0][0] = row[1];
		out[1][2] = (previous[0]+previous[2])/2; out[1][1] = (row[0]+row[2]+previous[1])/3; out[1][0] = row[1];
	}
	static inline void lower_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][2] = previous[0]; out[0][1] = row[0]; out[0][0] = (row[-1]+row[1])/2;
		out[1][2] = (previous[0] + previous[2])/2; out[1][1] = (row[0]+row[2]+previous[1])/3;  out[1][0] = row[1];
	}
	static inline void lower_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][2] = previous[0]; out[0][1] = row[0]; out[0][0] = (row[-1]+row[1])/2;
		out[1][2] = previous[0]; out[1][1] = (row[0]+previous[0])/2;  out[1][0] = row[1];
	}
};

// GBGBGB
// RGRGRG
// GBGBGB
// RGRGRG
template <>
struct bayer_sample<1> {
	static inline void upper_left(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][0] = next[0];		out[0][1] = row[0]; 			out[0][2] = row[1];
		out[1][0] = (next[0]+next[2])/2;out[1][1] = (row[0]+row[2]+next[1])/3;	out[1][2] = row[1];
	}
	static inline void upper_row(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][0] = next[0];		out[0][1] = row[0]; 			out[0][2] = (row[-1]+row[1])/2;
		out[1][0] = (next[0]+next[2])/2;out[1][1] = (row[0]+row[2]+next[1])/3;	out[1][2] = row[1];
	}
	static inline void upper_right(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][0] = next[0];	out[0][1] = row[0]; 			out[0][2] = (row[-1]+row[1])/2;
		out[1][0] = next[0];	out[1][1] = (row[0]+next[1])/2;		out[1][2] = row[1];
	}

	static inline void odd_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<0>::even_left(out, previous, row, next);
	}
	static inline void odd_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<0>::even_row(out, previous, row, next);
	}
	static inline void odd_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<0>::even_right(out, previous, row, next);
	}

	static inline void even_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<0>::odd_left(out, previous, row, next);
	}
	static inline void even_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<0>::odd_row(out, previous, row, next);
	}
	static inline void even_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<0>::odd_right(out, previous, row, next);
	}

	static inline void lower_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][0] = row[0];  out[0][1] = (previous[0] + row[1])/2; 	out[0][2] = previous[1];
		out[1][0] = (row[0]+row[2])/2; out[1][1] = row[1]; 		out[1][2] = previous[1];
	}
	static inline void lower_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][0] = row[0];  out[0][1] = (row[-1]+previous[0] + row[1])/3;	out[0][2] = (previous[-1]+previous[1])/2;
		out[1][0] = (row[0]+row[2])/2; out[1][1] = row[1]; 			out[1][2] = previous[1];
	}
	static inline void lower_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][0] = row[0];  out[0][1] = (row[-1]+previous[0] + row[1])/3;	out[0][2] = (previous[-1]+previous[1])/2;
		out[1][0] = row[0];  out[1][1] = row[1]; 				out[1][2] = previous[1];
	}
};

// GRGRGR
// BGBGBG
// GRGRGR
// BGBGBG
template <>
struct bayer_sample<2> {
	static inline void upper_left(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][2] = next[0];		out[0][1] = row[0]; 			out[0][0] = row[1];
		out[1][2] = (next[0]+next[2])/2;out[1][1] = (row[0]+row[2]+next[1])/3;	out[1][0] = row[1];
	}
	static inline void upper_row(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][2] = next[0];		out[0][1] = row[0]; 			out[0][0] = (row[-1]+row[1])/2;
		out[1][2] = (next[0]+next[2])/2;out[1][1] = (row[0]+row[2]+next[1])/3;	out[1][0] = row[1];
	}
	static inline void upper_right(unsigned char (*out)[3], const unsigned char * row, const unsigned char * next){
		out[0][2] = next[0];	out[0][1] = row[0]; 			out[0][0] = (row[-1]+row[1])/2;
		out[1][2] = next[0];	out[1][1] = (row[0]+next[1])/2;		out[1][0] = row[1];
	}

	static inline void odd_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<3>::even_left(out, previous, row, next);
	}
	static inline void odd_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<3>::even_row(out, previous, row, next);
	}
	static inline void odd_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<3>::even_right(out, previous, row, next);
	}

	static inline void even_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<3>::odd_left(out, previous, row, next);
	}
	static inline void even_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<3>::odd_row(out, previous, row, next);
	}
	static inline void even_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row, const unsigned char * next){
		bayer_sample<3>::odd_right(out, previous, row, next);
	}

	static inline void lower_left(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][2] = row[0];  out[0][1] = (previous[0] + row[1])/2; 	out[0][0] = previous[1];
		out[1][2] = (row[0]+row[2])/2; out[1][1] = row[1]; 		out[1][0] = previous[1];
	}
	static inline void lower_row(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][2] = row[0];  out[0][1] = (row[-1]+previous[0] + row[1])/3;	out[0][0] = (previous[-1]+previous[1])/2;
		out[1][2] = (row[0]+row[2])/2; out[1][1] = row[1]; 			out[1][0] = previous[1];
	}
	static inline void lower_right(unsigned char (*out)[3], const unsigned char * previous, const unsigned char * row){
		out[0][2] = row[0];  out[0][1] = (row[-1]+previous[0] + row[1])/3;	out[0][0] = (previous[-1]+previous[1])/2;
		out[1][2] = row[0];  out[1][1] = row[1]; 				out[1][0] = previous[1];
	}
};

// TYPE is 0, 1, 2, 3 corresponding to the layouts RGGB = 0, GBRG = 1, GRBG = 2, BGGR = 3
template <int TYPE>
void bayer_to_rgb(const unsigned char* bggr, unsigned char* rgb, unsigned int width, unsigned int height)
{
  const unsigned char* row = bggr;
  const unsigned char* next = bggr+width;
  const unsigned char* prev = bggr;
  const unsigned int midcount = (width - 4)/2;

  unsigned char (*out)[3] = (unsigned char (*)[3])rgb;

  bayer_sample<TYPE>::upper_left(out, row, next);
  out += 2; row += 2; next += 2;
  for (unsigned int j=0; j<midcount; ++j ) {
	bayer_sample<TYPE>::upper_row(out, row, next);
	out += 2; row += 2; next += 2;
  }
  bayer_sample<TYPE>::upper_right(out, row, next);
  out += 2; row += 2; next += 2;

  // Middle rows
  for (unsigned int i=1; i<height-1; i+=2) {
	// odd row
	bayer_sample<TYPE>::odd_left(out, prev, row, next);
	out += 2; prev+= 2; row += 2; next += 2;
	for (unsigned int j=0; j<midcount; ++j) {
		bayer_sample<TYPE>::odd_row(out, prev, row, next);
		out += 2; prev+= 2; row += 2; next += 2;
	}
	bayer_sample<TYPE>::odd_right(out, prev, row, next);
	out += 2; prev+= 2; row += 2; next += 2;
	// even row
	bayer_sample<TYPE>::even_left(out, prev, row, next);
	out += 2; prev+= 2; row += 2; next += 2;
	for (unsigned int j=0; j<midcount; ++j) {
		bayer_sample<TYPE>::even_row(out, prev, row, next);
		out += 2; prev+= 2; row += 2; next += 2;
	}
	bayer_sample<TYPE>::even_right(out, prev, row, next);
	out += 2; prev+= 2; row += 2; next += 2;
  }
  // last row
  bayer_sample<TYPE>::lower_left(out, prev, row);
  out += 2; prev+= 2; row += 2;
  for (unsigned int j=0; j<midcount; ++j) {
	bayer_sample<TYPE>::lower_row(out, prev, row);
	out += 2; prev+= 2; row += 2;
  }
  bayer_sample<TYPE>::lower_right(out, prev, row);
}

void bayer_to_rgb_rggb(const unsigned char* rggb, unsigned char* rgb, unsigned int width, unsigned int height){
	bayer_to_rgb<0>(rggb, rgb, width, height);
}

void bayer_to_rgb_gbrg(const unsigned char* rggb, unsigned char* rgb, unsigned int width, unsigned int height){
	bayer_to_rgb<1>(rggb, rgb, width, height);
}

void bayer_to_rgb_grbg(const unsigned char* rggb, unsigned char* rgb, unsigned int width, unsigned int height){
	bayer_to_rgb<2>(rggb, rgb, width, height);
}

void bayer_to_rgb_bggr(const unsigned char* bggr, unsigned char* rgb, unsigned int width, unsigned int height){
	bayer_to_rgb<3>(bggr, rgb, width, height);
}

inline unsigned char cie(unsigned char * c) { return (c[0]*77 + c[1]*150 + c[2]*29)>>8; }

// TYPE is 0, 1, 2, 3 corresponding to the layouts RGGB = 0, GBRG = 1, GRBG = 2, BGGR = 3
template<int TYPE>
void bayer_to_grey(const unsigned char* bggr, unsigned char* grey, unsigned int width, unsigned int height)
{
  const unsigned char* row = bggr;
  const unsigned char* next = bggr+width;
  const unsigned char* prev = bggr;
  const unsigned int midcount = (width - 4)/2;

  unsigned char out[2][3];

  bayer_sample<TYPE>::upper_left(out, row, next);
  grey[0] = cie(out[0]); grey[1] = cie(out[1]);
  grey += 2; row += 2; next += 2;
  for (unsigned int j=0; j<midcount; ++j ) {
	bayer_sample<TYPE>::upper_row(out, row, next);
	grey[0] = cie(out[0]); grey[1] = cie(out[1]);
	grey += 2; row += 2; next += 2;
  }
  bayer_sample<TYPE>::upper_right(out, row, next);
  grey[0] = cie(out[0]); grey[1] = cie(out[1]);
  grey += 2; row += 2; next += 2;

  // Middle rows
  for (unsigned int i=1; i<height-1; i+=2) {
	// odd row
	bayer_sample<TYPE>::odd_left(out, prev, row, next);
	grey[0] = cie(out[0]); grey[1] = cie(out[1]);
	grey += 2; prev +=2; row += 2; next += 2;
	for (unsigned int j=0; j<midcount; ++j) {
		bayer_sample<TYPE>::odd_row(out, prev, row, next);
		grey[0] = cie(out[0]); grey[1] = cie(out[1]);
		grey += 2; prev +=2; row += 2; next += 2;
	}
	bayer_sample<TYPE>::odd_right(out, prev, row, next);
	grey[0] = cie(out[0]); grey[1] = cie(out[1]);
	grey += 2; prev +=2; row += 2; next += 2;
	// even row
	bayer_sample<TYPE>::even_left(out, prev, row, next);
	grey[0] = cie(out[0]); grey[1] = cie(out[1]);
	grey += 2; prev +=2; row += 2; next += 2;
	for (unsigned int j=0; j<midcount; ++j) {
		bayer_sample<TYPE>::even_row(out, prev, row, next);
		grey[0] = cie(out[0]); grey[1] = cie(out[1]);
		grey += 2; prev +=2; row += 2; next += 2;
	}
	bayer_sample<TYPE>::even_right(out, prev, row, next);
	grey[0] = cie(out[0]); grey[1] = cie(out[1]);
	grey += 2; prev +=2; row += 2; next += 2;
  }
  // last row
  bayer_sample<TYPE>::lower_left(out, prev, row);
  grey[0] = cie(out[0]); grey[1] = cie(out[1]);
  grey += 2; row += 2; prev += 2;
  for (unsigned int j=0; j<midcount; ++j) {
	grey[0] = cie(out[0]); grey[1] = cie(out[1]);
	grey += 2; row += 2; prev += 2;
  }
  bayer_sample<TYPE>::lower_right(out, prev, row);
}

void bayer_to_grey_rggb(const unsigned char* rggb, unsigned char* grey, unsigned int width, unsigned int height){
	bayer_to_grey<0>(rggb, grey, width, height);
}

void bayer_to_grey_gbrg(const unsigned char* rggb, unsigned char* grey, unsigned int width, unsigned int height){
	bayer_to_grey<1>(rggb, grey, width, height);
}

void bayer_to_grey_grbg(const unsigned char* rggb, unsigned char* grey, unsigned int width, unsigned int height){
	bayer_to_grey<2>(rggb, grey, width, height);
}

void bayer_to_grey_bggr(const unsigned char* bggr, unsigned char* grey, unsigned int width, unsigned int height){
	bayer_to_grey<3>(bggr, grey, width, height);
}

}}
