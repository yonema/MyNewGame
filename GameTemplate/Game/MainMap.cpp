#include "stdafx.h"
#include "MainMap.h"
#include "MapConstDatah.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "GameMainState.h"
#include "BGM.h"
#include "BackGround.h"
#include "AIField.h"
#include "Level3D.h"
#include "AICar.h"
#include "RenderingEngine.h"
#include "Fade.h"

namespace nsNinjaAttract
{
	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		using namespace nsMapConstData;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CMainMap::Start()
		{
			// ゲームステートの生成
			m_gameState = NewGO<nsGameState::CGameMainState>(
				nsCommonData::enPriorityFirst,
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainGameState]
				);

			// プレイヤーの初期化
			InitPlayer();

			// ゲームステートを初期化
			m_gameState->Init(*m_player);

			// AIフィールドクラスの生成
			m_aiField = new nsAI::CAIField();

			// 車の初期化
			InitCar();

			if (m_backGround == nullptr)
			{
				// 背景ステージが設定されていなければ、
				// 背景ステージを生成する。
				m_backGround = new nsBackGround::CBackGround;
			}
			// タイトル画面ではない
			m_backGround->SetIsTitle(false);

			// BGMクラスの生成と初期化
			m_bgm = NewGO<nsBGM::CBGM>(nsCommonData::enPriorityFirst);
			m_bgm->Init(*m_player);

			// フェードインを開始
			nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeIn();

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CMainMap::OnDestroy()
		{
			DeleteGO(m_player);
			DeleteGO(m_gameState);
			DeleteGO(m_bgm);

			QueryGOs<nsAICharacter::CAICar>(
				nsCommonData::kGameObjectName[nsCommonData::enGN_Car],
				[&](nsAICharacter::CAICar* car)->bool
				{
					// 車を全消去
					DeleteGO(car);
					return true;
				}
			);

			delete m_backGround;
			delete m_aiField;

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CMainMap::Update()
		{
			m_startFallTimer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_startFallTimer >= 5.0f)
			{
				// プレイヤーを入力可能にする
				m_player->SetIsInputtable(true);
			}

			return;
		}

		/**
		 * @brief プレイヤーの初期化
		*/
		void CMainMap::InitPlayer()
		{
			// プレイヤーの生成
			// 何か優先度下げないと、鎖の位置がずれる。
			m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPrioritySecond);
			// プレイヤーを入力不可にする
			m_player->SetIsInputtable(false);

			// プレイヤー用のレベルを生成
			m_playerLevel = std::make_unique<nsLevel3D::CLevel3D>();
			// レベルをロード。マップチップのインスタンスは生成しない。
			m_playerLevel->Init(
				kLevelFilePath[enLevelPlayer],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
					// ゲーム中のプレイヤーのレベル上の名前
					if (objData.EqualObjectName(kPlayerLevelNameInGame))
					{
						objData.position.y = 2500.0f;
						// ゲーム中のプレイヤーの座標と回転を設定
						m_player->SetPosition(objData.position);
						m_player->SetRotation(objData.rotation);
					}
					return true;
				}
			);

			return;
		}

		/**
		 * @brief 車の初期化
		*/
		void CMainMap::InitCar()
		{
			// 車用のレベルを生成
			m_carLevel = std::make_unique<nsLevel3D::CLevel3D>();
			// レベルをロード。マップチップのインスタンスは生成しない。
			m_carLevel->Init(
				kLevelFilePath[enLevelCar],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
#ifdef _DEBUG
					static int num = 0;
					if (num != 0)
					{
						return true;
					}
					num++;
#endif

					// 車を生成
					nsAICharacter::CAICar* car = NewGO<nsAICharacter::CAICar>(
						nsCommonData::enPriorityFirst, 
						nsCommonData::kGameObjectName[nsCommonData::enGN_Car]
						);

					// 車を初期化
					car->Init(
						m_aiField->GetAICharaInitData(),
						objData.position,
						objData.rotation,
						Vector3::One
					);

					return true;
				}
			);

			return;
		}
	}
}