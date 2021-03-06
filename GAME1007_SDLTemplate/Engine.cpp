#include "Engine.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

int playerHealth = 3;
int coolDown = 0;
int itemNum = 0;

int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing engine..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay...
	{
		// Create the SDL window...
		cout << "First pass." << endl;
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr)
		{
			// Create the SDL renderer...(back buffer)
			cout << "Second pass." << endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, 1, NULL);
			if (m_pRenderer != nullptr)
			{
				// Initialize subsystems later...
				cout << "Third pass." << endl;
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
				{
					m_playerBullet = IMG_LoadTexture(m_pRenderer, "../assets/textures/PlayerBullet.png");
					m_playerIdleTexture = IMG_LoadTexture(m_pRenderer, "../assets/player/idle.png");
					m_playerRunTexture = IMG_LoadTexture(m_pRenderer, "../assets/player/run.png");
					m_playerAttackTexture = IMG_LoadTexture(m_pRenderer, "../assets/player/attack.png");
					m_yellowEnemyWalkTexture = IMG_LoadTexture(m_pRenderer, "../assets/enemy/walking.png");
					m_yellowEenemyAttackTexture = IMG_LoadTexture(m_pRenderer, "../assets/enemy/Attack.png");
					m_redEnemyWalkTexture = IMG_LoadTexture(m_pRenderer, "../assets/enemy/red walking.png");
					m_groundTexture = IMG_LoadTexture(m_pRenderer, "../assets/textures/ground.png");
					heartTexture = IMG_LoadTexture(m_pRenderer, "../assets/HUD/heart.png");
					m_pBGTexture = IMG_LoadTexture(m_pRenderer, "../assets/background/clouds.png");
					m_obstacletexture = IMG_LoadTexture(m_pRenderer, "../assets/textures/Woodsmall.png");
					m_pCupTexture = IMG_LoadTexture(m_pRenderer, "../assets/textures/cup.png");
					m_pgoal = IMG_LoadTexture(m_pRenderer, "../assets/textures/goal_texture2.png");
					m_titleScreen = IMG_LoadTexture(m_pRenderer, "../assets/textures/titlescreen.png");
					m_gameOverScreen = IMG_LoadTexture(m_pRenderer, "../assets/textures/gameOverScreen.png");
					m_winScreen = IMG_LoadTexture(m_pRenderer, "../assets/textures/winScreen.png");
					m_goodWinScreen = IMG_LoadTexture(m_pRenderer, "../assets/textures/winScreenGoodEnd.png");
					m_signTexture = IMG_LoadTexture(m_pRenderer, "../assets/textures/sign.png");
					m_levelOneText = IMG_LoadTexture(m_pRenderer, "../assets/textures/level1Sign.png");

					m_bossTexture = IMG_LoadTexture(m_pRenderer, "../assets/sprites/boss.png");
					m_laserSmallTexture = IMG_LoadTexture(m_pRenderer, "../assets/textures/laserthin.png");
					m_laserBigTexture = IMG_LoadTexture(m_pRenderer, "../assets/textures/laserthick.png");
					m_bossBackground = IMG_LoadTexture(m_pRenderer, "../assets/background/BossBackground.png");


					m_flyEnemyTexture = IMG_LoadTexture(m_pRenderer, "../assets/enemy/Flying Enemy.png");

					cout << "Fourth pass." << endl;

					if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
					{// Load the chunks into the Mix_Chunk vector.
						Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048); // Good for most games.
						Mix_AllocateChannels(16);
						m_pMusic = Mix_LoadMUS("../assets/Aud/limbo.mp3"); // Load the music track.
						if (m_pMusic == nullptr)
							cout << Mix_GetError() << endl;

						m_pMenuMusic = Mix_LoadMUS("../assets/sound/bensound-newdawn.mp3");
						if (m_pMenuMusic == nullptr)
							cout << Mix_GetError() << endl;
						
						m_pBullet = Mix_LoadWAV("../assets/Aud/bullet_sfx.wav");
						if (m_pBullet == nullptr)
							cout << Mix_GetError() << endl;
						
						m_pCheer = Mix_LoadWAV("../assets/Aud/cheer_win_sfx.wav");
						if (m_pCheer == nullptr)
							cout << Mix_GetError() << endl;
						
						m_pGameOver = Mix_LoadWAV("../assets/Aud/gameover_sfx2.wav");
						if (m_pGameOver == nullptr)
							cout << Mix_GetError() << endl;
						
						m_pJump = Mix_LoadWAV("../assets/Aud/jump_sfx.wav");
						if (m_pJump == nullptr)
							cout << Mix_GetError() << endl;
						
						m_pEnemy = Mix_LoadWAV("../assets/Aud/monster_sfx.wav");
						if (m_pEnemy == nullptr)
							cout << Mix_GetError() << endl; 

						m_pWin = Mix_LoadWAV("../assets/Aud/win_sfx.wav");
						if (m_pWin == nullptr)
							cout << Mix_GetError() << endl;


					}
					else return false;

				}
				else return false;
			}
			else return false; // Renderer creation failed.
		}
		else return false; // Window creation failed.
	}
	else return false; // initalization failed.
	m_fps = (Uint32)round(1.0 / (double)FPS * 1000); // Converts FPS into milliseconds, e.g. 16.67
	m_keystates = SDL_GetKeyboardState(nullptr);
	m_bg1.m_src = { 0,0,1024,768 };
	m_bg1.m_dst = { 0,0,1024,768 };

	m_pCollectibleLVL2_1 = new Collectible(m_pRenderer, m_pCupTexture, { 0,0,200,200 }, { 870, 300, 30, 30 });
	m_pCollectibleLVL2_2 = new Collectible(m_pRenderer, m_pCupTexture, { 0,0,200,200 }, { 100, 300, 30, 30 });
	m_pCollectibleLVL2_3 = new Collectible(m_pRenderer, m_pCupTexture, { 0,0,200,200 }, { 500, 300, 30, 30 });

	m_pCollectible2 = new Collectible(m_pRenderer, m_pCupTexture, { 0,0,200,200 }, { 100, 300, 30, 30 });
	m_pCollectible3 = new Collectible(m_pRenderer, m_pCupTexture, { 0,0,200,200 }, { 500, 300, 30, 30 });

	//int x = 0;
	//for (auto element : m_Platforms)
	//{
//		x++;
//		if (x != 5)
//			m_yellowEnemyCreation.push_back(new Enemy(element.x, element.y, element.x + element.w, element.y));
	//
	//}
	m_player.Init(m_pRenderer);
	m_yellowEnemy.m_src = { 0,0,200,292 };
	cout << "Initialization successful!" << endl;
	
	m_running = true;

	Mix_PlayMusic(m_pMenuMusic, -1);
	Mix_VolumeMusic(50);

	//Item
	

	//
	//Items LVL 2 
	/*m_pCollectibleLVL2_1 = new Collectible(m_pRenderer, m_pCupTexture, { 0,0,200,200 }, { 870, 300, 30, 30 });

	m_pCollectibleLVL2_2 = new Collectible(m_pRenderer, m_pCupTexture, { 0,0,200,200 }, { 100, 300, 30, 30 });

	m_pCollectibleLVL2_3 = new Collectible(m_pRenderer, m_pCupTexture, { 0,0,200,200 }, { 500, 300, 30, 30 });

	m_pGoalLvl2 = new Collectible(m_pRenderer, m_pgoal, { 0,0,465,135 }, { 435, 30, 150,75 });*/

	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;

		case SDL_KEYDOWN:
			if (gameState == 1) //checks if the game is in play mode 
			{
				if (event.key.keysym.sym == ' ' && (m_player.isGrounded() || m_player.isForgettable()))
				{
					m_player.SetGrounded(false);
					m_player.SetAccelY(-(JUMPFORCE));

					// change animation to running 
					m_player.SetRunning(true);
					Mix_VolumeChunk(m_pJump, 3);
					Mix_PlayChannel(-1, m_pJump, 0);
				}
			}


		case SDL_KEYUP:
			if (gameState == 1) //checks if the game is in play mode 
			{
				// change animation to idle 
				m_player.SetRunning(false);
				m_player.setAttack(false);
			}

		}
	}
}

bool Engine::KeyDown(const SDL_Scancode c)
{
	if (m_keystates[c] == 1)
		return true;
	else return false;
}

void Engine::CheckCollision()
{
	//collided = false; 
	m_player.SetGrounded(false);

	//I edited this section to work no matter how many platforms there are, @ Ryan on discord if it's confusing 
	for (SDL_Rect x : m_Platforms)
	{
		if (SDL_HasIntersection(m_player.GetDstRect(), &x))
		{
			if ((m_player.GetDstRect()->y + m_player.GetDstRect()->h) - (float)m_player.GetVelY() <= x.y + 5 && !collided)
			{
				//colliding with the top side of platforms. 
				m_player.SetGrounded(true);
				m_player.m_isFall = false;
				m_player.StopY();
				m_player.SetY(x.y - m_player.GetDstRect()->h);
			}
			else if (m_player.GetDstRect()->y - (float)m_player.GetVelY() >= (x.y + x.h) && !collided)
			{
				//colliding with the bottom side of platforms. 
				m_player.StopY();
				m_player.SetY(x.y + x.h);
			}
			else if ((m_player.GetDstRect()->x + m_player.GetDstRect()->w) - (float)m_player.GetVelX() <= x.x && !collided)
			{
				//colliding with the left side of platforms. 
				m_player.StopX();
				m_player.SetX(x.x - m_player.GetDstRect()->w);
			}
			else if (m_player.GetDstRect()->x - (float)m_player.GetVelX() >= (x.x + x.w) && !collided)
			{
				//colliding with the right side of platforms. 
				m_player.StopX();
				m_player.SetX(x.x + x.w);
			}

		}
	}
	for (SDL_Rect x : m_Obstacles)
	{
		if (SDL_HasIntersection(m_player.GetDstRect(), &x) && !collided)
		{
			if ((m_player.GetDstRect()->y + m_player.GetDstRect()->h) - (float)m_player.GetVelY() <= x.y && !collided)
			{
				//colliding with the top side of platforms. 
				m_player.SetGrounded(true);
				m_player.m_isFall = false;
				m_player.StopY();
				m_player.SetY(x.y - m_player.GetDstRect()->h);
			}
			else if (m_player.GetDstRect()->y - (float)m_player.GetVelY() >= (x.y + x.h) && !collided)
			{
				//colliding with the bottom side of platforms.
				m_player.StopY();
				m_player.SetY(x.y + x.h);
			}
			else if ((m_player.GetDstRect()->x + m_player.GetDstRect()->w) - (float)m_player.GetVelX() <= x.x && !collided)
			{
				//colliding with the left side of platforms.
				m_player.StopX();
				m_player.SetX(x.x - m_player.GetDstRect()->w);
			}
			else if (m_player.GetDstRect()->x - (float)m_player.GetVelX() >= (x.x + x.w) && !collided)
			{
				//colliding with the right side of platforms.
				m_player.StopX();
				m_player.SetX(x.x + x.w);
			}

		}
	}

	//enemy - player collision
	for (unsigned i = 0; i < m_yellowEnemyCreation.size(); i++)
	{
		// Cooldown feature
		if (coolDown > 0 && coolDown != 0)
		{
			coolDown--;
		}

		// If the player gets hit they lose health and can't get hit again unti the cooldown ends
		if (SDL_HasIntersection(m_player.GetDstRect(), m_yellowEnemyCreation[i]->GetRect()) && coolDown == 0)
		{
			cout << "Hit!!" << endl << endl;
			playerHealth--;
			coolDown = 300;
			cout << "You have " << playerHealth << " left" << endl << endl;

			// set attack animation to true
			m_yellowEnemyCreation[i]->SetAttack(true);

		}

		if (coolDown == 0)
		{
			// set attack animation back to false
			m_yellowEnemyCreation[i]->SetAttack(false);
		}

	}

	//boss laser damage/collision
	if (SDL_HasIntersection(m_player.GetDstRect(), &m_laserRect) && coolDown == 0 && laserTimer < 40)
	{
		playerHealth -= 1;
		coolDown = 150;
	}

	for (unsigned i = 0; i < m_flyingEnemyCreation.size(); i++)
	{
		// Cooldown feature
		if (coolDown > 0 && coolDown != 0)
		{
			coolDown--;
		}

		// If the player gets hit they lose health and can't get hit again unti the cooldown ends
		if (SDL_HasIntersection(m_player.GetDstRect(), m_flyingEnemyCreation[i]->GetRect()) && coolDown == 0)
		{
			cout << "Hit!!" << endl << endl;
			playerHealth--;
			coolDown = 300;
			cout << "You have " << playerHealth << " left" << endl << endl;

			// set attack animation to true
			m_flyingEnemyCreation[i]->SetAttack(true);

		}

		if (coolDown == 0)
		{
			// set attack animation back to false
			m_flyingEnemyCreation[i]->SetAttack(false);
		}

	}
	//Player right bullet - collision to enemy
	for (unsigned i = 0; i < m_yellowEnemyCreation.size(); i++)
	{
		for (unsigned a = 0; a < m_playerbullet.size(); a++)
			if (SDL_HasIntersection(m_playerbullet[a]->GetRekt(), m_yellowEnemyCreation[i]->GetRect()))
			{
				Mix_VolumeChunk(m_pEnemy, 20);
				Mix_PlayChannel(-1, m_pEnemy, 0);
				cout << "Player bullet Collision!" << endl;

				delete m_playerbullet[a]; // flag for reallocation
				m_playerbullet[a] = nullptr; // get rid of the dangling pointer
				m_playerbullet.erase(m_playerbullet.begin() + a);
				m_playerbullet.shrink_to_fit();
				cout << " Bullet Deleted \n";

				delete m_yellowEnemyCreation[i]; // flag for reallocation
				m_yellowEnemyCreation[i] = nullptr; // get rid of the dangling pointer
				m_yellowEnemyCreation.erase(m_yellowEnemyCreation.begin() + i);
				m_yellowEnemyCreation.shrink_to_fit();
				cout << " Enemy Deleted \n";

			}
	}

	//Player left bullet - collision to enemy
	for (unsigned i = 0; i < m_yellowEnemyCreation.size(); i++)
	{
		for (unsigned a = 0; a < m_playerleftbullet.size(); a++)
			if (SDL_HasIntersection(m_playerleftbullet[a]->GetRekt(), m_yellowEnemyCreation[i]->GetRect()))
			{
				Mix_VolumeChunk(m_pEnemy, 20);
				Mix_PlayChannel(-1, m_pEnemy, 0);
				cout << "Player bullet Collision!" << endl;

				delete m_playerleftbullet[a]; // flag for reallocation
				m_playerleftbullet[a] = nullptr; // get rid of the dangling pointer
				m_playerleftbullet.erase(m_playerleftbullet.begin() + a);
				m_playerleftbullet.shrink_to_fit();
				cout << " Bullet Deleted \n";

				delete m_yellowEnemyCreation[i]; // flag for reallocation
				m_yellowEnemyCreation[i] = nullptr; // get rid of the dangling pointer
				m_yellowEnemyCreation.erase(m_yellowEnemyCreation.begin() + i);
				m_yellowEnemyCreation.shrink_to_fit();
				cout << " Enemy Deleted \n";

			}
	}

	//Player R.bullet to Obstacle
	for (SDL_Rect x : m_Obstacles)
	{
		for (unsigned i = 0; i < m_playerbullet.size(); i++)
		{
			if (SDL_HasIntersection(m_playerbullet[i]->GetRekt(), &x))
			{
				delete m_playerbullet[i]; // flag for reallocation 
				m_playerbullet[i] = nullptr; // get rid of the dangling pointer 
				m_playerbullet.erase(m_playerbullet.begin() + i);
				m_playerbullet.shrink_to_fit();
			}

		}
	}

	//Player L.bullet to Obstacle
	for (SDL_Rect y : m_Obstacles)
	{
		for (unsigned i = 0; i < m_playerleftbullet.size(); i++)
		{
			if (SDL_HasIntersection(m_playerleftbullet[i]->GetRekt(), &y))
			{
				delete m_playerleftbullet[i]; // flag for reallocation 
				m_playerleftbullet[i] = nullptr; // get rid of the dangling pointer 
				m_playerleftbullet.erase(m_playerleftbullet.begin() + i);
				m_playerleftbullet.shrink_to_fit();
			}

		}
	}
	
	//Player R.bullet to Platforms
	for (SDL_Rect x : m_Platforms)
	{
		for (unsigned i = 0; i < m_playerbullet.size(); i++)
		{
			if (SDL_HasIntersection(m_playerbullet[i]->GetRekt(), &x))
			{
				delete m_playerbullet[i]; // flag for reallocation 
				m_playerbullet[i] = nullptr; // get rid of the dangling pointer 
				m_playerbullet.erase(m_playerbullet.begin() + i);
				m_playerbullet.shrink_to_fit();
			}
		}
	}
	
	//Player L.bullet to Platforms
	for (SDL_Rect y : m_Platforms)
	{
		for (unsigned i = 0; i < m_playerleftbullet.size(); i++)
		{

			if (SDL_HasIntersection(m_playerleftbullet[i]->GetRekt(), &y))
			{
				delete m_playerleftbullet[i]; // flag for reallocation 
				m_playerleftbullet[i] = nullptr; // get rid of the dangling pointer 
				m_playerleftbullet.erase(m_playerleftbullet.begin() + i);
				m_playerleftbullet.shrink_to_fit();
			}

		}
	}

	

	//Here's the logic for checking if the player fell off the edge
	if (m_player.GetDstRect()->y > 1000)
	{
		cout << "You died!" << "/n";

		//where the player respawns
		if (level == 3)
		{

		}
		else
		{
			m_player.SetX(130);
			m_player.SetY(250);
		}
		
		playerHealth--;
	}
}

void Engine::Update()
{
	//if Gamestate = level2 
	//move_platforms(); 
	/*m_pCollectibleLVL2_1->Update();
	m_pCollectibleLVL2_2->Update();
	m_pCollectibleLVL2_3->Update();
	m_pGoalLvl2->Update();

	*/



	//checks if the game is being played
	if (gameState == 1)
	{
		
		Mix_ResumeMusic();
		//Item
		m_pCollectible1->Update();
		m_pCollectible2->Update();
		m_pCollectible3->Update();
		m_pGoal->Update();
		
		if (level == 2)
		{
			move_platforms();
		}
		
		if (level == 3)
		{
			BossUpdate();
		}

		//move right and left
		if (KeyDown(SDL_SCANCODE_A))
		{
			m_player.SetAccelX(-1.0);
			m_player.SetRunning(true);
			// change animation to running
		}
		else if (KeyDown(SDL_SCANCODE_D))
		{
			m_player.SetAccelX(1.0);

			// change animation to running
			m_player.SetRunning(true);
		}

		if (KeyDown(SDL_SCANCODE_K))
		{
			// change animation to attack
			m_player.setAttack(true);
			if (m_shottimer == 0) {
				m_shottimer = 30;
				m_playerleftbullet.push_back(new LeftBullet({ m_player.GetDstRect()->x + 0, m_player.GetDstRect()->y + 20 }));
				m_playerleftbullet.shrink_to_fit();
				cout << "New bullet vector capacity: " << m_playerleftbullet.capacity() << endl;
				m_player.setAttack(true);
				Mix_VolumeChunk(m_pBullet, 15);
				Mix_PlayChannel(-1, m_pBullet, 0);
			}
		}
		else if (KeyDown(SDL_SCANCODE_L))
		{
			// change animation to attack
			m_player.setAttack(true);
			if (m_shottimer == 0) {
				m_shottimer = 30;
				m_playerbullet.push_back(new Bullet({ m_player.GetDstRect()->x + 60, m_player.GetDstRect()->y + 20 }));
				m_playerbullet.shrink_to_fit();
				cout << "New bullet vector capacity: " << m_playerbullet.capacity() << endl;
				m_player.setAttack(true);
				Mix_VolumeChunk(m_pBullet, 15);
				Mix_PlayChannel(-1, m_pBullet, 0);
			}
		}

		//wrap the player
		if (m_player.GetDstRect()->x < 0) m_player.SetX(1);
		else if (m_player.GetDstRect()->x > 1000.0) m_player.SetX(1000);
		
		//Update the player
		m_player.Update();
		
		if (m_player.GetDstRect()->x > (WIDTH / 3))
		{
			//m_Camera.x -= m_player.GetVelX();
			//m_Camera.x = -m_player.GetDstRect()->x+WIDTH/2;
		}

		for (unsigned i = 0; i < m_flyingEnemyCreation.size(); i++) // size() is actual filled numbers of elements
		{
			m_flyingEnemyCreation[i]->Update();
		}
			
		for (unsigned i = 0; i < m_yellowEnemyCreation.size(); i++) // size() is actual filled numbers of elements
		{
			m_yellowEnemyCreation[i]->Update();

			// Enemy delete
			for (unsigned i = 0; i < m_yellowEnemyCreation.size(); i++) // size() is actual filled numbers of elements
			{
				if (m_yellowEnemyCreation[i]->GetRect()->x < -100)
				{
					m_yellowEnemyCreation[i] = nullptr; // get rid of the dangling pointer
					delete m_yellowEnemyCreation[i]; // flag for reallocation
					m_yellowEnemyCreation.erase(m_yellowEnemyCreation.begin() + i);
					m_yellowEnemyCreation.shrink_to_fit();
					cout << " Enemy Deleted \n";

				}
			}
		}

		// Player respawns when health is 0 or below 0
		if (playerHealth == 0 || playerHealth < 0)
		{
			playerHealth = 3;
			m_player.Init(m_pRenderer);
		}
		CheckCollision();

		//cout << m_player.GetDstRect()->x << endl<<m_Camera.x<<endl;

		if (m_shottimer != 0) m_shottimer--;
		for (int i = 0; i < m_playerbullet.size(); i++)
			m_playerbullet[i]->Update();

		// Check for bullets going off - screen...
		for (unsigned i = 0; i < m_playerbullet.size(); i++)
		{
			if (m_playerbullet[i]->GetRekt()->x >= WIDTH)
			{
				delete m_playerbullet[i];
				m_playerbullet[i] = nullptr;
				m_playerbullet.erase(m_playerbullet.begin() + i);
				m_playerbullet.shrink_to_fit();
				break;
			}
		}

		for (int i = 0; i < m_playerleftbullet.size(); i++)
			m_playerleftbullet[i]->Update();

		for (unsigned i = 0; i < m_playerleftbullet.size(); i++)
		{
			if (m_playerleftbullet[i]->GetRekt()->x <= 0)
			{
				delete m_playerleftbullet[i];
				m_playerleftbullet[i] = nullptr;
				m_playerleftbullet.erase(m_playerleftbullet.begin() + i);
				m_playerleftbullet.shrink_to_fit();
				break;
			}
		}

		// item
		if (SDL_HasIntersection(m_player.GetDstRect(), m_pCollectible1->getDest()) && coolDown == 0 && !m_pCollectible1->IsCollected())
		{
			coolDown = 300;
			m_pCollectible1->setIsCollected(true);
			itemNum++;
			playerHealth++;
		}
		if (level == 2)
		{
			if (SDL_HasIntersection(m_player.GetDstRect(), m_pCollectible2->getDest()) && coolDown == 0 && !m_pCollectible2->IsCollected())
			{
				coolDown = 300;
				m_pCollectible2->setIsCollected(true);
				itemNum++;
				playerHealth++;
			}
			if (SDL_HasIntersection(m_player.GetDstRect(), m_pCollectible3->getDest()) && coolDown == 0 && !m_pCollectible3->IsCollected())
			{
				coolDown = 300;
				m_pCollectible3->setIsCollected(true);
				itemNum++;
				playerHealth++;
			}
		}
		
		//{ 435, 30, 150,75 }
		if (m_yellowEnemyCreation.size() == 0 && (m_player.GetDstRect()->x > 930 && m_player.GetDstRect()->y < 200) 
				|| level == 2 && (m_player.GetDstRect()->x > 435 && m_player.GetDstRect()->x < 535 && m_player.GetDstRect()->y < 75))
		{
			if(level < 3)
			LevelInitialize(++level);
			else
			{
				level = 1;
				gameState = 3; //win state
				Mix_PauseMusic();
				Mix_VolumeChunk(m_pWin, 16);
				Mix_VolumeChunk(m_pCheer, 16);
				Mix_PlayChannel(-1, m_pWin, 0);
				Mix_PlayChannel(-1, m_pCheer, 0);
			}
			
		}
		if (playerHealth == 0 && gameState == 1)
		{
			gameState = 4; //game over state
			level = 1;
			LevelInitialize(level);
			Mix_PauseMusic();
			Mix_VolumeChunk(m_pGameOver, 10);
			Mix_PlayChannel(-1, m_pGameOver, 0);
			
		}

		//boss level specific collission
		if (level == 3)
		{
			//boss getting hit collission
			for (unsigned i = 0; i < m_playerbullet.size(); i++)
			{
				if (SDL_HasIntersection(m_playerbullet[i]->GetRekt(), &m_bossRect))
				{
					if (bossActive)
					{
						m_bossHealth--;
					}
					delete m_playerbullet[i];
					m_playerbullet[i] = nullptr;
					m_playerbullet.erase(m_playerbullet.begin() + i);
					m_playerbullet.shrink_to_fit();
					break;
				}
			}
		}
		
	}
	//On the title screen if the player pressese enter, it starts the game
	if (gameState == 0)
	{
		//Mix_PauseMusic();
		if (KeyDown(SDL_SCANCODE_RETURN) && enterPressed == false)
		{
			Mix_PauseMusic();
			gameState = 1;
			Mix_PlayMusic(m_pMusic, -1);
			Mix_VolumeMusic(50); // 0-MIX_MAX_VOLUME (128).
			//deletes all enemies and recreate them
			LevelInitialize(level);
		}
		
	}

	if (gameState == 2) //unpauses the game
	{
		if (KeyDown(SDL_SCANCODE_P) && pPressed == false)
		{
			pPressed = true;
			gameState = 1;
		}
	}
	else if (gameState == 1) //pauses the game
	{
		if (KeyDown(SDL_SCANCODE_P) && pPressed == false)
		{
			pPressed = true;
			gameState = 2;
		}
	}
	else if (gameState == 4 || gameState == 3) //lets the player return to the title screen from the game over screen
	{
		
		if (KeyDown(SDL_SCANCODE_RETURN) && enterPressed == false)
		{
			Mix_PlayMusic(m_pMenuMusic, -1);
			Mix_VolumeMusic(50);
			gameState = 0;
			enterPressed = true;
			LevelInitialize(1);
		}
	}

	//checks for button releases
	if (!KeyDown(SDL_SCANCODE_P))
	{
		pPressed = false;
	}
	if (!KeyDown(SDL_SCANCODE_RETURN))
	{
		enterPressed = false;
	}
	
}

void Engine::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 64, 128, 255, 255);
	SDL_RenderClear(m_pRenderer);
	//Render Background
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, &m_bg1.m_src, &m_bg1.m_dst);
	if (level == 3)
	{
		SDL_RenderCopy(m_pRenderer, m_bossBackground, &m_bg1.m_src, &m_bg1.m_dst);
	}
	

	SDL_SetRenderDrawColor(m_pRenderer, 192, 64, 0, 255);

	//boss rendering
	if (level == 3)
	{
		BossRender();
	}

	if (gameState == 0) //Title screen
	{
		SDL_RenderCopy(m_pRenderer, m_titleScreen, NULL, &m_bg1.m_dst);
	}
	else if (gameState == 4) //game over screen
	{
		SDL_RenderCopy(m_pRenderer, m_gameOverScreen, NULL, &m_bg1.m_dst);
	}
	else if (gameState == 3)
	{
		if (playerHealth == 7)
		{
			SDL_RenderCopy(m_pRenderer, m_goodWinScreen, NULL, &m_bg1.m_dst);
		}
		else
		{
			SDL_RenderCopy(m_pRenderer, m_winScreen, NULL, &m_bg1.m_dst);
		}
	}
	else if (gameState == 1 || gameState == 2) // game is being played
	{
		//Render Platforms
		for (SDL_Rect x : m_Platforms)
		{

			SDL_RenderFillRect(m_pRenderer, &x);
			//SDL_RenderCopy(m_pRenderer, m_groundTexture, NULL, &x); OLD render code(stretches)
			//this should help with stretching
			SDL_RenderCopyEx(m_pRenderer, m_groundTexture, new SDL_Rect {0, 0, x.w, x.h}, &x, NULL, NULL, SDL_FLIP_NONE);
		}
		//Render Obstacles
		for (SDL_Rect x : m_Obstacles)
		{
			SDL_RenderFillRect(m_pRenderer, &x);
			//SDL_RenderCopy(m_pRenderer, m_obstacletexture, NULL, &x);
			SDL_RenderCopyEx(m_pRenderer, m_obstacletexture, new SDL_Rect{ 0, 0, x.w, x.h }, &x, NULL, NULL, SDL_FLIP_NONE);
		}
		//Render Right bullet
		for (int i = 0; i < m_playerbullet.size(); i++)
			SDL_RenderCopy(m_pRenderer, m_playerBullet, m_playerbullet[i]->GetSrc(), m_playerbullet[i]->GetDst());
		//Render Left bullet
		for (int i = 0; i < m_playerleftbullet.size(); i++)
			SDL_RenderCopyEx(m_pRenderer, m_playerBullet, m_playerleftbullet[i]->GetSrc(), m_playerleftbullet[i]->GetDst(), 0, 0, SDL_FLIP_HORIZONTAL);

		//Render Enemy
		for (unsigned i = 0; i < m_yellowEnemyCreation.size(); i++) // size() is actual filled numbers of elements
		{
			if (m_yellowEnemyCreation[i]->GetAttack() == false)
				m_yellowEnemyCreation[i]->Render(m_pRenderer, m_yellowEnemyWalkTexture, flipEnemy);
			else
				m_yellowEnemyCreation[i]->Render(m_pRenderer, m_yellowEenemyAttackTexture, flipEnemy);
		}
		for (unsigned i = 0; i < m_flyingEnemyCreation.size(); i++) // size() is actual filled numbers of elements
		{
				m_flyingEnemyCreation[i]->Render(m_pRenderer, m_flyEnemyTexture, flipEnemy);

		}
		// Render the player
		if (m_player.getRunning() == false && m_player.getAttack() == false)
		{
			m_player.Render(m_playerIdleTexture, m_player, flip);
		}
		else if (m_player.getAttack() == true)
		{
			m_player.Render(m_playerAttackTexture, m_player, flip);
		}
		else
		{
			m_player.Render(m_playerRunTexture, m_player, flip);
		}

		//Render item
		m_pCollectible1->Render();
		m_pCollectible2->Render();
		m_pCollectible3->Render();
		m_pGoal->Render();

		

		//SDL_RenderSetViewport(m_pRenderer, &m_Camera);

		// flip the sprites face to another side
		if (KeyDown(SDL_SCANCODE_A) || KeyDown(SDL_SCANCODE_K))
		{
			flip = SDL_FLIP_HORIZONTAL;
		}
		if (KeyDown(SDL_SCANCODE_D) || KeyDown(SDL_SCANCODE_L))
		{
			flip = SDL_FLIP_NONE;
		}

		//health rendering
		SDL_Rect heartRenderPosition = { 10, 30, 48, 48 };
		for (int i = 0; i < playerHealth; i++)
		{
			SDL_RenderCopy(m_pRenderer, heartTexture, NULL, &heartRenderPosition);
			heartRenderPosition.x += 64;
		}
		//sign rendering
		if (level == 1)
		{
			SDL_RenderCopy(m_pRenderer, m_signTexture, NULL, &m_signOne);
			//code for reading the sign
			if (SDL_HasIntersection(m_player.GetDstRect(), &m_signOne))
			{
				SDL_RenderCopy(m_pRenderer, m_levelOneText, NULL, &m_levelOneTextPosition);
			}
		}

		//laser rendering
		if (laserOnScreen)
		{
			LaserRender();
		}
	}
	
	// Any drawing here...

	if (level == 2)
	{
	/*m_pCollectibleLVL2_1->Render();
	m_pCollectibleLVL2_2->Render();
	m_pCollectibleLVL2_3->Render();
	m_pGoalLvl2->Render();*/
	}
	SDL_RenderPresent(m_pRenderer); // Flip buffers - send data to window.
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start; // 1055 - 1050 = 5ms
	if (m_delta < m_fps)
		SDL_Delay(m_fps - m_delta);

}

int Engine::Run()
{
	if (m_running == true)
	{
		return 1;
	}
	// Start and run the "engine"
	if (Init("GAME1007 M1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
	{
		return 2;
	}
	// We passed our initial checks, start the loop!
	while (m_running == true)
	{
		Wake();
		HandleEvents(); // Input
		Update();       // Processing
		Render();       // Output
		if (m_running == true)
			Sleep();
	}
	Clean();
	return 0;
}

void Engine::Clean()
{
	cout << "Cleaning engine..." << endl;
	for (int i = 0; i < m_yellowEnemyCreation.size(); i++)
	{
		delete m_yellowEnemyCreation[i]; // Flag for reallocation 
		m_yellowEnemyCreation[i] = nullptr;
	}
	m_yellowEnemyCreation.clear();
	m_yellowEnemyCreation.shrink_to_fit();
	for (int i = 0; i < m_playerbullet.size(); i++)
	{
		delete m_playerbullet[i]; // Flag for reallocation
		m_playerbullet[i] = nullptr;
	}
	m_playerbullet.clear();
	m_playerbullet.shrink_to_fit();
	for (int i = 0; i < m_playerleftbullet.size(); i++)
	{
		delete m_playerleftbullet[i]; // Flag for reallocation
		m_playerleftbullet[i] = nullptr;
	}
	m_playerleftbullet.clear();
	m_playerleftbullet.shrink_to_fit();
	SDL_DestroyTexture(m_playerIdleTexture);
	SDL_DestroyTexture(m_playerRunTexture);
	SDL_DestroyTexture(m_playerAttackTexture);
	SDL_DestroyTexture(m_yellowEnemyWalkTexture);
	SDL_DestroyTexture(m_redEnemyWalkTexture);
	SDL_DestroyTexture(m_groundTexture);
	SDL_DestroyTexture(heartTexture);
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_obstacletexture);
	SDL_DestroyTexture(m_yellowEenemyAttackTexture);
	SDL_DestroyTexture(m_pCupTexture);
	SDL_DestroyTexture(m_titleScreen);
	SDL_DestroyTexture(m_gameOverScreen);
	SDL_DestroyTexture(m_winScreen);
	Mix_FreeChunk(m_pWin);
	Mix_FreeChunk(m_pBullet);
	Mix_FreeChunk(m_pCheer);
	Mix_FreeChunk(m_pEnemy);
	Mix_FreeChunk(m_pGameOver);
	Mix_FreeMusic(m_pMusic);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

void Engine::LevelInitialize(int level)
{
	int i = 0;
	int x = 0;
	switch (level)
	{
	case 1: //loading level 1 into the current level variables

		playerHealth = 3; //makes sure that the player doesn't get bonus health if they beat the game with over 3 health

		m_pCollectibleLevelOne = new Collectible(m_pRenderer, m_pCupTexture, { 0,0,200,200 }, { 660, 300, 30, 30 });
		m_pGoalLevelOne = new Collectible(m_pRenderer, m_pgoal, { 0,0,465,135 }, { 940, 50, 75,50 });

		//delete all the current obstacles
		for (SDL_Rect obstacle : m_Obstacles)
		{
			m_Obstacles[i] = *new SDL_Rect();
			i++;
		}
		i = 0; //reset the value of i to 0 for the next loop
		//add the level 1 obstacles
		for(SDL_Rect obstacle : m_ObstaclesLevelOne)
		{
			m_Obstacles[i] = obstacle;
			i++;
		}
		i = 0;
		//delete all the current platforms
		for (SDL_Rect platform : m_Platforms)
		{
			m_Platforms[i] = *new SDL_Rect();
			i++;
		}
		i = 0;
		//add the level 1 platforms
		for (SDL_Rect platform : m_PlatformsLevelOne)
		{
			m_Platforms[i] = platform;
			i++;
		}
		m_pGoal = m_pGoalLevelOne; //set the goal location
		m_pCollectible1 = m_pCollectibleLevelOne; //set the collectible location
		m_pCollectible1->setIsCollected(false); //make it so you can collect the collectible again
		m_pCollectible2->setIsCollected(true);
		m_pCollectible3->setIsCollected(true);



		//set the player's position to the spawn location

		//deletes all enemies and recreate them
		m_yellowEnemyCreation.clear();
		m_yellowEnemyCreation.shrink_to_fit();
		m_flyingEnemyCreation.clear();
		m_flyingEnemyCreation.shrink_to_fit();
		
		for (auto element : m_Platforms)
		{
			x++;
			if (x < 5)
			{
				m_yellowEnemyCreation.push_back(new Enemy(element.x, element.y, element.x + element.w, element.y));
			}
			m_flyingEnemyCreation.push_back(new FlyingEnemy(500, 500, 500, 50));
			
		}
		m_player.SetX(levelOneSpawnX);
		m_player.SetY(levelOneSpawnY);
		break;
	case 2:
		timer = 0;
		timer2 = 0;
		timer3 = 0;
		m_pGoalLvl2 = new Collectible(m_pRenderer, m_pgoal, { 0,0,465,135 }, { 435, 30, 150,75 });
		for (SDL_Rect obstacle : m_Obstacles)
		{
			m_Obstacles[i] = *new SDL_Rect();
			i++;
		}
		i = 0; //reset the value of i to 0 for the next loop
		//add the level 2 obstacles
		for (SDL_Rect obstacle : m_ObstaclesLevelTwo)
		{
			m_Obstacles[i] = obstacle;
			i++;
		}
		i = 0;
		//delete all the current platforms
		for (SDL_Rect platform : m_Platforms)
		{
			m_Platforms[i] = *new SDL_Rect();
		}
		i = 0;
		//add the level 2 platforms
		for (SDL_Rect obstacle : m_PlatformsLevelTwo)
		{
			m_Platforms[i] = obstacle;
			i++;
		}
		m_pGoal = m_pGoalLvl2; //set the goal location
		m_pCollectible1 = m_pCollectibleLVL2_1; //set the collectible location
		//set the player's position to the spawn location

		m_pCollectible1 = m_pCollectibleLVL2_1; //set the collectible location
		m_pCollectible1->setIsCollected(false); //make it so you can collect the collectible again
		m_pCollectible2 = m_pCollectibleLVL2_2; //set the collectible location
		m_pCollectible2->setIsCollected(false); //make it so you can collect the collectible again
		m_pCollectible3 = m_pCollectibleLVL2_3; //set the collectible location
		m_pCollectible3->setIsCollected(false); //make it so you can collect the collectible again

		//deletes all enemies and recreate them
		m_yellowEnemyCreation.clear();
		m_yellowEnemyCreation.shrink_to_fit();
		m_flyingEnemyCreation.clear();
		m_flyingEnemyCreation.shrink_to_fit();

		for (auto element : m_Platforms)
		{
			x++;
			if (x != 5)
			{
				//for now I'm deleting all the grounded enemies from level 2 and making it purely a platforming level, since
				//they don't function properly on moving platforms
				//m_yellowEnemyCreation.push_back(new Enemy(element.x, element.y, element.x + element.w, element.y));
			}
			m_flyingEnemyCreation.push_back(new FlyingEnemy(500, 500, 500, 50));

		}
		m_player.SetX(levelOneSpawnX);
		m_player.SetY(levelOneSpawnY);

		break;
	case 3:
		//reset boss
		bossDying = false;
		m_bossHealth = 30;
		m_bossRect = { 1200, 0, 350, 350 };
		bossActive = false;

		//change the music
		Mix_PauseMusic();
		Mix_PlayMusic(m_pMenuMusic, -1);

		m_pGoal->setIsCollected(true);
		//delete all the current obstacles
		for (SDL_Rect obstacle : m_Obstacles)
		{
			m_Obstacles[i] = *new SDL_Rect();
			i++;
		}
		i = 0; //reset the value of i to 0 for the next loop
		//add the level 1 obstacles
		for (SDL_Rect obstacle : m_ObstaclesLevelThree)
		{
			m_Obstacles[i] = obstacle;
			i++;
		}
		i = 0;
		//delete all the current platforms
		for (SDL_Rect platform : m_Platforms)
		{
			m_Platforms[i] = *new SDL_Rect();
			i++;
		}
		i = 0;

		//deletes all enemies and recreate them
		m_yellowEnemyCreation.clear();
		m_yellowEnemyCreation.shrink_to_fit();
		m_flyingEnemyCreation.clear();
		m_flyingEnemyCreation.shrink_to_fit();

		m_pCollectible1->setIsCollected(true); 
		m_pCollectible2->setIsCollected(true);
		m_pCollectible3->setIsCollected(true);

		m_player.SetX(130);
		m_player.SetY(250);
		break;

	default:
		break;
	}
}
void Engine::move_platforms()
{

	timer++;
	timer2++;
	timer3++;

	if (timer > 0 && timer <= 210)
	{
		m_Platforms[3].x--;
		m_Platforms[4].y++;
		m_Platforms[2].x++;
		m_Platforms[5].y++;
	}
	else if(timer > 230 && timer <= 440)
	{
		m_Platforms[3].x++;
		m_Platforms[2].x--;
		m_Platforms[4].y--;
		m_Platforms[5].y--;

	}
	if (timer > 460)
		timer = 0;


	if (timer2 > 0 && timer2 <= 700)
	{
		m_Platforms[6].x++;
	}
	else
	{
		m_Platforms[6].x--;
	}
	if (timer2 > 1400)
		timer2 = 0;

	if (timer3 >= 0 && timer3 <= 50)
	{
		m_Platforms[7].y++;
		m_Platforms[8].y++;
	}
	else
	{
		m_Platforms[7].y--;
		m_Platforms[8].y--;
	}
	if (timer3 >= 100)
		timer3 = 0;

}

//boss stuff from here down
void Engine::BossUpdate()
{
	//boss horizontal movement for the start of the fight
	if (m_bossHealth > 0)
	{
		if (!bossActive && m_bossRect.x > 600)
		{
			m_bossRect.x--;
		}
		else
		{
			bossActive = true;
			if (laserTimer < 40)
			{
				laserTimer++;
				//render the laser because it's been fired recently
				laserOnScreen = true;
			}
			else
			{
				laserOnScreen = false;
			}
			//bosses vertical movement
			if (bossGoingDown)
			{
				m_bossRect.y++;
				if (m_bossRect.y > 300)
					//swap to up
					bossGoingDown = false;
			}
			else
			{
				m_bossRect.y--;
				if (m_bossRect.y < -100)
					//swap to down
					bossGoingDown = true;
			}
			for (int i = 0; i < 4; i++)
			{
				if (m_bossRect.y + m_bossRect.h - 70 == 300
					|| m_bossRect.y + m_bossRect.h - 70 == 390
					|| m_bossRect.y + m_bossRect.h - 70 == 480
					|| m_bossRect.y + m_bossRect.h - 70 == 570)
				{
					laserTimer = 0;
					//this should make the laser engulf the platform
					laserY = m_bossRect.y + m_bossRect.h - 100;
					m_laserRect = { -400, laserY, 1000, 64 };
				}
			}
		}
	}
	else
	{
		bossDying = true;
		m_bossRect.y += 2;
		if (m_bossRect.y > 756)
		{
			gameState = 3;
			Mix_PauseMusic();
			Mix_VolumeChunk(m_pWin, 16);
			Mix_VolumeChunk(m_pCheer, 16);
			Mix_PlayChannel(-1, m_pWin, 0);
			Mix_PlayChannel(-1, m_pCheer, 0);
		}
	}
}

void Engine::BossRender()
{
	bossFrameTimer++;
	if (!bossDying)
	{
		if (bossFrameTimer >= 15)
		{
			bossFrame++;
			if (bossFrame >= 4)
			{
				bossFrame = 0;
			}
			m_bossSrc.x = bossFrame * 552;
			bossFrameTimer = 0;
		}
		SDL_RenderCopyEx(m_pRenderer, m_bossTexture, &m_bossSrc, &m_bossRect, 0, 0, m_bossFlip);
		if (bossActive)
		{
			SDL_RenderFillRect(m_pRenderer, new SDL_Rect{ 100, 600, 22 * m_bossHealth, 60 });
		}
	}
	else
	{
		SDL_RenderCopyEx(m_pRenderer, m_bossTexture, &m_bossSrc, &m_bossRect, 90, 0, m_bossFlip);
	}
}

void Engine::LaserRender()
{
	if (level == 3)
	{
		cout << laserTimer << endl;
		if (laserTimer < 25 && !bossDying)
		{
			if (m_laserFlip)
			{
				SDL_RenderCopyEx(m_pRenderer, m_laserSmallTexture, &m_laserSrc, &m_laserRect, 0, 0, SDL_FLIP_VERTICAL);

			}
			else
			{
				SDL_RenderCopyEx(m_pRenderer, m_laserSmallTexture, &m_laserSrc, &m_laserRect, 0, 0, SDL_FLIP_NONE);
			}
		}
		else if (laserTimer > 25 && !bossDying)
		{
			if (m_laserFlip)
			{
				SDL_RenderCopyEx(m_pRenderer, m_laserBigTexture, &m_laserSrc, &m_laserRect, 0, 0, SDL_FLIP_VERTICAL);

			}
			else
			{
				SDL_RenderCopyEx(m_pRenderer, m_laserBigTexture, &m_laserSrc, &m_laserRect, 0, 0, SDL_FLIP_NONE);
			}
		}
		//laser flips every frame
		m_laserFlip = !m_laserFlip;
	}
}