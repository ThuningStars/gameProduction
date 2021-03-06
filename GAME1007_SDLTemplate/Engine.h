#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "SDL.h"
#include <iostream>
#include <string>
#include <SDL_mixer.h>
#include <vector>


#include "Collectible.h"
#include "PlatformPlayer.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define LEVEL_WIDTH 1500
#define LEVEL_HEIGHT 768


using namespace std;

class Engine
{
private: // private properties.
	bool m_running = false, pPressed = false, enterPressed = false, collided = false;
	Uint32 m_start, m_end, m_delta, m_fps, score = 0, gameState = 0;
	int m_shottimer = 0;
	int level = 1;
	// gameState 0 = title state, 1 = gameplay state, 2 = game paused state, 3 = win state, 4 = game over state
	string scoreString;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;		// FIRTS
	SDL_Rect m_Platforms[9];
	SDL_Rect m_PlatformsLevelOne[4] = {



		// third
		 {0,300,140,20}, // TOP LEFT GROUND


		//four
		 {200,170,250,20},
		 //FIFT


		 {0,720,300,50},// Ground left

			{960, 720, 70, 50}, // top down right








	}; //Position X and y, width and height
	SDL_Rect m_PlatformsLevelTwo[9] = {



		// 0 
		 {180,300,200,20},
		 // 1 
		  {630,300,200,20},
		  // 2 
		  {580,650,120,20},// Ground left 
		  //3 
		 {240, 650, 120, 20}, // top down right 

		 //4 
		 {70, 400, 50, 20},
		 //5 
		 {900, 400, 50, 20},
		 //6 
		 {130,430, 50,20},
		 //7 
		 {600,180,50,20},
		 //8
		 {380,180,50,20}




	}; //Position X and y, width and height 
	SDL_Rect m_ObstaclesLevelTwo[20]
	{
		//spaning point 
		{400,750,150,20},
		//GOAL 
		{450,100,120,20},
	};

	SDL_Rect m_Obstacles[20];
	SDL_Rect m_ObstaclesLevelOne[20]
	{
		//First 
	 {0,570,170,20},////first jump ground + enemies 
	{0,470,50	,100},// first jump ground WALL 
	{50,520,60	,70}, //first jump ground OBSTACLE - REMEMBER CHANGE TEXTURES OF OBSTACLES 
	//SECOND 
	{210,440,120,20},// 2 jump ground 
	{330,440,100,20},// 2 jump ground 
	{430,240,20,220},// 2 jump WALL 
	{230,325,30,30},// 2 jump OBSTACLE 
	{310,365,30,30},
	{400,410,30,30},// 2 jump OBSTACLE 

	//third 
	{120,210,20,20}, // TOP LEFT GROUND JUMP OBSTACLE 

	//FIFT 
	{640,340,70,20},
	{750,240,20,20},
	{850,190,20,20},


	//GROUND 
	{400,650,100,20},//initial structure 

	{200,670,50,50},// obstacle Ground left 
	{600, 720, 200, 50}, // Ground right 
	{600, 680, 100, 50},//Ground right second level 
	{600, 630, 50, 50},// Ground right 
	{600, 530, 40, 40},

	//GOAL 
							{940,100,120,20},// goal 
	};

	SDL_Rect m_ObstaclesLevelThree[4]
	{
		{100, 300, 30, 30},
		{130, 390, 30, 30},
		{160, 480, 30, 30},
		{190, 570, 30, 30}
	};

	SDL_Rect m_signOne = { 422,600,35,50 }; //the level one sign position 
	SDL_Rect m_levelOneTextPosition = { 300,300,400,250 }; //the text that appears when you're near the level one sign 
	//size of the ground texture for rendering 
	SDL_Rect textureSrc = { 0, 0, 225, 225 }; //NOT CURRENTLY USING THIS VARIABLE 

	Mix_Music* m_pMusic, * m_pMenuMusic;
	Mix_Chunk* m_pBullet, * m_pCheer, * m_pGameOver, * m_pJump, * m_pEnemy, * m_pWin;
	vector<Mix_Chunk*> m_vSounds;
	//SDL_Rect m_Camera = { 100, 0, WIDTH, HEIGHT }; 
	PlatformPlayer m_player;
	vector<Bullet*> m_playerbullet;
	vector<LeftBullet*> m_playerleftbullet;
	SDL_Texture* m_playerIdleTexture, * m_playerRunTexture, * m_playerAttackTexture, * m_groundTexture, * m_obstacletexture;
	SDL_Texture* m_yellowEnemyWalkTexture, * m_redEnemyWalkTexture, * m_yellowEenemyAttackTexture, * m_playerBullet;

	SDL_Texture* m_flyEnemyTexture, *m_bossTexture, *m_laserSmallTexture, *m_laserBigTexture, *m_bossBackground;
	
	SDL_Texture* heartTexture, * m_pBGTexture, * m_titleScreen, *m_gameOverScreen, *m_winScreen,* m_pCupTexture, *m_goodWinScreen, * m_pgoal, *m_signTexture, *m_levelOneText;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	SDL_RendererFlip flipEnemy = SDL_FLIP_NONE;
	Sprite m_yellowEnemy, m_bg1;
	vector<FlyingEnemy*> m_flyingEnemyCreation;
	vector<Enemy*> m_yellowEnemyCreation, m_yellowEnemyLevelOne;
	int m_EnemyTimer, levelOneSpawnX = 462, levelOneSpawnY = 600;

	int timer = 0;
	int timer2 = 0;
	int timer3 = 0;
	// currentLevel is the level currently being played 

	Collectible* m_pCollectible1, * m_pGoal, * m_pGoalLevelOne, * m_pCollectibleLevelOne, *m_pCollectible2, *m_pCollectible3;
	Collectible* m_pCollectibleLVL2_1, * m_pCollectibleLVL2_2, * m_pCollectibleLVL2_3, * m_pGoalLvl2;

	// Textures 
	SDL_Texture* m_pTexture;

	//stuff for the boss
	SDL_Rect m_bossSrc = {0, 0, 552, 536}; //boss Source rectangle
	SDL_Rect m_laserSrc = { 0, 0, 1000, 64 };
	SDL_Rect m_bossRect = {1200, 0, 350, 350}, m_laserRect; //boss dimensions

	bool bossGoingDown = true, bossActive = false, m_laserFlip = true, laserOnScreen, bossDying;
	SDL_RendererFlip m_bossFlip = SDL_FLIP_HORIZONTAL;
	int m_bossTimer = 0, laserTimer = 0, laserY = 0, m_bossHealth = 30, bossFrameTimer = 0, bossFrame = 0;


private: // private method prototypes. 
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	bool KeyDown(const SDL_Scancode c);
	void CheckCollision();
	void Update();
	void Render();
	void Sleep();
	void LevelInitialize(int level);
	void move_platforms();
	void BossUpdate();
	void BossRender();
	void LaserRender();

public: // public method prototypes. 
	int Run();
};

#endif 
