#include "CSlasherGameManager.h"
#include "SVideoManager.h"
#include "SAudioManager.h"
#include "Slogger.h"

#include "CChunk.h"
#include "CSurface.h"

#include "CBGround.h"
#include "CZero.h"
#include "CPlatform.h"
#include "CGameTimer.h"
#include "CAsteroid.h"
#include "CZUILifebar.h"

#include "constants.h"

using namespace GD4N;

CSlasherGameManager::CSlasherGameManager() {
	windowTitle = (char *)"Zero Slasher";
}

bool CSlasherGameManager::Init() {
	sVideo->SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	AddScene(CSlasherGameManager::Scene00);
	ChangeScene(0);
	if(!CGameManager::Init()) return false;

	return true;
}

void CSlasherGameManager::LoadResources() {
	CChunk::Load((char*)"sfx/zero-jump_x4.wav", SFXID_ZJUMP);
	CChunk::Load((char*)"sfx/zero-jump2_x4.wav", SFXID_ZJUMP2);
	CChunk::Load((char*)"sfx/x-land.wav", SFXID_ZLAND);
	CChunk::Load((char*)"sfx/zero-slash_x4.wav", SFXID_ZSLASH1);
	CChunk::Load((char*)"sfx/zero-slash2_x4.wav", SFXID_ZSLASH2);
	CChunk::Load((char*)"sfx/zero-slash3_x4.wav", SFXID_ZSLASH3);
	CChunk::Load((char*)"sfx/zero-slash-silent.wav", SFXID_ZSLASHAIR);
	CChunk::Load((char*)"sfx/zero-dash.wav", SFXID_ZDASH);
	CChunk::Load((char*)"sfx/x-dashskid.wav", SFXID_ZSKID);
	CChunk::Load((char*)"sfx/explode_weak.wav", SFXID_ZSLASHHIT);
	CChunk::Load((char*)"sfx/zero_x4_hurt.wav", SFXID_HURT);
	CChunk::Load((char*)"sfx/ready.wav", SFXID_READY);

	sChunkPool->CleanUp();

	/*
	CMusic::Load((char*)"music/01-Molly.mp3", MUSICID_01);
	CMusic::Load((char*)"music/02-Molly.mp3", MUSICID_02);
	sMusicPool->CleanUp();
	*/
	CSurface::Load((char *)"images/Zero.png", SURFID_ZERO);
	CSurface::Load((char *)"images/Zero_l.png", SURFID_ZEROL);
	CSurface::Load((char *)"images/bkgd.jpg", SURFID_BKGD);
	CSurface::Load((char *)"images/platform.png", SURFID_PLATFORM);
	CSurface::Load((char *)"images/asteroid.png", SURFID_ASTEROID);
	CSurface::Load((char *)"images/lifebar.png", SURFID_LIFEBAR);
	CSurface::Load((char *)"images/lifebar_diff.png", SURFID_LIFEBARDIFF);
	CSurface::Load((char *)"images/lifebar_white.png", SURFID_LIFEBARWHITE);
	CSurface::Load((char *)"images/zeroportraitbar.png", SURFID_ZEROPORTRAIT);
	CSurface::Load((char *)"images/lifebar_holder.png", SURFID_LIFECONTAINER);
	CSurface::Load((char *)"images/zeronumbers.png", SURFID_UINUMBERS);
	sSurfacePool->CleanUp();
}

void CSlasherGameManager::Scene00() {
	new CGameTimer();
	new CBGround();
	new CPlatform(PLATFORM_1X, PLATFORM_1Y);
	new CPlatform(PLATFORM_2X, PLATFORM_2Y);
	new CPlatform(PLATFORM_3X, PLATFORM_3Y);
	new CPlatform(PLATFORM_4X, PLATFORM_4Y);
	new CAsteroid();
	new CZero();
	new CZUILifebar();
}
