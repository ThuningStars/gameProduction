#include "Engine.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

int playerHealth = 3;
int coolDown = 0;

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
					m_playerIdleTexture = IMG_LoadTexture(m_pRenderer, "../assets/player/idle.png");
					m_playerRunTexture = IMG_LoadTexture(m_pRenderer, "../assets/player/run.png");
					m_playerAttackTexture = IMG_LoadTexture(m_pRenderer, "../assets/player/attack.png");
					m_yellowEnemyWalkTexture = IMG_LoadTexture(m_pRenderer, "../assets/enemy/walking.png");
					m_yellowEenemyAttackTexture = IMG_LoadTexture(m_pRenderer, "../assets/enemy/Attack.png");
					m_redEnemyWalkTexture = IMG_LoadTexture(m_pRenderer, "../assets/enemy/red walking.png");
					m_groundTexture = IMG_LoadTexture(m_pRenderer, "../assets/textures/ground.png");
					heartTexture = IMG_LoadTexture(m_pRenderer, "../assets/HUD/heart.png");
					m_pBGTexture = IMG_LoadTexture(m_pRenderer, "../assets/background/clouds.png");
					m_obstacletexture = IMG_LoadTexture(m_pRenderer, "../assets/textures/Wood.png");
					m_titleScreen = IMG_LoadTexture(m_pRenderer, "../assets/textures/titlescreen.png");
					m_gameOverScreen = IMG_LoadTexture(m_pRenderer, "../assets/textures/gameOverScreen.png");
					m_winScreen = IMG_LoadTexture(m_pRenderer, "../assets/textures/winScreen.png");
					cout << "Fourth pass." << endl;

					if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
					{// Load the chunks into the Mix_Chunk vector.
						Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048); // Good for most games.
						Mix_AllocateChannels(16);
						m_pMusic = Mix_LoadMUS("../assets/Aud/limbo.mp3"); // Load the music track.
						if (m_pMusic == nullptr)
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
	Mix_PlayMusic(m_pMusic, -1); // Play. -1 = looping.
	Mix_VolumeMusic(50); // 0-MIX_MAX_VOLUME (128).
	m_running = true;

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

				if (event.key.keysym.sym == ' ' && m_player.isGrounded())
				{
					m_player.SetAccelY(-JUMPFORCE);
					m_player.SetGrounded(false);

					// change animation to running
					m_player.SetRunning(true);
				}
				if (event.key.keysym.sym == SDLK_l && m_start % 2 * 1 == 0)
				{
					// Spawn a right bullet
					m_playerbullet.push_back(new Bullet({ m_player.GetDstRect()->x + 60, m_player.GetDstRect()->y + 40 }));
					m_playerbullet.shrink_to_fit();
					cout << "New bullet vector capacity: " << m_playerbullet.capacity() << endl;
					m_player.setAttack(true);
				}

				if (event.key.keysym.sym == SDLK_k && m_start % 2 * 1 == 0)
				{
					// Spawn a left bullet
					m_playerleftbullet.push_back(new LeftBullet({ m_player.GetDstRect()->x + 0, m_player.GetDstRect()->y + 40 }));
					m_playerleftbullet.shrink_to_fit();
					cout << "New bullet vector capacity: " << m_playerleftbullet.capacity() << endl;
					m_player.setAttack(true);
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
	//I edited this section to work no matter how many platforms there are, @ Ryan on discord if it's confusing
	for (SDL_Rect x : m_Platforms)
	{
		if (SDL_HasIntersection(m_player.GetDstRect(), &x))
		{
			if ((m_player.GetDstRect()->y + m_player.GetDstRect()->h) - (float)m_player.GetVelY() <= x.y)
			{
				//colliding with the top side of platforms.
				m_player.SetGrounded(true);
				m_player.StopY();
				m_player.SetY(x.y - m_player.GetDstRect()->h);
			}
			else if (m_player.GetDstRect()->y - (float)m_player.GetVelY() >= (x.y + x.h))
			{
				//colliding with the bottom side of platforms.
				m_player.StopY();
				m_player.SetY(x.y + x.h);
			}
			else if ((m_player.GetDstRect()->x + m_player.GetDstRect()->w) - (float)m_player.GetVelX() <= x.x)
			{
				//colliding with the left side of platforms.
				m_player.StopX();
				m_player.SetX(x.x - x.w);
			}
			else if (m_player.GetDstRect()->x - (float)m_player.GetVelX() >= (x.x + x.w))
			{
				//colliding with the right side of platforms.
				m_player.StopX();
				m_player.SetX(x.x + x.w);
			}

		}
	}
	for (SDL_Rect x : m_Obstacles)
	{
		if (SDL_HasIntersection(m_player.GetDstRect(), &x))
		{
			if ((m_player.GetDstRect()->y + m_player.GetDstRect()->h) - (float)m_player.GetVelY() <= x.y)
			{
				//colliding with the top side of platforms.
				m_player.SetGrounded(true);
				m_player.StopY();
				m_player.SetY(x.y - m_player.GetDstRect()->h);
			}
			else if (m_player.GetDstRect()->y - (float)m_player.GetVelY() >= (x.y + x.h))
			{
				//colliding with the bottom side of platforms.
				m_player.StopY();
				m_player.SetY(x.y + x.h);
			}
			else if ((m_player.GetDstRect()->x + m_player.GetDstRect()->w) - (float)m_player.GetVelX() <= x.x)
			{
				//colliding with the left side of platforms.
				m_player.StopX();
				m_player.SetX(x.x - x.w);
			}
			else if (m_player.GetDstRect()->x - (float)m_player.GetVelX() >= (x.x + x.w))
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

	//Player right bullet - collisionto enemy
	for (unsigned i = 0; i < m_playerbullet.size(); i++)
	{
		for (unsigned a = 0; a < m_yellowEnemyCreation.size(); a++)
			if (SDL_HasIntersection(m_playerbullet[i]->GetRekt(), m_yellowEnemyCreation[a]->GetRect()))
			{

				cout << "Player bullet Collision!" << endl;
				/*Mix_PlayChannel(-1, explosion, 0);*/

				delete m_playerbullet[i]; // flag for reallocation
				m_playerbullet[i] = nullptr; // get rid of the dangling pointer
				m_playerbullet.erase(m_playerbullet.begin() + i);
				m_playerbullet.shrink_to_fit();
				cout << " Bullet Deleted \n";

				delete m_yellowEnemyCreation[a]; // flag for reallocation
				m_yellowEnemyCreation[a] = nullptr; // get rid of the dangling pointer
				m_yellowEnemyCreation.erase(m_yellowEnemyCreation.begin() + a);
				m_yellowEnemyCreation.shrink_to_fit();
				cout << " Enemy Deleted \n";

			}
	}//Player left bullet - collision to enemy
	for (unsigned i = 0; i < m_playerleftbullet.size(); i++)
	{
		for (unsigned a = 0; a < m_yellowEnemyCreation.size(); a++)
			if (SDL_HasIntersection(m_playerleftbullet[i]->GetRekt(), m_yellowEnemyCreation[a]->GetRect()))
			{

				cout << "Player bullet Collision!" << endl;
				/*Mix_PlayChannel(-1, explosion, 0);*/

				delete m_playerleftbullet[i]; // flag for reallocation
				m_playerleftbullet[i] = nullptr; // get rid of the dangling pointer
				m_playerleftbullet.erase(m_playerleftbullet.begin() + i);
				m_playerleftbullet.shrink_to_fit();
				cout << " Bullet Deleted \n";

				delete m_yellowEnemyCreation[a]; // flag for reallocation
				m_yellowEnemyCreation[a] = nullptr; // get rid of the dangling pointer
				m_yellowEnemyCreation.erase(m_yellowEnemyCreation.begin() + a);
				m_yellowEnemyCreation.shrink_to_fit();
				cout << " Enemy Deleted \n";

			}
	}

	//Here's the logic for checking if the player fell off the edge
	if (m_player.GetDstRect()->y > 1000)
	{
		cout << "You died!" << "/n";

		//where the player respawns
		m_player.SetX(462);
		m_player.SetY(600);
		playerHealth--;
	}
}

void Engine::Update()
{
	//checks if the game is being played
	if (gameState == 1)
	{
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
		}
		else if (KeyDown(SDL_SCANCODE_L))
		{
			// change animation to attack
			m_player.setAttack(true);
		}

		//wrap the player
		if (m_player.GetDstRect()->x < -51.0) m_player.SetX(1024.0);
		else if (m_player.GetDstRect()->x > 1024.0) m_player.SetX(-50.0);
		//Update the player
		/*m_player.Update();*/
		m_player.Update();
		if (m_player.GetDstRect()->x > (WIDTH / 3))
		{
			//m_Camera.x -= m_player.GetVelX();
			//m_Camera.x = -m_player.GetDstRect()->x+WIDTH/2;
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
		if (m_yellowEnemyCreation.size() == 0
			&& m_player.GetDstRect()->x > 950
			&& m_player.GetDstRect()->y < 200)
		{
			gameState = 3; //win state
		}
		if (playerHealth == 0 && gameState == 1)
		{
			gameState = 4; //game over state
		}
	}
	//On the title screen if the player pressese enter, it starts the game
	if (gameState == 0)
	{
		if (KeyDown(SDL_SCANCODE_RETURN) && enterPressed == false)
		{
			gameState = 1;

			//deletes all enemies and recreate them
			m_yellowEnemyCreation.clear();
			m_yellowEnemyCreation.shrink_to_fit();
			int x = 0;
			for (auto element : m_Platforms)
			{
				x++;
				if (x != 5)
					m_yellowEnemyCreation.push_back(new Enemy(element.x, element.y, element.x + element.w, element.y));

			}
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
	else if (gameState == 4) //lets the player return to the title screen from the game over screen
	{
		if (KeyDown(SDL_SCANCODE_RETURN) && enterPressed == false)
		{
			gameState = 0;
			enterPressed = true;
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

	SDL_RenderCopy(m_pRenderer, m_pBGTexture, &m_bg1.m_src, &m_bg1.m_dst);
	//Render Platforms
	SDL_SetRenderDrawColor(m_pRenderer, 192, 64, 0, 255);

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
		SDL_RenderCopy(m_pRenderer, m_winScreen, NULL, &m_bg1.m_dst);
	}
	else if (gameState == 1 || gameState == 2)
	{
		//todo: fill with ground texture instead
		for (SDL_Rect x : m_Platforms)
		{
			SDL_RenderFillRect(m_pRenderer, &x);
			//SDL_RenderCopyEx(m_pRenderer, m_groundTexture, &textureSrc, &m_Platforms[i], 0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopy(m_pRenderer, m_groundTexture, NULL, &x);
		}

		for (SDL_Rect x : m_Obstacles)
		{
			SDL_RenderFillRect(m_pRenderer, &x);
			//SDL_RenderCopyEx(m_pRenderer, m_groundTexture, &textureSrc, &m_Platforms[i], 0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopy(m_pRenderer, m_obstacletexture, NULL, &x);
		}

		for (int i = 0; i < m_playerbullet.size(); i++)
			m_playerbullet[i]->Render(m_pRenderer);

		for (int i = 0; i < m_playerleftbullet.size(); i++)
			m_playerleftbullet[i]->Render(m_pRenderer);

		for (unsigned i = 0; i < m_yellowEnemyCreation.size(); i++) // size() is actual filled numbers of elements
		{
			if (m_yellowEnemyCreation[i]->GetAttack() == false)
				m_yellowEnemyCreation[i]->Render(m_pRenderer, m_yellowEnemyWalkTexture, flipEnemy);
			else
				m_yellowEnemyCreation[i]->Render(m_pRenderer, m_yellowEenemyAttackTexture, flipEnemy);
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
	}
	SDL_RenderPresent(m_pRenderer); // Flip buffers - send data to window.
	// Any drawing here...


//	SDL_RenderPresent(m_pRenderer);
	//SDL_RenderPresent(m_pRenderer);

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
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}


