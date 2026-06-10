#include <algorithm>
#include <cmath>
#include <format>
#include <raylib.h>

#include "cl_numbers.h"
#include "cl_random.h"
#include "constants.h"
#include "cube.h"
#include "utils.h"

RubikCube cube;
int selection = 0;
int faceSelection = -1;

Camera3D camera = {
	.position   = { 5.0f, 5.0f, 5.0f },  // where the eye is in world space
	.target     = { 0.0f, 0.0f, 0.0f },  // what it looks at
	.up         = { 0.0f, 1.0f, 0.0f },  // which direction is "up" (almost always Y)
	.fovy       = 45.0f,                  // vertical field of view in degrees
	.projection = CAMERA_PERSPECTIVE      // or CAMERA_ORTHOGRAPHIC
};

int getRotation(const int rotation) {
	if (rotation == ROT_DOWN || rotation == ROT_UP) {
		if (faceSelection == Xm_FACE) {
			switch (selection) {
			case 1: case 4: case 7: return Zm_ROT;
			case 2: case 5: case 8: return Zo_ROT;
			case 3: case 6: case 9: return Zp_ROT;
			}
		} else if (faceSelection == Xp_FACE) {
			switch (selection) {
			case 1: case 4: case 7: return Zp_ROT;
			case 2: case 5: case 8: return Zo_ROT;
			case 3: case 6: case 9: return Zm_ROT;
			}
		} else if (faceSelection == Zm_FACE) {
			switch (selection) {
			case 1: case 4: case 7: return Xp_ROT;
			case 2: case 5: case 8: return Xo_ROT;
			case 3: case 6: case 9: return Xm_ROT;
			}
		} else if (faceSelection == Zp_FACE) {
			switch (selection) {
			case 1: case 4: case 7: return Xm_ROT;
			case 2: case 5: case 8: return Xo_ROT;
			case 3: case 6: case 9: return Xp_ROT;
			}
		}
	} else {
		switch (selection) {
		case 1: case 2: case 3: return Ym_ROT;
		case 4: case 5: case 6: return Yo_ROT;
		case 7: case 8: case 9: return Yp_ROT;
		}
	}

	return NO_ROT;
}

bool isMinorFaceSelection() {
	return faceSelection < 3;
}

void shuffle(const int amount) {
	for (int i = 0; i < amount; i++) {
		selection = cl::randomInt(1, 9);
		faceSelection = cl::randomInt(2, 5);
		int rotation = cl::randomInt(0, 3);
		cube.rotate(getRotation(rotation), rotation);
	}
}

void processPOV() {
	float angle = GetTime() * 0.0f; // manual control below
	static float orbit = 0.0f;
	static float dist = 8.0f;
	static float height = 5.0f;

	if (IsKeyDown(KEY_A)) orbit -= 3.0f * GetFrameTime();
	if (IsKeyDown(KEY_D)) orbit += 3.0f * GetFrameTime();
	/*if (IsKeyDown(KEY_UP))     dist   = fmaxf(2.0f, dist - 10.0f * GetFrameTime());
	if (IsKeyDown(KEY_DOWN))     dist  += 10.0f * GetFrameTime();*/
	if (IsKeyDown(KEY_S)) height = std::min(height + 15.0f * GetFrameTime(), 8.0f);
	if (IsKeyDown(KEY_W)) height = std::max(height - 15.0f * GetFrameTime(), -8.0f);

	if (IsKeyPressed(KEY_KP_0))	selection = 0;
	if (IsKeyPressed(KEY_KP_1))	selection = 1;
	if (IsKeyPressed(KEY_KP_2))	selection = 2;
	if (IsKeyPressed(KEY_KP_3))	selection = 3;
	if (IsKeyPressed(KEY_KP_4))	selection = 4;
	if (IsKeyPressed(KEY_KP_5))	selection = 5;
	if (IsKeyPressed(KEY_KP_6))	selection = 6;
	if (IsKeyPressed(KEY_KP_7))	selection = 7;
	if (IsKeyPressed(KEY_KP_8))	selection = 8;
	if (IsKeyPressed(KEY_KP_9))	selection = 9;

	if (IsKeyPressed(KEY_UP))		cube.rotate(getRotation(ROT_UP), isMinorFaceSelection() ? ROT_UP : ROT_DOWN);
	if (IsKeyPressed(KEY_DOWN))		cube.rotate(getRotation(ROT_DOWN), isMinorFaceSelection() ? ROT_DOWN : ROT_UP);
	if (IsKeyPressed(KEY_RIGHT))	cube.rotate(getRotation(ROT_RIGHT), ROT_RIGHT);
	if (IsKeyPressed(KEY_LEFT))		cube.rotate(getRotation(ROT_LEFT), ROT_LEFT);

	/*if (IsKeyPressed(KEY_U))	  cube.rotate(Zp_ROT, ROT_UP);
	if (IsKeyPressed(KEY_D))	  cube.rotate(Zp_ROT, ROT_DOWN);*/

	if (IsKeyPressed(KEY_SPACE))  shuffle(5);
	if (IsKeyPressed(KEY_R))	  cube.init();

	camera.position = {
		sinf(orbit) * dist,
		height,
		cosf(orbit) * dist
	};
}

void drawAxes() {
	DrawLine3D({ 0,0,0 }, { 3,0,0 }, RED);   // X
	DrawLine3D({ 0,0,0 }, { 0,3,0 }, GREEN);  // Y
	DrawLine3D({ 0,0,0 }, { 0,0,3 }, BLUE);   // Z
}

void drawFaces(const Piece& piece) {
	if (!compareColors(piece.colors[Xm_FACE], BLACK)) {
		DrawTriangle3D(
			{piece.coordinates.x - 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x - 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			piece.colors[Xm_FACE]);
		DrawTriangle3D(
			{piece.coordinates.x - 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			piece.colors[Xm_FACE]);
	}
	if (!compareColors(piece.colors[Ym_FACE], BLACK)) {
		DrawTriangle3D(
			{piece.coordinates.x - 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x - 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			piece.colors[Ym_FACE]);
		DrawTriangle3D(
			{piece.coordinates.x - 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			piece.colors[Ym_FACE]);
	}
	if (!compareColors(piece.colors[Zm_FACE], BLACK)) {
		DrawTriangle3D(
			{piece.coordinates.x - 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z - 0.5f},
			piece.colors[Zm_FACE]);
		DrawTriangle3D(
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z - 0.5f},
			piece.colors[Zm_FACE]);
	}
	if (!compareColors(piece.colors[Xp_FACE], BLACK)) {
		DrawTriangle3D(
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			piece.colors[Xp_FACE]);
		DrawTriangle3D(
			{piece.coordinates.x + 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			piece.colors[Xp_FACE]);
	}
	if (!compareColors(piece.colors[Yp_FACE], BLACK)) {
		DrawTriangle3D(
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			piece.colors[Yp_FACE]);
		DrawTriangle3D(
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z - 0.5f},
			piece.colors[Yp_FACE]);
	}
	if (!compareColors(piece.colors[Zp_FACE], BLACK)) {
		DrawTriangle3D(
			{piece.coordinates.x - 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z + 0.5f},
			piece.colors[Zp_FACE]);
		DrawTriangle3D(
			{piece.coordinates.x + 0.5f, piece.coordinates.y - 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x + 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z + 0.5f},
			{piece.coordinates.x - 0.5f, piece.coordinates.y + 0.5f, piece.coordinates.z + 0.5f},
			piece.colors[Zp_FACE]);
	}
}

void drawCube() {
	for (int i = 0; i < 27; i++) {
		DrawCubeWires(cube.pieces[i].coordinates, 1.0f, 1.0f, 1.0f, LINES_COL);
		drawFaces(cube.pieces[i]);
	}
}

void drawSelection() {
	if (selection > 0) {
		if (camera.position.x < 5.65f && cl::isBetween(camera.position.z, -5.65f, 5.65f)) {
			faceSelection = Xm_FACE;
			switch (selection) {
			case 1: DrawCube({-1, -1, -1}, 1, 1, 1, HL_COL); break;
			case 2: DrawCube({-1, -1, 0}, 1, 1, 1, HL_COL); break;
			case 3: DrawCube({-1, -1, 1}, 1, 1, 1, HL_COL); break;
			case 4: DrawCube({-1, 0, -1}, 1, 1, 1, HL_COL); break;
			case 5: DrawCube({-1,0, 0}, 1, 1, 1, HL_COL); break;
			case 6: DrawCube({-1, 0, 1}, 1, 1, 1, HL_COL); break;
			case 7: DrawCube({-1, 1, -1}, 1, 1, 1, HL_COL); break;
			case 8: DrawCube({-1, 1, 0}, 1, 1, 1, HL_COL); break;
			case 9: DrawCube({-1, 1, 1}, 1, 1, 1, HL_COL); break;
			}
		} else if (camera.position.x > 5.65f && cl::isBetween(camera.position.z, -5.65f, 5.65f)) {
			faceSelection = Xp_FACE;
			switch (selection) {
			case 1: DrawCube({1, -1, 1}, 1, 1, 1, HL_COL); break;
			case 2: DrawCube({1, -1, 0}, 1, 1, 1, HL_COL); break;
			case 3: DrawCube({1, -1, -1}, 1, 1, 1, HL_COL); break;
			case 4: DrawCube({1, 0, 1}, 1, 1, 1, HL_COL); break;
			case 5: DrawCube({1,0, 0}, 1, 1, 1, HL_COL); break;
			case 6: DrawCube({1, 0, -1}, 1, 1, 1, HL_COL); break;
			case 7: DrawCube({1, 1, 1}, 1, 1, 1, HL_COL); break;
			case 8: DrawCube({1, 1, 0}, 1, 1, 1, HL_COL); break;
			case 9: DrawCube({1, 1, -1}, 1, 1, 1, HL_COL); break;
			}
		} else if (cl::isBetween(camera.position.x, -5.65f, 5.65f) && camera.position.z < 5.65f) {
			faceSelection = Zm_FACE;
			switch (selection) {
			case 1: DrawCube({1, -1, -1}, 1, 1, 1, HL_COL); break;
			case 2: DrawCube({0, -1, -1}, 1, 1, 1, HL_COL); break;
			case 3: DrawCube({-1, -1, -1}, 1, 1, 1, HL_COL); break;
			case 4: DrawCube({1, 0, -1}, 1, 1, 1, HL_COL); break;
			case 5: DrawCube({0,0, -1}, 1, 1, 1, HL_COL); break;
			case 6: DrawCube({-1, 0, -1}, 1, 1, 1, HL_COL); break;
			case 7: DrawCube({1, 1, -1}, 1, 1, 1, HL_COL); break;
			case 8: DrawCube({0, 1, -1}, 1, 1, 1, HL_COL); break;
			case 9: DrawCube({-1, 1, -1}, 1, 1, 1, HL_COL); break;
			}
		} else if (cl::isBetween(camera.position.x, -5.65f, 5.65f) && camera.position.z > 5.65f) {
			faceSelection = Zp_FACE;
			switch (selection) {
			case 1: DrawCube({-1, -1, 1}, 1, 1, 1, HL_COL2); break;
			case 2: DrawCube({0, -1, 1}, 1, 1, 1, HL_COL2); break;
			case 3: DrawCube({1, -1, 1}, 1, 1, 1, HL_COL2); break;
			case 4: DrawCube({-1, 0, 1}, 1, 1, 1, HL_COL2); break;
			case 5: DrawCube({0,0, 1}, 1, 1, 1, HL_COL2); break;
			case 6: DrawCube({1, 0, 1}, 1, 1, 1, HL_COL2); break;
			case 7: DrawCube({-1, 1, 1}, 1, 1, 1, HL_COL2); break;
			case 8: DrawCube({0, 1, 1}, 1, 1, 1, HL_COL2); break;
			case 9: DrawCube({1, 1, 1}, 1, 1, 1, HL_COL2); break;
			}
		}
	}
}

void drawVertexSpheres() {
	DrawSphere({ 1.5,1.5,1.5 }, 0.1f, RED);
	DrawSphere({ 1.5,1.5,-1.5 }, 0.1f, RED);
	DrawSphere({ 1.5,-1.5,1.5 }, 0.1f, RED);
	DrawSphere({ 1.5,-1.5,-1.5 }, 0.1f, RED);
	DrawSphere({ -1.5,1.5,1.5 }, 0.1f, RED);
	DrawSphere({ -1.5,1.5,-1.5 }, 0.1f, RED);
	DrawSphere({ -1.5,-1.5,1.5 }, 0.1f, RED);
	DrawSphere({ -1.5,-1.5,-1.5 }, 0.1f, RED);
}

int main() {
	InitWindow(1600, 1200, "rubik's cube");
	SetTargetFPS(60);
	cube.init();

	while (!WindowShouldClose()) {
		processPOV();

		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		BeginMode3D(camera);

		//drawAxes();
		drawCube();
		drawSelection();
		//drawVertexSpheres();

		//DrawGrid(10, 1.0f);

		EndMode3D();

		// HUD
		DrawFPS(10, 10);
		DrawText("arrows = orbit  |  W/S = zoom", 10, 30, 18, DARKGRAY);
		DrawText(std::format("{:.2f}   {:.2f}   {:.2f}", camera.position.x, camera.position.y, camera.position.z).c_str(), 10, 50, 18, DARKGRAY);

		float progress = cube.getProgress();

		if (progress > 99.0f) {
			DrawText("SOLVED !", 730, 100, 30, DARKPURPLE);
		} else {
			DrawText(std::format("{:.2f}", progress).c_str(), 730, 100, 30, DARKPURPLE);
		}


		EndDrawing();
	}

	CloseWindow();

	return 0;
}
