#pragma once
#include "SDL/SDL.h"

struct Vector2
{
	float x;
	float y;
};
struct Player
{
	int paddleDir;
	Vector2 paddlePos;
	
};
class Game
{
public:
	Game();
	bool Initialize(); // 게임 초기화
	void RunLoop(); // 게임이 끝나기 전까지 게임 루프를 실행
	void Shutdown(); // 게임 종료

private:
	// 게임 루프를 위한 헬퍼 함수
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;
	// Number of ticks since start of game
	Uint32 mTicksCount;

	// Pong specific
	
	//int mPaddleDir; // paddle의 방향
	//Vector2 mPaddlePos; // paddle의 위치
	Vector2 mBallPos; // 공의 위치
	Vector2 mBallVel; // 공의 속력

	Player player[2];
};