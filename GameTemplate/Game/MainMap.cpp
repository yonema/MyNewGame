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
#include "GameMainStateConstData.h"
#include "TitleMap.h"
#include "SoundCue.h"

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

			// 決定音のサウンドの生成と初期化
			m_decisionSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_decisionSC->Init(kDecisionSoundFilePath, nsSound::CSoundCue::enSE);
			// 開始ボイスのサウンドの生成と初期化
			m_startVoiceSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_startVoiceSC->Init(kStartVoiceSoundFilePath, nsSound::CSoundCue::enSE);
			m_startVoiceSC->SetVolume(kStartVoiceSoundVolume);

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

			DeleteGO(m_decisionSC);
			DeleteGO(m_startVoiceSC);

			if (m_backGround)
			{
				// 背景ステージクラスの参照をまだ所有していたら、破棄する。
				delete m_backGround;
			}
			delete m_aiField;

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CMainMap::Update()
		{
			// ゲームステートに応じて処理を振り分ける
			switch (m_gameState->GetGameMainStateState())
			{
			case nsGameState::nsGameMainStateConstData::enGS_startDirecting:
				// 開始演出の更新
				UpdateStartDirecting();

				break;
			case nsGameState::nsGameMainStateConstData::enGS_inGame:
				// ゲーム中の更新
				UpdateInGame();

				break;
			case nsGameState::nsGameMainStateConstData::enGS_beforeClearDirecting:
				break;
			case nsGameState::nsGameMainStateConstData::enGS_startFadeOutToClearDirecting:
				// クリア演出の前に、フェードアウトを行う。
				nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeOut(kFadeOutToClearDirectingTime);
				// ゲームステートをクリア演出の前のフェードアウト中に遷移
				m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_fadeOutToClearDirecting);
				break;
			case nsGameState::nsGameMainStateConstData::enGS_fadeOutToClearDirecting:
				// クリア演出の前のフェードアウトの更新
				UpdateFadeOutToClearDirecting();

				break;
			case nsGameState::nsGameMainStateConstData::enGS_clearDirecting:
				break;
			case nsGameState::nsGameMainStateConstData::enGS_result:
				break;
			case nsGameState::nsGameMainStateConstData::enGS_lastJump:
				// 最後のジャンプの更新
				UpdateLastJump();

				break;
			case nsGameState::nsGameMainStateConstData::enGS_goTitle:
				// タイトルへ遷移の更新
				UpdateGoTitle();

				break;
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
#ifdef _DEBUG
			int carNumForDebug = 0;
#endif

			// 車の総数をリセットする
			nsAICharacter::CAICar::ResetCarTortalNumber();

			// 車用のレベルを生成
			m_carLevel = std::make_unique<nsLevel3D::CLevel3D>();
			// レベルをロード。マップチップのインスタンスは生成しない。
			m_carLevel->Init(
				kLevelFilePath[enLevelCar],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
#ifdef _DEBUG
					if (carNumForDebug != 0)
					{
						return true;
					}
					carNumForDebug++;
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

		/**
		 * @brief 開始演出の更新
		*/
		void CMainMap::UpdateStartDirecting()
		{
			// 演出用のタイマーを進める
			m_directingTimer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_directingTimer >= kStartDirectingTime)
			{
				// タイマーが開始演出のタイムを超えたら

				// タイマーをリセット
				m_directingTimer = 0.0f;
				// プレイヤーを入力可能にする
				m_player->SetIsInputtable(true);
				// ミッションを表示する
				m_gameState->ShowMission();
				// ゲームステートをゲーム中に遷移
				m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_inGame);
				m_startVoiceSC->Play(false);
			}

			return;
		}

		/**
		 * @brief ゲーム中の更新
		*/
		void CMainMap::UpdateInGame()
		{
			if (m_player->GetState() == nsPlayer::nsPlayerConstData::enOnEnemy)
			{
				// プレイヤーのステートが、敵の上にいる状態のとき、何もしない。早期リターン。
				return;
			}

			// プレイヤーのステートが、敵の上にいる状態ではないとき


			if (g_pad[0]->IsTrigger(enButtonStart))
			{
				// startボタンを押したら、ミッションを表示
				m_gameState->ShowMission();
			}

			return;
		}


		/**
		 * @brief クリア演出の前のフェードアウトの更新
		*/
		void CMainMap::UpdateFadeOutToClearDirecting()
		{
			if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd())
			{
				// フェードアウトが終了したら

				// フェードインを行う
				nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeIn(kFadeInToClearDirectingTime);
				// ゲームステートをクリア演出へ遷移
				m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_clearDirecting);

				// リザルトのサウンドを再生する
				m_bgm->PlayResultSound();
			}

			return;
		}

		/**
		 * @brief 最後のジャンプの更新
		*/
		void CMainMap::UpdateLastJump()
		{
			// プレイヤーを入力可能にする
			m_player->SetIsInputtable(true);

			if (g_pad[0]->IsTrigger(enButtonA))
			{
				// Aボタンが押されたら

				// 決定音をワンショット再生
				m_decisionSC->Play(false);
				// ゲームステートをタイトルへ移動に遷移
				m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_goTitle);
			}
			return;
		}

		/**
		 * @brief タイトルへ遷移の更新
		*/
		void CMainMap::UpdateGoTitle()
		{
			// 演出用のタイマーを進める
			m_directingTimer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_directingTimer > kFadeOutGoTitleMinActiveTime && m_directingTimer <= kFadeOutGoTitleMaxActiveTime)
			{
				// タイマーがタイトルへ遷移するためのフェードアウトの有効タイム
				if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd() == true)
				{
					// フェード中ではなかったら、フェードアウト開始
					nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeOut(kFadeOutGoTitleTime);
				}
			}

			if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd() != true)
			{
				// フェード中なら

				// フェードの進行率に応じて、リザルトのBGMの音量を下げていく
				const float volume = nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->GetFadeRate();
				m_bgm->SetResultSoundVolume(volume);
			}

			if (m_directingTimer < kGoTitleDirectingTime)
			{
				// タイマーがタイトルへ行くための演出タイムより小さい。
				// まだタイトルへ行かない。早期リターン。
				return;
			}

			// bgmを音量を完全に消す
			m_bgm->SetResultSoundVolume(0.0f);

			// タイトルマップを生成
			CTitleMap* titleMap = NewGO<CTitleMap>(
				nsCommonData::enPriorityFirst,
				nsCommonData::kGameObjectName[nsCommonData::enGN_TitleMap]
				);
			// 背景ステージクラスの参照を譲渡する
			titleMap->SetBackGround(m_backGround);

			// 参照を捨てる
			m_backGround = nullptr;
			// 自信を破棄する。
			DeleteGO(this);
			return;
		}
	}
}