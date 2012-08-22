#include "CSlasherGameManager.h"
#include "SVideoManager.h"
#include "SAudioManager.h"
#include "Slogger.h"

#include "CChunk.h"
#include "CSurface.h"
#include "CMusic.h"

#include "CBGround.h"
#include "CZero.h"
#include "CPlatform.h"
#include "CGameTimer.h"
#include "CAsteroid.h"
#include "CZUILifebar.h"

#include "constants.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace GD4N;
bool gameOn = false;

CSlasherGameManager::CSlasherGameManager() {
	windowTitle = (char *)"Zero Slasher";
	icon = (char *)"images/icon.bmp";
	
	
}

bool CSlasherGameManager::Init() {
	sVideo->SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	
	readHighScoreFile();

	AddScene(CSlasherGameManager::Scene00);
	ChangeScene(0);
	
	if(!CGameManager::Init()) return false;
	
	return true;
}

void CSlasherGameManager::LoadResources() {
	CChunk::Load((char*)"sfx/zero-jump_x4.ogg", SFXID_ZJUMP);
	CChunk::Load((char*)"sfx/zero-jump2_x4.ogg", SFXID_ZJUMP2);
	CChunk::Load((char*)"sfx/x-land.ogg", SFXID_ZLAND);
	CChunk::Load((char*)"sfx/zero-slash_x4.ogg", SFXID_ZSLASH1);
	CChunk::Load((char*)"sfx/zero-slash2_x4.ogg", SFXID_ZSLASH2);
	CChunk::Load((char*)"sfx/zero-slash3_x4.ogg", SFXID_ZSLASH3);
	CChunk::Load((char*)"sfx/zero-slash-silent.ogg", SFXID_ZSLASHAIR);
	CChunk::Load((char*)"sfx/zero-dash.ogg", SFXID_ZDASH);
	CChunk::Load((char*)"sfx/x-dashskid.ogg", SFXID_ZSKID);
	CChunk::Load((char*)"sfx/explode_weak.ogg", SFXID_ZSLASHHIT);
	CChunk::Load((char*)"sfx/zero_x4_hurt.ogg", SFXID_HURT);
	CChunk::Load((char*)"sfx/ready.ogg", SFXID_READY);
	CChunk::Load((char*)"sfx/step1.ogg", SFXID_STEP1);
	CChunk::Load((char*)"sfx/step2.ogg", SFXID_STEP2);
	CChunk::Load((char*)"sfx/explode_weak.ogg", SFXID_EXPLODE);

	sChunkPool->CleanUp();

	
	CMusic::Load((char*)"sfx/Megaman X4 Intro Stage.mp3", MUSICID_01);
	//CMusic::Load((char*)"music/02-Molly.mp3", MUSICID_02);
	sMusicPool->CleanUp();
	
	CSurface::Load((char *)"images/zeroslasherlogo.png", SURFID_SLASHERLOGO);
	CSurface::Load((char *)"images/platform.png", SURFID_PLATFORM);
	CSurface::Load((char *)"images/arrow.png", SURFID_ARROW);
	CSurface::Load((char *)"images/asteroid.png", SURFID_ASTEROID);
	CSurface::Load((char *)"images/lifebar_combo.png", SURFID_COMBOBAR);
	CSurface::Load((char *)"images/highscorelabel.png", SURFID_HIGHSCORELABEL);
	CSurface::Load((char *)"images/lifebar.png", SURFID_LIFEBAR);
	CSurface::Load((char *)"images/lifebar_diff.png", SURFID_LIFEBARDIFF);
	CSurface::Load((char *)"images/lifebar_white.png", SURFID_LIFEBARWHITE);
	CSurface::Load((char *)"images/zeroportraitbar.png", SURFID_ZEROPORTRAIT);
	CSurface::Load((char *)"images/lifebar_holder.png", SURFID_LIFECONTAINER);
	CSurface::Load((char *)"images/zeronumbers.png", SURFID_UINUMBERS);
	CSurface::Load((char *)"images/zerolargenumbers.png", SURFID_UILARGENUMBERS);
	CSurface::Load((char *)"images/zerohighscorenumbers.png", SURFID_UIHIGHSCORENUMBERS);
	CSurface::Load((char *)"images/explode.png", SURFID_EXPLODE);
	if(SHOW_DEBUG_NUMBERS) {
		CSurface::Load((char *)"images/bkgd_debug.jpg", SURFID_BKGD);
	} else {
		CSurface::Load((char *)"images/bkgd.jpg", SURFID_BKGD);
	}

	if(SHOW_SPRITE_NOTES) {
		CSurface::Load((char *)"images/Zero_debug.png", SURFID_ZERO);
		CSurface::Load((char *)"images/Zero_l_debug.png", SURFID_ZEROL);
	} else {
		CSurface::Load((char *)"images/Zero.png", SURFID_ZERO);
		CSurface::Load((char *)"images/Zero_l.png", SURFID_ZEROL);
	}

	sSurfacePool->CleanUp();
}

void CSlasherGameManager::Scene00() {
	CGameTimer* Ztimer = new CGameTimer();

	new CBGround();
	new CPlatform(PLATFORM_1X, PLATFORM_1Y);
	new CPlatform(PLATFORM_2X, PLATFORM_2Y);
	new CPlatform(PLATFORM_3X, PLATFORM_3Y);
	new CPlatform(PLATFORM_4X, PLATFORM_4Y);

	CAsteroid* aster = new CAsteroid(0);
	CAsteroid* aster2 = new CAsteroid(SECOND_ASTEROID_SPAWNDELAY);
	CAsteroid* aster3 = new CAsteroid(THIRD_ASTEROID_SPAWNDELAY);
	CAsteroid* aster4 = new CAsteroid(FOURTH_ASTEROID_SPAWNDELAY);
	CZero* zero = new CZero(this);
	CZUILifebar* lifebar = new CZUILifebar();

	//object communication
	aster->setLifeTarget(&zero->hitpoints);
	aster2->setLifeTarget(&zero->hitpoints);
	aster3->setLifeTarget(&zero->hitpoints);
	aster4->setLifeTarget(&zero->hitpoints);
	lifebar->setLifeSource(&zero->hitpoints);
	lifebar->setPointsSource(&zero->points);
}

void CSlasherGameManager::CleanUp() {
	writeHighScoreFile();
	CGameManager::CleanUp();
}

void CSlasherGameManager::readHighScoreFile(){
	using namespace std;
	string highScoreString;
	ifstream myFile ("highscore.dat");
	if (myFile.is_open()){
		while(myFile.good() ){
			getline (myFile,highScoreString);
		}
		myFile.close();
	}
	highScore = atoi(highScoreString.c_str() );
}

void CSlasherGameManager::writeHighScoreFile(){
	using namespace std;
	ofstream myFile; 
	myFile.open("highscore.dat");
	myFile << highScore;
	myFile.close();
}

void CSlasherGameManager::setHighScore(int recentScore){
	if(recentScore > highScore) 
		highScore = recentScore;
}