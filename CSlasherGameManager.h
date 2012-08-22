#ifndef _CSlasherGameManager_h
#define _CSlasherGameManager_h

#include "CGameManager.h"
#include "TSingleton.h"

class CSlasherGameManager :  public GD4N::CGameManager {
	friend class GD4N::TSingleton<CSlasherGameManager>;
	
protected:
	CSlasherGameManager();
	bool Init();
	void LoadResources();
	void CleanUp();

	void readHighScoreFile();
	void writeHighScoreFile();

	void Scene00();
public:
	void setHighScore(int recentScore);
	int highScore;
};

#define sGameManager GD4N::TSingleton< CSlasherGameManager >::GetInstance()

#endif