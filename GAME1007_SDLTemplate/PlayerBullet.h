#pragma once
#ifndef _PLAYER_BULLET_
#define _PLAYER_BULLET_

#include "SDL.h"
#include "SDL_image.h"
#include <SDL_mixer.h>
#include <sstream>
#include <iostream>
#include <ctime>
#include <vector>
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;

class Sprite
{
private:
	int m_frame = 0, // Frame counter.
		m_frameMax = 60, // Number of frames to display each sprite.
		m_sprite = 0, // Which sprite of the animation to display
		m_spriteMax = 8; // Number of sprites in the animation

public:

	SDL_Rect m_src; // Source rectangle
	SDL_Rect m_dst; // Destiniation rectangle

	void SetRekts(const SDL_Rect s, const SDL_Rect d)
	{
		m_src = s;
		m_dst = d;
	}
	SDL_Rect* GetSrc() { return &m_src; }
	SDL_Rect* GetDst() { return &m_dst; }

	void Animate()
	{
		if (m_frame++ % m_frameMax == 0)
			m_src.x = m_src.w * (m_sprite++ % (m_spriteMax)); // 128 * <our sprite ctr>

	}

	void Update()
	{
		this->m_dst.x += 5;
	}


};

class Bullet : public Sprite
{
private:
public:

	Bullet(SDL_Point spawnLoc = { 512, 384 })
	{
		m_src = { 0,0,32,32 };
		this->m_dst.x = spawnLoc.x; // this-> is optional.
		this->m_dst.y = spawnLoc.y; // this-> is optional.
		this->m_dst.w = 8;
		this->m_dst.h = 8;
	}
	~Bullet() // Destructor.
	{
		/*cout << "De-allocating Bullet at " << &(*this) << endl;*/
	}
	void SetLoc(SDL_Point newloc)
	{
		m_dst.x = newloc.x;
		m_dst.y = newloc.y;
	}
	SDL_Rect* GetRekt() { return &m_dst; }
	void Update()
	{
		m_dst.x += 5; // Move the bullet "to the side" 7 pixels every frame.
	}

};

class LeftBullet : public Sprite
{
private:
public:

	LeftBullet(SDL_Point spawnLoc = { 512, 384 })
	{
		m_src = { 0,0,32,32 };
		this->m_dst.x = spawnLoc.x; // this-> is optional.
		this->m_dst.y = spawnLoc.y; // this-> is optional.
		this->m_dst.w = 8;
		this->m_dst.h = 8;
	}
	~LeftBullet() // Destructor.
	{
		/*cout << "De-allocating Bullet at " << &(*this) << endl;*/
	}
	void SetLoc(SDL_Point newloc)
	{
		m_dst.x = newloc.x;
		m_dst.y = newloc.y;
	}
	SDL_Rect* GetRekt() { return &m_dst; }
	void Update()
	{
		m_dst.x -= 5; // Move the bullet "to the side" 7 pixels every frame.
	}



};

#endif