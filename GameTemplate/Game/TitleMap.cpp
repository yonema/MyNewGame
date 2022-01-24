#include "stdafx.h"
#include "TitleMap.h"
#include "Player.h"
#include "GameMainState.h"
#include "BackGround.h"
#include "Level3D.h"
#include "SoundCue.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "BGMConstData.h"
#include "MainMap.h"
#include "GameTime.h"
#include "RenderingEngine.h"
#include "Fade.h"
#include "SavedPlayerInputDataConstData.h"

// 入力データを保存する
//#define SAVE_INPUTDATA


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
		bool CTitleMap::Start()
		{
			// ここ改造
			//constexpr float kWorldSoundVolume = 1.0f;
			constexpr float kWorldSoundVolume = 0.2f;
			nsSound::CSoundCue::SetBGMVolume(kWorldSoundVolume);
			nsSound::CSoundCue::SetSEVolume(kWorldSoundVolume);

			// ゲームステートの生成
			m_gameState = NewGO<nsGameState::CGameMainState>(
				nsCommonData::enPriorityFirst,
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainGameState]
				);

			// プレイヤーの初期化
			InitPlayer();

			// ゲームステートを初期化
			m_gameState->Init(*m_player);

			// サウンドの初期化
			InitSound();

			// スプライトの初期化
			InitSprite();

			// 背景ステージの生成
			m_backGround = new nsBackGround::CBackGround();
			// タイトル画面
			m_backGround->SetIsTitle(true);

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CTitleMap::OnDestroy()
		{
			DeleteGO(m_gameState);
			DeleteGO(m_player);
			DeleteGO(m_bgmSC);
			DeleteGO(m_decisionSC);
			DeleteGO(m_titleSR);
			DeleteGO(m_titleStartSR);

			if (m_backGround)
			{
				// 背景ステージクラスの参照をまだ所有していたら、破棄する。
				delete m_backGround;
			}

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CTitleMap::Update()
		{
			// スプライトの更新
			UpdateSprite();

			return;
		}

		/**
		 * @brief プレイヤーの初期化
		*/
		void CTitleMap::InitPlayer()
		{
			// プレイヤーの生成
			// 何か優先度下げないと、鎖の位置がずれる。
			m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPrioritySecond);
			// プレイヤーをタイトルモードにする
			m_player->TitleMode();

			// プレイヤー用のレベルを生成
			m_playerLevel = std::make_unique<nsLevel3D::CLevel3D>();
			// レベルをロード。マップチップのインスタンスは生成しない。
			m_playerLevel->Init(
				kLevelFilePath[enLevelPlayer],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
					// タイトル中のプレイヤーのレベル上の名前
					if (objData.EqualObjectName(kPlayerLevelNameInTitle))
					{
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
		 * @brief サウンドの初期化
		*/
		void CTitleMap::InitSound()
		{
			// BGMのサウンドの初期化
			m_bgmSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_bgmSC->Init(
				nsBGM::nsBGMConstData::kBGMSoundFilePaht[nsBGM::nsBGMConstData::enBT_Swing],
				nsSound::CSoundCue::enBGM
			);

			// 決定音のサウンドの初期化
			m_decisionSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_decisionSC->Init(
				kDecisionSoundFilePath,
				nsSound::CSoundCue::enBGM
			);

			return;
		}

		/**
		 * @brief スプライトの初期化
		*/
		void CTitleMap::InitSprite()
		{
			// タイトルのスプライトの生成と初期化
			m_titleSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_titleSR->Init(
				kTitleSpriteFilePath,
				kTitleSpriteWidth,
				kTitleSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			// スタートのスプライトの生成と初期化
			m_titleStartSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_titleStartSR->Init(
				kTitleStartSpriteFilePath,
				kTitleStartSpriteWidth,
				kTitleStartSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			// 下から登ってくるように動かすため、Y座標は最初は低めにしておく
			Vector2 titlePos = kTitleSpritePosition;
			titlePos.y = kTitleSpriteInitialHeight;
			// 座標とアルファ値を設定。最初は透明。
			m_titleSR->SetPosition(titlePos);
			m_titleSR->SetAlphaValue(0.0f);
			m_titleStartSR->SetPosition(kTitleStartSpritePosition);
			m_titleStartSR->SetAlphaValue(0.0f);

			return;
		}


		/**
		 * @brief スプライトの更新
		*/
		void CTitleMap::UpdateSprite()
		{
			// タイマーを進める
			m_timer += nsTimer::GameTime().GetFrameDeltaTime();

			// ステートによって処理を振り分ける
			switch (m_state)
			{
			case enTS_beforeFadeIn:
				// フェードイン前の更新
				UpdateBeforeFadeIn();
				break;
			case enTS_fadeIn:
				// フェードインの更新
				UpdateFadeIn();
				break;
			case enTS_titleIn:
				// タイトルインの更新
				UpdateTitleIn();
				break;
			case enTS_titleIdle:
				// タイトルアイドルの更新
				UpdateTitleIdle();
				break;
			case enTS_titleOut:
				// タイトルアウトの更新
				UpdateTitleOut();
				break;
			case enTS_swingDirecting:
				// スイングの演出の更新
				UpdateSwingDirecting();
				break;
			case enTS_fadeOut:
				// フェードアウトの更新
				UpdateFadeOut();
				break;
			}


			return;
		}


		/**
		 * @brief フェードイン前の更新
		*/
		void CTitleMap::UpdateBeforeFadeIn()
		{
			// フェードイン開始
			nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeIn();
			// BGMを再生
			m_bgmSC->Play(true);
			// ステートをフェードインに遷移
			ChangeState(enTS_fadeIn);

			return;
		}

		/**
		 * @brief フェードインの更新
		*/
		void CTitleMap::UpdateFadeIn()
		{
			if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd())
			{
				// フェードインが終わったら、ステートをタイトルインへ遷移
				ChangeState(enTS_titleIn);
			}

			return;
		}

		/**
		 * @brief タイトルインの更新
		*/
		void CTitleMap::UpdateTitleIn()
		{
			// タイマーの進み率
			float rate = min(1.0f, m_timer / kTitleInTime);
			// タイトルの座標
			Vector2 titlePos = kTitleSpritePosition;
			// だんだんと上に上がっていく
			titlePos.y = Math::Lerp<float>(rate, kTitleSpriteInitialHeight, kTitleSpritePosition.y);

			// 座標とアルファ値を設定。アルファ値は徐々に不透明になっていく。
			m_titleSR->SetPosition(titlePos);
			m_titleSR->SetAlphaValue(rate);

			if (m_timer >= kTitleInTime)
			{
				// タイマーが一定時間を過ぎたら、ステートをタイトルアイドル状態へ遷移
				ChangeState(enTS_titleIdle);
			}
			return;
		}

		/**
		 * @brief タイトルアイドルの更新
		*/
		void CTitleMap::UpdateTitleIdle()
		{
			// タイマーの進み率。0.0f～1.0fの範囲から、0.0f～2.0fの範囲に変換。
			float rate = min(2.0f, m_timer / (kTitleIdleLoopTime * 0.5f));
			// アルファ値
			float alphaValue = 0.0f;
			if (rate < 1.0f)
			{
				// タイマーの進み率が0.0f～1.0fの時は、だんだんと不透明になっていく。
				alphaValue = rate;
			}
			else
			{
				// タイマーの進み率が1.0f～2.0fの時は、だんだんと透明になっていく。
				alphaValue = 1.0f - (rate - 1.0f);
			}

			// スタートのスプライトのアルファ値を設定
			m_titleStartSR->SetAlphaValue(alphaValue);

			if (m_timer >= kTitleIdleLoopTime)
			{
				// タイマーが一定時間過ぎたら、リセット。
				m_timer = 0.0f;
			}

			if (g_pad[0]->IsTrigger(enButtonA))
			{
				// Aボタンが押されたら、ステートをタイトルアウトに遷移
				ChangeState(enTS_titleOut);
				// 決定音を再生
				m_decisionSC->Play(false);
			}
			return;
		}


		/**
		 * @brief タイトルアウトの更新
		*/
		void CTitleMap::UpdateTitleOut()
		{
			// タイマーの進み率
			float rate = min(1.0f, m_timer / kTitleOutTime);
			// アルファ値。だんだんと小さくなる。
			float alphaValue = 1.0f - rate;

			// スプライトのアルファ値を設定。だんだんと透明になる。
			m_titleSR->SetAlphaValue(alphaValue);
			m_titleStartSR->SetAlphaValue(alphaValue);

			if (m_timer >= kTitleOutTime)
			{
				// タイマーが一定時間を過ぎたら、ステートをスイング演出に遷移
				ChangeState(enTS_swingDirecting);
			}

			return;
		}


		/**
		 * @brief スイングの演出の更新
		*/
		void CTitleMap::UpdateSwingDirecting()
		{
#ifdef SAVE_INPUTDATA

			if (g_pad[0]->IsTrigger(enButtonStart))
			{
				// startボタンで、入力情報の保存を終了する。
				m_player->EndSaveInputDataAndSave(
					nsExternalData::nsSavedPlayerInputDataConstData::enST_titleDirecting
				);
				// ステートをフェードアウトに遷移
				ChangeState(enTS_fadeOut);
			}
#else
			if (m_player->IsEndLoadDataProgress())
			{
				// ロードした入力情報が最後まで進んだら、
				// ロードした入力情報の使用を終了する。
				m_player->EndUsingSavedInputData();

				// ステートをフェードアウトに遷移
				ChangeState(enTS_fadeOut);
			}
#endif



			return;
		}

		/**
		 * @brief フェードアウトの更新
		*/
		void  CTitleMap::UpdateFadeOut()
		{
			if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd() != true)
			{
				// フェードが終了するまで、徐々にBGMを小さくしていく。
				m_bgmSC->SetVolume(
					1.0f - 1.0f * nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->GetFadeRate()
				);
				// 早期リターン
				return;
			}

			// 完全の音量を0にする
			m_bgmSC->SetVolume(0.0f);

			// メインマップに遷移
			ChangeToMainMap();

			return;
		}

		/**
		 * @brief メインマップに遷移
		*/
		void CTitleMap::ChangeToMainMap()
		{

			// メインマップを生成
			CMainMap* mainMap = NewGO<CMainMap>(
				nsCommonData::enPriorityFirst,
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainMap]
				);
			// 背景ステージクラスの参照を譲渡する
			mainMap->SetBackGround(m_backGround);
			// 参照を捨てる
			m_backGround = nullptr;
			// 自信を破棄する。
			DeleteGO(this);


			return;
		}

		/**
		 * @brief ステート遷移
		 * @param[in] newState 新しいステート
		*/
		void CTitleMap::ChangeState(const nsMapConstData::EnTitleState newState)
		{
			if (m_state == newState)
			{
				return;
			}

			m_state = newState;
			m_timer = 0.0f;

			switch (m_state)
			{
			case enTS_beforeFadeIn:
				break;
			case enTS_fadeIn:
				break;
			case enTS_titleIn:
				break;
			case enTS_titleIdle:
				break;
			case enTS_titleOut:
				break;
			case enTS_swingDirecting:
#ifdef SAVE_INPUTDATA
				// 入力情報の保存を開始する
				m_player->StartSaveInputData();
				// プレイヤーの入力を有効にする
				m_player->SetIsInputtable(true);
#else
				// 保存してある入力情報の使用を開始する
				m_player->StartUsingSavedInputData(
					nsExternalData::nsSavedPlayerInputDataConstData::enST_titleDirecting
				);
#endif
				break;
			case enTS_fadeOut:
				// フェードアウトを開始
				nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeOut();
				break;
			}

			return;
		}


	}
}