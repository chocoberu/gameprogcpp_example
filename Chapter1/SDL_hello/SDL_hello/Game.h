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
	bool Initialize(); // ���� �ʱ�ȭ
	void RunLoop(); // ������ ������ ������ ���� ������ ����
	void Shutdown(); // ���� ����

private:
	// ���� ������ ���� ���� �Լ�
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;
	// Number of ticks since start of game
	Uint32 mTicksCount;

	// Pong specific
	
	//int mPaddleDir; // paddle�� ����
	//Vector2 mPaddlePos; // paddle�� ��ġ
	Vector2 mBallPos; // ���� ��ġ
	Vector2 mBallVel; // ���� �ӷ�

	Player player[2];
};