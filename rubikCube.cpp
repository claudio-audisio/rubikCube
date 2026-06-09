#include <cmath>
#include <format>
#include <raylib.h>

#include "constants.h"
#include "cube.h"
#include "utils.h"

RubikCube cube;

Camera3D camera = {
	.position   = { 5.0f, 5.0f, 5.0f },  // where the eye is in world space
	.target     = { 0.0f, 0.0f, 0.0f },  // what it looks at
	.up         = { 0.0f, 1.0f, 0.0f },  // which direction is "up" (almost always Y)
	.fovy       = 45.0f,                  // vertical field of view in degrees
	.projection = CAMERA_PERSPECTIVE      // or CAMERA_ORTHOGRAPHIC
};

void processPOV() {
	float angle = GetTime() * 0.0f; // manual control below
	static float orbit = 0.0f;
	static float dist  = 8.0f;
	static float height = 5.0f;

	if (IsKeyDown(KEY_LEFT))  orbit -= 3.0f * GetFrameTime();
	if (IsKeyDown(KEY_RIGHT)) orbit += 3.0f * GetFrameTime();
	if (IsKeyDown(KEY_W))     dist   = fmaxf(2.0f, dist - 10.0f * GetFrameTime());
	if (IsKeyDown(KEY_S))     dist  += 10.0f * GetFrameTime();
	if (IsKeyDown(KEY_UP))	  height = std::min(height + 15.0f * GetFrameTime(), 8.0f);
	if (IsKeyDown(KEY_DOWN))  height = std::max(height - 15.0f * GetFrameTime(), -8.0f);
	if (IsKeyPressed(KEY_KP_7))	  cube.rotate(Yp_ROT, ROT_LEFT);
	if (IsKeyPressed(KEY_KP_9))	  cube.rotate(Yp_ROT, ROT_RIGHT);
	if (IsKeyPressed(KEY_KP_4))	  cube.rotate(Yo_ROT, ROT_LEFT);
	if (IsKeyPressed(KEY_KP_6))	  cube.rotate(Yo_ROT, ROT_RIGHT);
	if (IsKeyPressed(KEY_KP_1))	  cube.rotate(Ym_ROT, ROT_LEFT);
	if (IsKeyPressed(KEY_KP_3))	  cube.rotate(Ym_ROT, ROT_RIGHT);

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

void DrawFaces(const Piece& piece) {
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
		DrawFaces(cube.pieces[i]);
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
		//drawVertexSpheres();

		//DrawGrid(10, 1.0f);

		EndMode3D();

		// HUD
		DrawFPS(10, 10);
		DrawText("arrows = orbit  |  W/S = zoom", 10, 30, 18, DARKGRAY);
		DrawText(std::format("{:.2f}   {:.2f}   {:.2f}", camera.position.x, camera.position.y, camera.position.z).c_str(), 10, 50, 18, DARKGRAY);

		if (cube.isSolved()) {
			DrawText("SOLVED !", 730, 100, 30, DARKPURPLE);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
