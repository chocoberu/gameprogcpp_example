#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() :mIsRunning(true),mRenderer(nullptr),mWindow(nullptr),mTicksCount(0)
{
	
}
bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO); // SDL ���̺귯�� �ʱ�ȭ
	if (sdlResult != 0) // ����� �ʱ�ȭ ���� �ʾҴٸ�
	{
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError()); // ���� �α�
		return false;
	}
	mWindow = SDL_CreateWindow( // ������ ����
		"HelloSDL",
		100,
		100,
		1024,
		768,
		0
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window : %s ", SDL_GetError());
		return false;
	}
	mRenderer = SDL_CreateRenderer(
		mWindow, // �������� ���� ������ ������
		-1, // �Ϲ������� -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC 
	);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// paddle�� ���� ��ġ �ʱ�ȭ
	//mPaddlePos.x = 10.0f;
	//mPaddlePos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	player[0].paddlePos.x = 10.0f;
	player[0].paddlePos.y = 768.0f / 2.0f;
	player[1].paddlePos.x = 1004.0f;
	player[1].paddlePos.y = 768.0f / 2.0f;

	return true;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer); // ������ ��ü ����
	SDL_DestroyWindow(mWindow); // ������ ��ü ����
	SDL_Quit(); // SDL ����
}
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) // ť�� ������ �̺�Ʈ�� ���� �ִ� ����
	{
		switch (event.type) 
		{
		case SDL_QUIT: // �̺�Ʈ�� SDL_QUIT�̸� ������ ����
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	//// W/S Ű�� �е��� ������ ����
	//mPaddleDir = 0;
	//if (state[SDL_SCANCODE_W])
	//{
	//	mPaddleDir -= 1;
	//}
	//if (state[SDL_SCANCODE_S])
	//{
	//	mPaddleDir += 1;
	//}

	player[0].paddleDir = 0;
	player[1].paddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		player[0].paddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		player[0].paddleDir += 1;
	}

	if (state[SDL_SCANCODE_I])
	{
		player[1].paddleDir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		player[1].paddleDir += 1;
	}
}
void Game::UpdateGame()
{
	// ��Ÿ �ð��� ���� ������ ƽ���� ���� ������ ƽ���� ��

	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) // ������ ������ ���ķ� 16ms�� ����� ������ ���
		;
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f; // (���� �� ������ ��ȯ)
	mTicksCount = SDL_GetTicks(); // ���� �������� ���� ƽ���� ����

	if (deltaTime > 0.05f) // �ִ� ��Ÿ �ð������� ����
	{
		deltaTime = 0.05f;
	}
	// ���� �������� ���� ƽ ī��Ʈ ������Ʈ
	mTicksCount = SDL_GetTicks();

	//// ���⿡ ���� �е� ��ġ ����
	//if (mPaddleDir != 0)
	//{
	//	mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
	//	// �е��� ȭ�� ������ ������ �ʵ��� ����
	//	if (mPaddlePos.y < (paddleH / 2.0f + thickness))
	//	{
	//		mPaddlePos.y = paddleH / 2.0f + thickness;
	//	}
	//	else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
	//	{
	//		mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
	//	}
	//}

	if (player[0].paddleDir != 0)
	{
		player[0].paddlePos.y += player[0].paddleDir * 300.0f * deltaTime;
		// �е��� ȭ�� ������ ������ �ʵ��� ����
		if (player[0].paddlePos.y < (paddleH / 2.0f + thickness))
		{
			player[0].paddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (player[0].paddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			player[0].paddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	if (player[1].paddleDir != 0)
	{
		player[1].paddlePos.y += player[1].paddleDir * 300.0f * deltaTime;
		// �е��� ȭ�� ������ ������ �ʵ��� ����
		if (player[1].paddlePos.y < (paddleH / 2.0f + thickness))
		{
			player[1].paddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (player[1].paddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			player[1].paddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// ���� �ӷ��� �������� ���� ��ġ ����
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	//// ���� �ٿ
	//// Did we intersect with the paddle?
	//float diff = mPaddlePos.y - mBallPos.y;
	//// ������ ���밪�� ����
	//diff = (diff > 0.0f) ? diff : -diff;
	//if (
	//	// y ���� ����� �۰�
	//	diff <= paddleH / 2.0f &&
	//	// ���� �ùٸ� x���� ���� �ְ�
	//	mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
	//	// ���� �������� �̵��ϰ� �ִٸ�
	//	mBallVel.x < 0.0f)
	//{
	//	mBallVel.x *= -1.0f;
	//}

	float diff1p = player[0].paddlePos.y - mBallPos.y;
	float diff2p = player[1].paddlePos.y - mBallPos.y;
	// ������ ���밪�� ����
	diff1p = (diff1p > 0.0f) ? diff1p : -diff1p;
	diff2p = (diff2p > 0.0f) ? diff2p : -diff2p;
	if (
		// y ���� ����� �۰�
		diff1p <= paddleH / 2.0f &&
		// ���� �ùٸ� x���� ���� �ְ�
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// ���� �������� �̵��ϰ� �ִٸ�
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	if (
		// y ���� ����� �۰�
		diff2p <= paddleH / 2.0f &&
		// ���� �ùٸ� x���� ���� �ְ�
		mBallPos.x <= 999.0f && mBallPos.x >= 989.0f &&
		// ���� ���������� �̵��ϰ� �ִٸ�
		mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	// ���� ��ũ�� ������ �����ٸ� ���� ����
	else if (mBallPos.x <= 0.0f || mBallPos.x >= 1024.0f)
	{
		mIsRunning = false;
	}

	//// ���� ������ ���� �浹�ߴٸ�
	//else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	//{
	//	mBallVel.x *= -1.0f;
	//}
	// ���� ���� ���� �浹�ߴٸ�
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	//  ���� �Ʒ��� ���� �浹�ߴٸ�
	else if (mBallPos.y >= (768 - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
}
void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(
		mRenderer,
		0, // R
		0, // G
		255,//B
		255// A
	);
	SDL_RenderClear(mRenderer); // �ĸ� ���� Ŭ����
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255); // ���� �׸���

	// ���� ���� ����
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall); // ���� ���� �׸���
	
	// �Ʒ��� ���� �׸���
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	//// ������ ���� �׸���
	//wall.x = 1024 - thickness;
	//wall.y = 0;
	//wall.w = thickness;
	//wall.h = 1024;
	//SDL_RenderFillRect(mRenderer, &wall);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // ���� �׸���
	//// Draw paddle
	//SDL_Rect paddle{
	//	static_cast<int>(mPaddlePos.x),
	//	static_cast<int>(mPaddlePos.y - paddleH / 2),
	//	thickness,
	//	static_cast<int>(paddleH)
	//};
	//SDL_RenderFillRect(mRenderer, &paddle);

	SDL_Rect paddle1p{
		static_cast<int>(player[0].paddlePos.x),
		static_cast<int>(player[0].paddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle1p);

	SDL_Rect paddle2p{
		static_cast<int>(player[1].paddlePos.x),
		static_cast<int>(player[1].paddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2p);

	// ���� �׸���
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2), // int ������ ���� ĳ����
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer); // ���� �ĸ� ���� ��ü
}