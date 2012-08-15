#include "CSlasherGameManager.h"
#include "SVideoManager.h"
#include "SAudioManager.h"
#include "Slogger.h"
#include "CSurface.h"

#include "CBGround.h"
#include "CZero.h"

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
	/*
	CChunk::Load((char*)"sfx/eat.wav", SFXID_EAT);
	sChunkPool->CleanUp();

	CMusic::Load((char*)"music/01-Molly.mp3", MUSICID_01);
	CMusic::Load((char*)"music/02-Molly.mp3", MUSICID_02);
	sMusicPool->CleanUp();
	*/
	CSurface::Load((char *)"images/Zero.png", SURFID_ZERO);
	CSurface::Load((char *)"images/bkgd.bmp", SURFID_BKGD);
	CSurface::Load((char *)"images/platform.png", SURFID_PLATFORM);
	sSurfacePool->CleanUp();
}

void CSlasherGameManager::Scene00() {
	new CBGround();
	new CZero();
}
