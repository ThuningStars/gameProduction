#pragma once
#ifndef FLYINGENEMY_H
#define FLYINGENEMY_H
#include"SDL.h"
#include <iostream>

#include "PlayerBullet.h"

using namespace std;


class FlyingEnemy : public Sprite
{
private:
	SDL_Rect m_src; // Source rectangle
	SDL_Rect m_rect;

	int m_angle = 0, m_speed;
	SDL_Point* m_pCenter = NULL;
	bool m_attack = false;
	SDL_RendererFlip m_flip = SDL_FLIP_NONE;
	int m_upX, m_upY, m_bottomX, m_bottomY, m_timer = 0;


public:
	FlyingEnemy(int, int, int, int);
	~FlyingEnemy();
	SDL_Rect* GetRect();// Source rectangle
	void Update();
	void Render(SDL_Renderer* rend);
	void Render(SDL_Renderer* rend, SDL_Texture* texture, SDL_RendererFlip flip);
	bool GetAttack() { return m_attack; }
	void SetAttack(bool a) { m_attack = a; }
};




#endif