#include "FlyingEnemy.h"
#include "Enemy.h"

FlyingEnemy::FlyingEnemy(int upLimitX, int upLimitY, int bottomLimitX, int bottomLimitY)
{

	m_upX = upLimitX - 25;
	m_upY = upLimitY;
	m_bottomX = bottomLimitX - 25;
	m_bottomY = bottomLimitY;
	m_speed = -2;

	m_src = { 0,0,400,248 };

	this->m_rect.w = 100;
	this->m_rect.h = 62;
	this->m_rect.x = m_upX;
	this->m_rect.y = m_upY - m_rect.h;

	m_timer = 0;
	m_attack = false;
}

FlyingEnemy::~FlyingEnemy()
{
	cout << "De-allocating Enemy at " << &(*this) << endl;
}

void FlyingEnemy::Update()
{
	if (m_rect.y < m_bottomY)
	{
		m_speed = -m_speed;// m_speed;
		m_flip = SDL_FLIP_HORIZONTAL;

	}
	else if (m_rect.y > m_upY)
	{
		m_speed = -m_speed;
		m_flip = SDL_FLIP_NONE;
	}
	m_rect.y += m_speed;

	// animation
	m_timer++;
	if (FPS / m_timer == 6)
	{
		m_timer = 0;
		m_src.x += 400;
	}

	if (m_src.x == 2000)
		m_src.x = 0;
}
void FlyingEnemy::Render(SDL_Renderer* rend)
{
	SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
	SDL_RenderFillRect(rend, &m_rect);
}

void FlyingEnemy::Render(SDL_Renderer* rend, SDL_Texture* texture, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(rend, texture, &m_src, &m_rect, m_angle, m_pCenter, m_flip);
}


SDL_Rect* FlyingEnemy::GetRect()
{
	return &m_rect;
}
