#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() :mIsRunning(true),mRenderer(nullptr),mWindow(nullptr),mTicksCount(0)
{
	
}
bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO); // SDL 라이브러리 초기화
	if (sdlResult != 0) // 제대로 초기화 되지 않았다면
	{
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError()); // 에러 로그
		return false;
	}
	mWindow = SDL_CreateWindow( // 윈도우 생성
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
		mWindow, // 렌더링을 위해 생성한 윈도우
		-1, // 일반적으로 -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC 
	);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// paddle과 공의 위치 초기화
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
	SDL_DestroyRenderer(mRenderer); // 렌더러 객체 해제
	SDL_DestroyWindow(mWindow); // 윈도우 객체 해제
	SDL_Quit(); // SDL 종료
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
	while (SDL_PollEvent(&event)) // 큐에 여전히 이벤트가 남아 있는 동안
	{
		switch (event.type) 
		{
		case SDL_QUIT: // 이벤트가 SDL_QUIT이면 루프를 종료
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	//// W/S 키로 패들의 방향을 설정
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
	// 델타 시간은 현재 프레임 틱값과 이전 프레임 틱값의 차

	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) // 마지막 프레임 이후로 16ms가 경과할 때까지 대기
		;
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f; // (차를 초 단위로 변환)
	mTicksCount = SDL_GetTicks(); // 다음 프레임을 위해 틱값을 갱신

	if (deltaTime > 0.05f) // 최대 델타 시간값으로 고정
	{
		deltaTime = 0.05f;
	}
	// 다음 프레임을 위해 틱 카운트 업데이트
	mTicksCount = SDL_GetTicks();

	//// 방향에 따라 패들 위치 조정
	//if (mPaddleDir != 0)
	//{
	//	mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
	//	// 패들이 화면 밖으로 나가지 않도록 설정
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
		// 패들이 화면 밖으로 나가지 않도록 설정
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
		// 패들이 화면 밖으로 나가지 않도록 설정
		if (player[1].paddlePos.y < (paddleH / 2.0f + thickness))
		{
			player[1].paddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (player[1].paddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			player[1].paddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// 공의 속력을 기준으로 공의 위치 설정
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	//// 공을 바운스
	//// Did we intersect with the paddle?
	//float diff = mPaddlePos.y - mBallPos.y;
	//// 차이의 절대값을 구함
	//diff = (diff > 0.0f) ? diff : -diff;
	//if (
	//	// y 차가 충분히 작고
	//	diff <= paddleH / 2.0f &&
	//	// 공이 올바른 x값을 갖고 있고
	//	mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
	//	// 공이 왼쪽으로 이동하고 있다면
	//	mBallVel.x < 0.0f)
	//{
	//	mBallVel.x *= -1.0f;
	//}

	float diff1p = player[0].paddlePos.y - mBallPos.y;
	float diff2p = player[1].paddlePos.y - mBallPos.y;
	// 차이의 절대값을 구함
	diff1p = (diff1p > 0.0f) ? diff1p : -diff1p;
	diff2p = (diff2p > 0.0f) ? diff2p : -diff2p;
	if (
		// y 차가 충분히 작고
		diff1p <= paddleH / 2.0f &&
		// 공이 올바른 x값을 갖고 있고
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// 공이 왼쪽으로 이동하고 있다면
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	if (
		// y 차가 충분히 작고
		diff2p <= paddleH / 2.0f &&
		// 공이 올바른 x값을 갖고 있고
		mBallPos.x <= 999.0f && mBallPos.x >= 989.0f &&
		// 공이 오른쪽으로 이동하고 있다면
		mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	// 공이 스크린 밖으로 나갔다면 게임 오버
	else if (mBallPos.x <= 0.0f || mBallPos.x >= 1024.0f)
	{
		mIsRunning = false;
	}

	//// 공이 오른쪽 벽과 충돌했다면
	//else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	//{
	//	mBallVel.x *= -1.0f;
	//}
	// 공이 위쪽 벽과 충돌했다면
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	//  공이 아래쪽 벽과 충돌했다면
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
	SDL_RenderClear(mRenderer); // 후면 버퍼 클리어
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255); // 벽을 그린다

	// 위쪽 벽을 설정
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall); // 위쪽 벽을 그린다
	
	// 아래쪽 벽을 그린다
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	//// 오른쪽 벽을 그린다
	//wall.x = 1024 - thickness;
	//wall.y = 0;
	//wall.w = thickness;
	//wall.h = 1024;
	//SDL_RenderFillRect(mRenderer, &wall);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // 벽을 그린다
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

	// 공을 그린다
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2), // int 값으로 정적 캐스팅
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer); // 전면 후면 버퍼 교체
}