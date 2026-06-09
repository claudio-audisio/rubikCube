#pragma once
#include <iostream>
#include <ostream>
#include <raylib.h>

#include "constants.h"

struct Piece {
	Vector3 coordinates;
	Color colors[6];
};

class RubikCube {
public:
	Piece pieces[27];

	RubikCube() = default;

	void init() {
		for (int i = 0; i < 27; i++) {
			pieces[i] = initPiece(i);
		}
	}

	Piece initPiece(const int i) {
		Piece piece;
		piece.coordinates = {
			static_cast<float>(i < 9 ? -1 : i > 17 ? 1 : 0),
			static_cast<float>(i % 9 < 3 ? -1 : i % 9 > 5 ? 1 : 0),
			static_cast<float>(i % 3 == 0 ? -1 : i % 3 == 2 ? 1 : 0)
		};
		piece.colors[Xm_FACE] = piece.coordinates.x == -1 ? Xm_COL : BLACK;
		piece.colors[Ym_FACE] = piece.coordinates.y == -1 ? Ym_COL : BLACK;
		piece.colors[Zm_FACE] = piece.coordinates.z == -1 ? Zm_COL : BLACK;
		piece.colors[Xp_FACE] = piece.coordinates.x == 1 ? Xp_COL : BLACK;
		piece.colors[Yp_FACE] = piece.coordinates.y == 1 ? Yp_COL : BLACK;
		piece.colors[Zp_FACE] = piece.coordinates.z == 1 ? Zp_COL : BLACK;
		return piece;
	}

	/*void assignToFace(const int index, const Piece& piece) {
		switch (static_cast<int>(piece.coordinates.x)) {
			case -1: faces[Xm_FACE].pieces[index] = piece; break;
			case 1: faces[Xp_FACE].pieces[index - 16] = piece; break;
			default: break;
		}
		switch (static_cast<int>(piece.coordinates.y)) {
			case -1: faces[Ym_FACE].pieces[index / 9 + index % 3] = piece; break;
			case 1: faces[Yp_FACE].pieces[(index - 6) / 9 + index % 3] = piece; break;
			default: break;
		}
		switch (static_cast<int>(piece.coordinates.z)) {
			case -1: faces[Zm_FACE].pieces[index / 3] = piece; break;
			case 1: faces[Zp_FACE].pieces[(index - 2) / 3] = piece; break;
			default: break;
		}
	}*/
};