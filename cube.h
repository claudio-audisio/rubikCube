#pragma once
#include <iostream>
#include <list>
#include <ostream>
#include <raylib.h>
#include <unordered_set>
#include <vector>

#include "constants.h"
#include "utils.h"

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
		std::cout << i << " -> " << piece.coordinates.x << " " << piece.coordinates.y << " " << piece.coordinates.z << std::endl;
		return piece;
	}

	bool isSolved() {
		const Color colXm = pieces[0].colors[Xm_FACE];
		const Color colYm = pieces[0].colors[Ym_FACE];
		const Color colZm = pieces[0].colors[Zm_FACE];
		const Color colXp = pieces[26].colors[Xp_FACE];
		const Color colYp = pieces[26].colors[Yp_FACE];
		const Color colZp = pieces[26].colors[Zp_FACE];

		for (const auto& piece : pieces) {
			if (piece.coordinates.x == -1 && !compareColors(piece.colors[Xm_FACE], colXm) ||
				piece.coordinates.y == -1 && !compareColors(piece.colors[Ym_FACE], colYm) ||
				piece.coordinates.z == -1 && !compareColors(piece.colors[Zm_FACE], colZm) ||
				piece.coordinates.x == 1 && !compareColors(piece.colors[Xp_FACE], colXp) ||
				piece.coordinates.y == 1 && !compareColors(piece.colors[Yp_FACE], colYp) ||
				piece.coordinates.z == 1 && !compareColors(piece.colors[Zp_FACE], colZp)) {
				return false;
			}
		}

		return true;
	}

	void rotate(const int rotation, const int direction) {
		switch (rotation) {
		case Yp_ROT:
			swapColorsOnY(direction, { 6, 8, 26, 24 });
			swapColorsOnY(direction, { 7, 17, 25, 15 });
			break;
		case Yo_ROT:
			swapColorsOnY(direction, { 3, 5, 23, 21 });
			swapColorsOnY(direction, { 4, 14, 22, 12 });
			break;
		case Ym_ROT:
			swapColorsOnY(direction, { 0, 2, 20, 18 });
			swapColorsOnY(direction, { 1, 11, 19, 9 });
			break;
		}
	}

	void swapColorsOnY(const int direction, const std::vector<int>& positions) {
		Color tmp[6];

		if (direction == ROT_RIGHT) {
			tmp[Xm_FACE] = pieces[positions[3]].colors[Xm_FACE];
			tmp[Xp_FACE] = pieces[positions[3]].colors[Xp_FACE];
			tmp[Ym_FACE] = pieces[positions[3]].colors[Ym_FACE];
			tmp[Yp_FACE] = pieces[positions[3]].colors[Yp_FACE];
			tmp[Zm_FACE] = pieces[positions[3]].colors[Zm_FACE];
			tmp[Zp_FACE] = pieces[positions[3]].colors[Zp_FACE];
			for (int i = 3; i > 0; i--) {
				pieces[positions[i]].colors[Xm_FACE] = pieces[positions[i - 1]].colors[Zm_FACE];
				pieces[positions[i]].colors[Xp_FACE] = pieces[positions[i - 1]].colors[Zp_FACE];
				pieces[positions[i]].colors[Ym_FACE] = pieces[positions[i - 1]].colors[Ym_FACE];
				pieces[positions[i]].colors[Yp_FACE] = pieces[positions[i - 1]].colors[Yp_FACE];
				pieces[positions[i]].colors[Zm_FACE] = pieces[positions[i - 1]].colors[Xp_FACE];
				pieces[positions[i]].colors[Zp_FACE] = pieces[positions[i - 1]].colors[Xm_FACE];
			}
			pieces[positions[0]].colors[Xm_FACE] = tmp[Zm_FACE];
			pieces[positions[0]].colors[Xp_FACE] = tmp[Zp_FACE];
			pieces[positions[0]].colors[Ym_FACE] = tmp[Ym_FACE];
			pieces[positions[0]].colors[Yp_FACE] = tmp[Yp_FACE];
			pieces[positions[0]].colors[Zm_FACE] = tmp[Xp_FACE];
			pieces[positions[0]].colors[Zp_FACE] = tmp[Xm_FACE];
		} else {
			tmp[Xm_FACE] = pieces[positions[0]].colors[Xm_FACE];
			tmp[Xp_FACE] = pieces[positions[0]].colors[Xp_FACE];
			tmp[Ym_FACE] = pieces[positions[0]].colors[Ym_FACE];
			tmp[Yp_FACE] = pieces[positions[0]].colors[Yp_FACE];
			tmp[Zm_FACE] = pieces[positions[0]].colors[Zm_FACE];
			tmp[Zp_FACE] = pieces[positions[0]].colors[Zp_FACE];
			for (int i = 0; i < 3; i++) {
				pieces[positions[i]].colors[Zm_FACE] = pieces[positions[i + 1]].colors[Xm_FACE];
				pieces[positions[i]].colors[Zp_FACE] = pieces[positions[i + 1]].colors[Xp_FACE];
				pieces[positions[i]].colors[Ym_FACE] = pieces[positions[i + 1]].colors[Ym_FACE];
				pieces[positions[i]].colors[Yp_FACE] = pieces[positions[i + 1]].colors[Yp_FACE];
				pieces[positions[i]].colors[Xp_FACE] = pieces[positions[i + 1]].colors[Zm_FACE];
				pieces[positions[i]].colors[Xm_FACE] = pieces[positions[i + 1]].colors[Zp_FACE];
			}
			pieces[positions[3]].colors[Zm_FACE] = tmp[Xm_FACE];
			pieces[positions[3]].colors[Zp_FACE] = tmp[Xp_FACE];
			pieces[positions[3]].colors[Ym_FACE] = tmp[Ym_FACE];
			pieces[positions[3]].colors[Yp_FACE] = tmp[Yp_FACE];
			pieces[positions[3]].colors[Xp_FACE] = tmp[Zm_FACE];
			pieces[positions[3]].colors[Xm_FACE] = tmp[Zp_FACE];
		}
	}

};