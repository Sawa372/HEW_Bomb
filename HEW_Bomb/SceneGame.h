#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "Ground.h"
#include "Goal.h"
#include "Camera.h"
#include "Model.h"
#include "Map.h"
#include "MapRenderer.h"
#include "Player.h"         
#include "GridCollision.h"  


class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();
	void Update() final;
	void Draw() final;

private:
	Ground* m_pGround; // 地面オブジェクト
	Goal* m_pGoal; // ゴールオブジェクト
	Camera* m_pCamera; // ゲーム中に動かすカメラ
	
	Map m_map; // マップ
	MapRenderer m_mapRenderer;// モデル軍
	Player m_player; // プレイヤー

};

#endif // __SCENE_GAME_H__