#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "Player.h"
#include "GameMainState.h"
#include "AICar.h"
#include "SpriteRender.h"
#include "PlayerCommandInput.h"
#include "SoundCue.h"
#include "GameMainState.h"

namespace nsNinjaAttract
{

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		using namespace nsPlayerConstData::nsCatchEnemyConstData;


		/**
		 * @brief コンストラクタ
		*/
		CPlayerCatchEnemy::CPlayerCatchEnemy()
		{
			// タイマーバーのスプライトの初期化
			InitTimerBarSprite();

			// QTEに使うボタンのスプライトの初期化
			InitQTEButtonSprite();

			// 忍術のエフェクトを初期化
			InitNinjyutuEffect();

			// サウンドの初期化
			InitSound();

			// コマンド入力クラスを生成する
			m_commandInput = std::make_unique<CPlayerCommandInput>();

			// 非決定的な乱数生成器でシード生成機を生成
			std::random_device rnd;
			// メルセンヌツイスターの32ビット版、引数は初期シード
			m_mt = std::make_unique<std::mt19937>(rnd());
			// 範囲の一様乱数
			m_fireVoiceRand = std::make_unique<std::uniform_int_distribution<>>(0, kFireVoiceSoundTypeNum - 1);
			m_failureVoiceRand = std::make_unique<std::uniform_int_distribution<>>(0, kFailureVoiceSoundTypeNum - 1);

			return;
		}


		/**
		 * @brief デストラクタ
		*/
		CPlayerCatchEnemy::~CPlayerCatchEnemy()
		{
			DeleteGO(m_onEnemyTimerBar);
			DeleteGO(m_onEnemyTimerBarFrame);

			for (auto& qteButtonSR : m_QTEButtonSRs)
			{
				DeleteGO(qteButtonSR);
			}

			DeleteGO(m_QTEButtonFraneSR);
			DeleteGO(m_QTEButtonAfterImageSR);
			for (auto& qteResultSR : m_QTEResultSR)
			{
				DeleteGO(qteResultSR);
			}
			DeleteGO(m_QTEResultFrameIn);
			DeleteGO(m_QTEResultFrameOut);

			DeleteGO(m_ninjyutuEF);
			DeleteGO(m_sonarEF);

			DeleteGO(m_commandSuccessSC);
			DeleteGO(m_commandMissSC);
			DeleteGO(m_windowOpneSC);
			DeleteGO(m_windowCloseSC);
			DeleteGO(m_fireStartSC);
			DeleteGO(m_fireReleaseSC);
			DeleteGO(m_sonarSC);

			for (int i = 0; i < kFireVoiceSoundTypeNum; i++)
			{
				DeleteGO(m_fireVoiceSC[i]);
			}
			for (int i = 0; i < kFailureVoiceSoundTypeNum; i++)
			{
				DeleteGO(m_failureVoiceSC[i]);
			}

			return;
		}

		/**
		 * @brief 初期化処理
		 * @param[in,out] playerRef プレイヤーの参照
		*/
		void CPlayerCatchEnemy::Init(CPlayer* playerRef)
		{
			m_playerRef = playerRef;

			m_commandInput->Init(*m_playerRef);


			// ゲームステートの参照を引っ張ってくる
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainGameState]
				);

			// 車たちの参照を引っ張ってくる
			m_aiCarsRef = m_gameState->GetAICar();

			return;
		}

		/**
		 * @brief レイヤーが敵を捕まえる処理クラスのアップデートを実行
		*/
		void CPlayerCatchEnemy::ExecuteUpdate()
		{
			// いつでも共通で行われる更新
			CommonUpdate();

			switch (m_catchEnemyState)
			{
			case enCE_FindTarget:

				if (m_playerRef->GetState() == nsPlayerConstData::enOnEnemy)
				{
					ChangeState(enCE_GoOnEnemy);
				}

				if (m_isStandUp && 
					m_playerRef->GetPlayerModelAnimation().GetAnimationState() == 
					nsPlayerConstData::nsModelAnimationConstData::enAnim_idle)
				{
					// 起き上がり中かつ、アニメーションがアイドル状態まで行ったら

					// 起き上がり中を解除
					m_isStandUp = false;
					// 入力を可能にする
					m_playerRef->SetIsInputtable(true);
				}

				if (m_playerRef->GetInputData().actionSearchEnemy == true)
				{
					// 敵を探知するアクションが入っていたら、
					// ソナーのエフェクトを再生する
					m_sonarEF->SetPosition(m_playerRef->GetPosition());
					m_sonarEF->SetScale(Vector3::One);
					m_sonarEF->Play();
					m_sonarTimer = 0.0f;
					m_sonarSC->Play(false);
				}

				if (m_sonarEF->IsPlay())
				{
					// ソナーのエフェクトが再生されていたら、徐々に拡大していく
					const float rate = m_sonarTimer / kSonarEffectTime;
					const float scale = Math::Lerp<float>(rate, kSonarEffectMinScale, kSonarEffectMaxScale);
					Vector3 scaleVec = { scale, kSonarEffectYScale, scale };
					m_sonarEF->SetScale(scaleVec);
					m_sonarTimer += m_playerRef->GetDeltaTime();
				}

				// ターゲットを探す
				FindTarget();

				// ターゲットとの距離を更新
				UpdateTargetLength();

				// 敵を捕まえるかどうか調べる
				CheckCatchEnemy();
				break;

			case enCE_GoOnEnemy:
				if (m_playerRef->GetPlayerMovement().GetPlayerOnEnemy().GetOnEnemyState() ==
					nsPlayerConstData::nsOnEnemyConstData::enOnEnemy)
				{
					ChangeState(enCE_InputingCommand);
				}
				break;

			case enCE_InputingCommand:
				OnEnemyUpdate();
				break;

			case enCE_SuccessCommand:
				SuccessCommandUpdate();
				break;

			case enCE_FailureCommand:
				ChangeState(enCE_End);
				break;

			case enCE_End:
				break;
			}

			return;
		}

		/**
		 * @brief タイマーバーのスプライトの初期化
		*/
		void CPlayerCatchEnemy::InitTimerBarSprite()
		{
			// バーの生成と初期化
			m_onEnemyTimerBar = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_onEnemyTimerBar->Init(
				kOnEnemyTimerBarSpriteFilePath,
				kOnEnemyTimerBarSpirteWidth,
				kOnEnemyTimerBarSpirteHeight,
				kOnEnemyTimerVarSpritePivot,
				AlphaBlendMode_Trans
			); 

			// バーの座標。ピボットをずらすため、座標を計算する。
			Vector2 pos = kOnEnemyTimerVarSpritePosition;
			// 画像のハーフサイズ
			float halfSize = 0.0f;
			halfSize = static_cast<float>(kOnEnemyTimerBarSpirteWidth) * 0.5f;
			// ピボットの分だけ、ずらす。
			pos.x += halfSize * kOnEnemyTimerVarSpritePivot.x;
			// 座標を設定
			m_onEnemyTimerBar->SetPosition(pos);
			// 非表示にする
			m_onEnemyTimerBar->Deactivate();

			// バーのフレームの生成と初期化
			m_onEnemyTimerBarFrame = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_onEnemyTimerBarFrame->Init(
				kOnEnemyTimerBarFrameSpriteFilePath,
				kOnEnemyTimerBarFrameSpirteWidth,
				kOnEnemyTimerBarFrameSpirteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// 座標を設定。こちらは、ピボットをずらしてないため、そのまま入れる。
			m_onEnemyTimerBarFrame->SetPosition(kOnEnemyTimerVarSpritePosition);
			// 非表示にする
			m_onEnemyTimerBarFrame->Deactivate();

			return;
		}

		/**
		 * @brief QTEに使うボタンのスプライトの初期化
		*/
		void CPlayerCatchEnemy::InitQTEButtonSprite()
		{
			// いや、まだリソースバンク作ってなかったわ。
			for (int i = 0; i < enQTEButtonTypeNum; i++)
			{
				// ゲーム中の読み込みを速くするため、最初に読み込んですぐに破棄する。
				// リソースバンクに登録される。
				nsGraphic::nsSprite::CSpriteRender* sr = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFirst);
				sr->Init(
					kQTEButtonSpriteFilePath[i],
					0.0f, 0.0f
				);
				sr->Deactivate();
				DeleteGO(sr);
			}


			// QTEに使うボタンの枠のスプライトの生成と初期化
			m_QTEButtonFraneSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_QTEButtonFraneSR->Init(
				kQTEButtonFrameSpriteFilePath,
				kQTEButtonFrameSpriteWidth,
				kQTEButtonFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// 非表示
			m_QTEButtonFraneSR->Deactivate();

			// QTEに使うボタンの残像のスプライトの生成と初期化
			m_QTEButtonAfterImageSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_QTEButtonAfterImageSR->Init(
				kQTEButtonAfterImageSpriteFilePath,
				kQTEButtonAfterImageSpriteWidth,
				kQTEButtonAfterImageSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// アルベドカラーを制御可能にする
			m_QTEButtonAfterImageSR->SetIsControlAlbedo(true);
			// 非表示
			m_QTEButtonAfterImageSR->Deactivate();

			int i = 0;	// インデックス
			for (auto& qteResultSR : m_QTEResultSR)
			{			
				// QTEの結果のスプライトの生成と初期化
				qteResultSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth);
				qteResultSR->Init(
					kQTEResultSpriteFilePaths[i],
					kQTEResultSpriteWitdh,
					kQTEResultSpriteHeight,
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				// 座標を設定
				qteResultSR->SetPosition(kQTEResultSpriteStartPos);
				// 非表示
				qteResultSR->Deactivate();

				// インデックスを進める
				i++;
			}

			// QTEの結果の枠（内側）スプライトの生成と初期化
			m_QTEResultFrameIn = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_QTEResultFrameIn->Init(
				kQTEResultFrameInSpriteFilePath,
				kQTEResultFrameSpriteWitdh,
				kQTEResultFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// 座標を設定
			m_QTEResultFrameIn->SetPosition(kQTEResultSpriteEndPos);
			// 非表示
			m_QTEResultFrameIn->Deactivate();

			// QTEの結果の枠（外側）スプライトの生成と初期化
			m_QTEResultFrameOut = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_QTEResultFrameOut->Init(
				kQTEResultFrameOutSpriteFilePath,
				kQTEResultFrameSpriteWitdh,
				kQTEResultFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// 座標を設定
			m_QTEResultFrameOut->SetPosition(kQTEResultSpriteEndPos);
			// 非表示
			m_QTEResultFrameOut->Deactivate();

			return;
		}


		/**
		 * @brief 忍術のエフェクトを初期化
		*/
		void CPlayerCatchEnemy::InitNinjyutuEffect()
		{
			// 忍術のエフェクトの生成と初期化
			m_ninjyutuEF = NewGO<Effect>(nsCommonData::enPrioritySecond);
			m_ninjyutuEF->Init(kNinjyutuEffectFilePath);

			m_sonarEF = NewGO<Effect>(nsCommonData::enPrioritySecond);
			m_sonarEF->Init(kSonarEffectFilePath);

			return;
		}

		/**
		 * @brief サウンドを初期化
		*/
		void CPlayerCatchEnemy::InitSound()
		{
			m_commandSuccessSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_commandSuccessSC->Init(kCommandSuccessSoundFilePath, nsSound::CSoundCue::enSE);
			m_commandSuccessSC->SetVolume(kCommandSuccessSoundVolume);

			m_commandMissSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_commandMissSC->Init(kCommandMissSoundFilePath, nsSound::CSoundCue::enSE);
			m_commandMissSC->SetVolume(kCommandMissSoundVolume);

			m_windowOpneSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_windowOpneSC->Init(kWindowOpneSoundFilePath, nsSound::CSoundCue::enSE);
			m_windowOpneSC->SetVolume(kWindowOpenSoundVolume);

			m_windowCloseSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_windowCloseSC->Init(kWindowCloseSoundFilePath, nsSound::CSoundCue::enSE);
			m_windowCloseSC->SetVolume(kWindowCloseSoundVolume);

			m_fireStartSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_fireStartSC->Init(kFireStartSoundFilePath, nsSound::CSoundCue::enSE);
			m_fireStartSC->SetVolume(kFireStartSoundVolume);

			m_fireReleaseSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_fireReleaseSC->Init(kFireReleaseSoundFilePath, nsSound::CSoundCue::enSE);
			m_fireReleaseSC->SetVolume(kFireReleaseSoundVolume);

			m_sonarSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_sonarSC->Init(kSonarSoundFilePath, nsSound::CSoundCue::enSE);
			m_sonarSC->SetVolume(kSonarSoundVolume);

			for (int i = 0; i < kFireVoiceSoundTypeNum; i++)
			{
				m_fireVoiceSC[i] = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_fireVoiceSC[i]->Init(kFireVoiceSoundFilePath[i], nsSound::CSoundCue::enSE);
				m_fireVoiceSC[i]->SetVolume(kFireVoiceSoundVolume);
			}
			for (int i = 0; i < kFailureVoiceSoundTypeNum; i++)
			{
				m_failureVoiceSC[i] = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_failureVoiceSC[i]->Init(KFailureVoiceSoundFilePath[i], nsSound::CSoundCue::enSE);
				m_failureVoiceSC[i]->SetVolume(kFailureVoiceSoundVolume);
			}

			return;
		}

		/**
		 * @brief QTEに使うボタンのスプライトの再初期化
		 * @param[in] commandArray コマンドの配列
		*/
		void CPlayerCatchEnemy::ReInitQTEBUttonSprite(
			const std::vector<nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType>& commandArray
		)
		{
			if (m_QTEButtonSRs.empty() != true)
			{
				// コンテナが空ではなかったら、DeleteGOを呼ぶ
				for (auto& qteButtonSR : m_QTEButtonSRs)
				{
					DeleteGO(qteButtonSR);
				}
				// 消し終わったらクリアする
				m_QTEButtonSRs.clear();
			}

			// 新しいスプライトを生成する

			// コマンドの数
			const int commandNum = static_cast<int>(commandArray.size());
			// コマンドの数だけリサーブする
			m_QTEButtonSRs.reserve(commandNum);

			// 左端の座標
			const float leftEndPos = -kQTEButtonSpriteDistance * 0.5f * (commandNum - 1);
			// 座標
			Vector2 pos = { leftEndPos , kQTEButtonSpritePosY };

			for (const auto command : commandArray)
			{
				// コマンドを一つずつ取り出してみる

				// スプライトレンダラーを生成する
				nsGraphic::nsSprite::CSpriteRender* qteButtonSR =
					NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
				// コマンドにあったスプライトのデータで初期化
				qteButtonSR->Init(
					kQTEButtonSpriteFilePath[command],
					kQTEButtonSpriteWidth[command] * kQTEButtonSizeScale[command],
					kQTEButtonSpriteHeight[command] * kQTEButtonSizeScale[command],
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				// 座標を設定する
				qteButtonSR->SetPosition(pos);
				// 待機モードの乗算カラーを設定する
				qteButtonSR->SetMulColor(kQTEButtonSpriteStandByMulColor);
				// 座標を次の座標に向けてずらす
				pos.x += kQTEButtonSpriteDistance;
				// スプライトレンダラーを配列に入れる
				m_QTEButtonSRs.emplace_back(qteButtonSR);
			}

			// 先頭のスプライトをアクティブなモードの乗算カラーと拡大率にする
			m_QTEButtonSRs[0]->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
			m_QTEButtonSRs[0]->SetScale(kQTEButtonSpriteActionScale);
		}


		/**
		 * @brief いつでも共通で行われる更新
		*/
		void CPlayerCatchEnemy::CommonUpdate()
		{
			// QTEに使うボタンの残像の更新
			QTEButternAfterImageUpdate();

			// QTEの結果の更新
			QTEResultUpdate();

			return;
		}

		/**
		 * @brief 敵の上に乗っている時の更新
		*/
		void CPlayerCatchEnemy::OnEnemyUpdate()
		{
			// コマンド入力を実行する
			m_commandInput->Execute();

			if (m_commandInput->IsEndCommandInput() == true)
			{
				// コマンド入力が終了した。コマンド入力成功。

				// コマンド入力が成功したステートへ遷移
				ChangeState(enCE_SuccessCommand);
				// QTEに使うボタンの残像を開始する
				StartQTEButtonAfterImage(true);
				// QTEの結果のスプライトの演出を成功で開始する
				StartQTEResult(enQR_success);

				// コマンド入力成功のサウンドを再生
				m_commandSuccessSC->Play(false);

				return;
			}
			
			// コマンド入力が、まだ終わっていない。

			if (m_commandInput->GetCommandResult() == 
				nsPlayerConstData::nsCommandInputConstData::enCR_Success)
			{
				// コマンド入力が成功している。次のコマンドに進む。

				// 現在のボタンのスプライトを非表示にする
				m_QTEButtonSRs[m_oldCommandProgress]->Deactivate();
				// 次のボタンのスプライトをアクティブモードの乗算カラーと拡大率にする
				m_QTEButtonSRs[m_oldCommandProgress + 1]->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
				m_QTEButtonSRs[m_oldCommandProgress + 1]->SetScale(kQTEButtonSpriteActionScale);
				// ボタンの枠を次のボタンの位置にする
				m_QTEButtonFraneSR->SetPosition(m_QTEButtonSRs[m_oldCommandProgress + 1]->GetPosition());

				// QTEに使うボタンの残像を開始する
				StartQTEButtonAfterImage(true);

				// コマンド入力成功のサウンドを再生
				m_commandSuccessSC->Play(false);
			}
			else if (m_commandInput->GetCommandResult() ==
				nsPlayerConstData::nsCommandInputConstData::enCR_Miss)
			{
				// コマンド入力が失敗している。

				// コマンド入力が失敗したステートへ遷移
				ChangeState(enCE_FailureCommand);
				// QTEに使うボタンの残像を開始する
				StartQTEButtonAfterImage(false);
				// QTEの結果のスプライトの演出を入力ミスで開始する
				StartQTEResult(enQR_miss);

				// コマンド入力失敗のサウンドを再生
				m_commandMissSC->Play(false);
			}

			// 前フレームのコマンド進行度を更新する
			m_oldCommandProgress = m_commandInput->GetCommandProgress();

			// タイマーを進める
			m_onEnemyTimer += m_playerRef->GetDeltaTime();

			// バーの拡大率。徐々に小さくしていく。
			const float barRate = 1.0f - (m_onEnemyTimer / kOnEnemyTime);
			// バーの拡大率を設定
			m_onEnemyTimerBar->SetScale({ barRate ,1.0f,1.0f });


			if (m_onEnemyTimer >= kOnEnemyTime)
			{
				// 時間切れ

				// コマンド入力が失敗したステートへ遷移
				ChangeState(enCE_FailureCommand);
				// QTEの結果のスプライトの演出を時間切れで開始する
				StartQTEResult(enQR_failed);

				// コマンド入力失敗のサウンドを再生
				m_commandMissSC->Play(false);

				return;
			}

			return;
		}


		/**
		 * @brief QTEに使うボタンの残像の更新
		*/
		void CPlayerCatchEnemy::QTEButternAfterImageUpdate()
		{
			if (m_QTEButtonAfterImageSR->IsActive() != true)
			{
				// 残上が表示されていない。早期リターン。
				return;
			}

			// 残像が表示されている時実行

			// タイマーの進み率
			const float rate = m_buttonAfterImageTimer / kQTEButtonAfterImageTime;
			// 補完率。指数関数的変化にする。
			const float t = pow(rate, kQTEButtonAfterImagePowPower);

			// アルファ値。だんだん薄くなっていく。
			const float alphaValue =
				Math::Lerp<float>(t, kQTEButtonAfterImageSpriteAlphaValue, 0.0f);
			// 拡大率。だんだん大きくなっていく。
			const float scale = Math::Lerp<float>(t, 1.0f, kQTEButtonAfterImageMaxScale);
			// 残像のアルファ値と拡大率を設定する
			m_QTEButtonAfterImageSR->SetAlphaValue(alphaValue);
			m_QTEButtonAfterImageSR->SetScale(scale);

			// タイマーを進める
			m_buttonAfterImageTimer += m_playerRef->GetDeltaTime();

			if (m_buttonAfterImageTimer >= kQTEButtonAfterImageTime)
			{
				// タイマーが時間を過ぎたら、残像を非表示にする。
				m_QTEButtonAfterImageSR->Deactivate();
			}


			return;
		}

		/**
		 * @brief QTEの結果の更新
		*/
		void CPlayerCatchEnemy::QTEResultUpdate()
		{
			for (auto& qteResultSR : m_QTEResultSR)
			{
				if (qteResultSR->IsActive() != true)
				{
					// この種類のQTEの結果が、表示しれていない。次へ。
					continue;
				}

				if (m_resultTimer <= kQTEResultFrameMoveTime)
				{
					// QTEの結果の枠の移動時間

					// 時間の進み率
					const float t = m_resultTimer / kQTEResultFrameMoveTime;
					// QTEの結果の枠の、内側と外側、それぞれの拡大率。
					const float scaleIn = Math::Lerp<float>(t, kQTEResultFrameInSpriteMinScale, 1.0f);
					const float scaleOut = Math::Lerp<float>(t, kQTEResultFrameOutSpriteMaxScale, 1.0f);
					// 結果のスプライトのアルファ値
					const float alphaValue = Math::Lerp<float>(t, kQTEResultSpriteStartAplhaValue, 1.0f);

					// QTEの結果の枠のスプライトの拡大率とアルファ値を設定
					m_QTEResultFrameIn->SetScale(scaleIn);
					m_QTEResultFrameIn->SetAlphaValue(alphaValue);
					m_QTEResultFrameOut->SetScale(scaleOut);
					m_QTEResultFrameOut->SetAlphaValue(alphaValue);

					if (m_windowOpneSC->IsPlaying() != true)
					{
						m_windowOpneSC->Play(false);
					}
				}
				else if (m_resultTimer <= kQTEResultFrameMoveTime + kQTEResultMoveTime)
				{
					// QTEの結果の移動時間

					// QTEの結果の枠のスプライトの拡大率とアルファ値を最終値を設定
					m_QTEResultFrameIn->SetScale(1.0f);
					m_QTEResultFrameIn->SetAlphaValue(1.0f);
					m_QTEResultFrameOut->SetScale(1.0f);
					m_QTEResultFrameOut->SetAlphaValue(1.0f);

					// タイマーの進み率
					const float t = (m_resultTimer - kQTEResultFrameMoveTime) / kQTEResultMoveTime;
					// 結果のスプライトの座標
					Vector2 pos = Vector2::Zero;
					pos.Lerp(t, kQTEResultSpriteStartPos, kQTEResultSpriteEndPos);
					// 結果のスプライトのアルファ値
					const float alphaValue = Math::Lerp<float>(t, kQTEResultSpriteStartAplhaValue, 1.0f);

					// 結果のスプライトの座標とアルファ値を設定
					qteResultSR->SetPosition(pos);
					qteResultSR->SetAlphaValue(alphaValue);
				}
				else if (m_resultTimer <=
					kQTEResultFrameMoveTime + kQTEResultMoveTime + kQTEResultDisplayTime)
				{
					// QTEの結果の表示時間

					// 結果のスプライトの座標とアルファ値の最終値を設定
					qteResultSR->SetPosition(kQTEResultSpriteEndPos);
					qteResultSR->SetAlphaValue(1.0f);
				}
				else if (m_resultTimer <= kQTEResultFrameMoveTime + kQTEResultMoveTime +
					kQTEResultDisplayTime + kQTEResultBackTime)
				{
					//  QTEの結果の戻る時間

					// タイマーの進み率
					const float t = (m_resultTimer - kQTEResultFrameMoveTime - 
						kQTEResultMoveTime - kQTEResultDisplayTime) /
						kQTEResultBackTime;
					// 結果のスプライトの座標
					Vector2 pos = Vector2::Zero;
					pos.Lerp(t, kQTEResultSpriteEndPos, kQTEResultSpriteStartPos);
					// 結果のスプライトのアルファ値
					const float alphaValue = Math::Lerp<float>(t, 1.0f, 0.0f);

					// 結果のスプライトの座標とアルファ値を設定
					qteResultSR->SetPosition(pos);
					qteResultSR->SetAlphaValue(alphaValue);
				}
				else if (m_resultTimer <= kQTEResultFrameMoveTime + kQTEResultMoveTime +
					kQTEResultDisplayTime + kQTEResultBackTime + kQTEResultFrameBackTime)
				{
					// QTEの結果の枠の戻る時間

					// タイマーの進み率。
					const float t = (m_resultTimer - kQTEResultFrameMoveTime -
						kQTEResultMoveTime - kQTEResultDisplayTime - kQTEResultBackTime) /
						kQTEResultFrameBackTime;

					// QTEの結果の枠の、内側と外側、それぞれの拡大率。
					const float scaleIn = Math::Lerp<float>(t, 1.0f, kQTEResultFrameInSpriteMinScale);
					const float scaleOut = Math::Lerp<float>(t, 1.0f, kQTEResultFrameOutSpriteMaxScale);
					// 結果のスプライトのアルファ値
					const float alphaValue = Math::Lerp<float>(t, 1.0f, 0.0f);

					// QTEの結果の枠のスプライトの拡大率とアルファ値を設定
					m_QTEResultFrameIn->SetScale(scaleIn);
					m_QTEResultFrameIn->SetAlphaValue(alphaValue);
					m_QTEResultFrameOut->SetScale(scaleOut);
					m_QTEResultFrameOut->SetAlphaValue(alphaValue);

					if (m_windowCloseSC->IsPlaying() != true)
					{
						m_windowCloseSC->Play(false);
					}
				}


				// タイマーを進める。
				m_resultTimer += m_playerRef->GetDeltaTime();

				if (m_resultTimer > kQTEResultFrameMoveTime + kQTEResultMoveTime +
					kQTEResultDisplayTime + kQTEResultBackTime + kQTEResultFrameBackTime)
				{
					// タイマーが、全ての時間を過ぎたら、非表示にして、終了。
					qteResultSR->Deactivate();
					m_QTEResultFrameIn->Deactivate();
					m_QTEResultFrameOut->Deactivate();
				}

				// 一種類しか、更新しないはずなので、ループを抜ける。
				break;
			}

			return;
		}


		/**
		 * @brief コマンドが成功した時の更新
		*/
		void CPlayerCatchEnemy::SuccessCommandUpdate()
		{
			// 忍術のエフェクトの座標
			Vector3 pos = m_targetRef->GetPosition();
			// 少し上にあげる
			pos.y += kNinjyutuEffectPosBufHeight;

			// 忍術のエフェクトの座標と回転を更新する
			m_ninjyutuEF->SetPosition(pos);
			m_ninjyutuEF->SetRotation(m_targetRef->GetRotation());

			// タイマーを進める
			m_ninnjyutuEFTimer += m_playerRef->GetDeltaTime();
			
			if (m_ninnjyutuEFTimer >= kNinjyutuEffectTime)
			{
				// タイマーが、忍術のエフェクトの時間を超えたら、
				// 車を捕まえた状態にして、忍術のエフェクトの参照を渡して、ステートを遷移する。
				m_targetRef->BeCaptured();
				m_targetRef->SetNinjyutuEffectRef(m_ninjyutuEF);
				ChangeState(enCE_End);

			}
			else if (m_ninnjyutuEFTimer >= kFireReleaseSoundTime)
			{
				if (m_fireReleaseSC->IsPlaying() != true)
				{
					m_fireReleaseSC->Play(false);
					m_fireVoiceSC[(*m_fireVoiceRand)(*m_mt)]->Play(false);
				}
			}


			return;
		}

		/**
		 * @brief ターゲットを探す
		*/
		void CPlayerCatchEnemy::FindTarget()
		{
			if (m_targetRef)
			{
				// ターゲットがいる場合、ターゲットとして継続してよいか調べる。

				// ターゲット状態を継続するか調べる
				CheckContinueTarget();
			}

			if (m_targetRef != nullptr)
			{
				// ターゲットがいる場合は探さないので、早期リターン。
				return;
			}

			if (m_aiCarsRef->empty())
			{
				// エネミーがいないなら、調べない。早期リターン。
				return;
			}

			// エネミー1体ずつ調べる
			for (auto& enemy : *m_aiCarsRef)
			{
				if (enemy->IsCaputred())
				{
					// 捕まっていたら、次へ。
					continue;
				}
				// プレイヤーからエネミーへのベクトル
				Vector3 playerToEnemyVec = enemy->GetPosition() - m_gameState->GetPlayer().GetPosition();
				// プレイヤーからエネミーへの距離
				const float playerToEnemyLen = playerToEnemyVec.Length();

				if (playerToEnemyLen >= kTargetMaxLength)
				{
					// 距離が一定以上離れているため、ターゲットにはなれない。次へ。
					continue;
				}

				// カメラからエネミーへのベクトル
				Vector3 cameraToEnemyVec =
					enemy->GetPosition() - m_gameState->GetPlayer().GetCamera().GetCameraPosition();
				// カメラからエネミーへの方向
				Vector3 cameraToEnemyNorm = cameraToEnemyVec;
				cameraToEnemyNorm.Normalize();	// 正規化する
				// カメラからエネミーへの方向と、カメラの前方向の、角度差
				float angleDiff =
					Dot(cameraToEnemyNorm, m_gameState->GetPlayer().GetCamera().GetCameraForward());
				angleDiff = acosf(angleDiff);	// 内積を角度に変換

				if (angleDiff >= kTargetMaxAngle)
				{
					// 角度差が一定以上の大きさのため、ターゲットにはなれない。次へ。
					continue;
				}

				// @todo
				// エネミーへの視線が、建物に遮られていないか調べる処理を追加する。
				// 多分OBBの当たり判定で計算する。


				// 全ての条件を満たしているため、ターゲットとする。
				m_targetRef = enemy;
				// ターゲットが見つかったため、ループを終了する。
				break;
			}

			return;
		}

		/**
		 * @brief ターゲット状態を継続するか調べる
		*/
		void CPlayerCatchEnemy::CheckContinueTarget()
		{
			// プレイヤーからターゲットへのベクトル
			Vector3 playerToTargetVec = m_targetRef->GetPosition() - m_gameState->GetPlayer().GetPosition();
			// プレイヤーからターゲットまでの距離
			const float playerToTargetLen = playerToTargetVec.Length();
			if (playerToTargetLen >= kTargetMaxLength)
			{
				// 距離が一定以上離れていたら、ターゲットから解除する。
				m_targetRef = nullptr;
				// 解除したので、早期リターン
				return;
			}

			// カメラからターゲットへのベクトル
			Vector3 cameraToTargetVec =
				m_targetRef->GetPosition() - m_gameState->GetPlayer().GetCamera().GetCameraPosition();
			// カメラからターゲットへの方向
			Vector3 cameraToTargetNorm = cameraToTargetVec;
			cameraToTargetNorm.Normalize();	// 正規化する
			// カメラからターゲットへの方向と、カメラの前方向の、角度差
			float angleDiff =
				Dot(cameraToTargetNorm, m_gameState->GetPlayer().GetCamera().GetCameraForward());
			angleDiff = acosf(angleDiff);	// 内積から角度へ変換

			if (angleDiff >= kTargetMaxAngle)
			{
				// 角度差が一定以上の大きさなら、ターゲットから解除する。
				m_targetRef = nullptr;
				// 解除したので、早期リターン
				return;
			}

			return;
		}


		/**
		 * @brief ターゲットとの距離を更新
		*/
		void CPlayerCatchEnemy::UpdateTargetLength()
		{
			if (m_targetRef == nullptr)
			{
				// ターゲットが見つかっていない場合は、これ以下の処理は行わない。
				return;
			}

			// プレイヤーから、ターゲットへの距離を計算する
			Vector3 playerToDiff = m_targetRef->GetPosition() - m_playerRef->GetPosition();
			m_targetLength = playerToDiff.Length();

			return;
		}

		/**
		 * @brief 敵を捕まえるかどうか調べる
		*/
		void CPlayerCatchEnemy::CheckCatchEnemy()
		{
			if (m_targetRef == nullptr ||
				m_targetLength > kCanCatchMaxLength)
			{
				// ターゲットが見つかっていない、または、
				// ターゲットとの距離が一定以下ではないとき
				
				// 敵を捕まえることができない。早期リターン。
				return;
			}

			// 捕まえることができる

			if (m_playerRef->GetInputData().actionCatchEnemy)
			{
				// 敵を捕まえる入力情報がある
				// 敵の上に乗る状態へ遷移する
				m_playerRef->ChangeOnEnemyState();
			}

			return;
		}


		/**
		 * @brief QTEに使うボタンの残像を開始する
		 * @param[in] isSuccess コマンド入力が成功したか？
		*/
		void CPlayerCatchEnemy::StartQTEButtonAfterImage(const bool isSuccess)
		{
			// 残像用のスプライトを表示する
			m_QTEButtonAfterImageSR->Activate();
			// 残像用のスプライトを前のボタンの位置に移動させる
			m_QTEButtonAfterImageSR->SetPosition(m_QTEButtonSRs[m_oldCommandProgress]->GetPosition());
			// 残像用のスプライトのアルファ値、拡大率、タイマーをリセット
			m_QTEButtonAfterImageSR->SetAlphaValue(kQTEButtonAfterImageSpriteAlphaValue);
			m_QTEButtonAfterImageSR->SetScale(1.0f);
			m_buttonAfterImageTimer = 0.0f;

			if (isSuccess)
			{
				// コマンド入力が成功していたら、
				// 残像用のスプライトのアルベドカラーを成功時のアルベドカラーにする。
				m_QTEButtonAfterImageSR->SetAlbedoColor(kQTEButtonAfterImageSpriteSuccessAlbedoColor);
			}
			else
			{
				// コマンド入力が失敗していたら、
				// 残像用のスプライトのアルベドカラーを失敗時のアルベドカラーにする。
				m_QTEButtonAfterImageSR->SetAlbedoColor(kQTEButtonAfterImageSpriteMissAlbedoColor);
			}


			return;
		}

		/**
		 * @brief QTEの結果のスプライトの演出を開始する
		 * @param[in] qteResultType QTEの結果の種類
		*/
		void CPlayerCatchEnemy::StartQTEResult(
			const nsPlayerConstData::nsCatchEnemyConstData::EnQTEResultType qteResultType
		)
		{
			// スプライトを有効化
			m_QTEResultSR[qteResultType]->Activate();
			m_QTEResultFrameIn->Activate();
			m_QTEResultFrameOut->Activate();
			// 座標とアルファ値とタイマーをリセット
			m_QTEResultSR[qteResultType]->SetPosition(kQTEResultSpriteStartPos);
			m_QTEResultSR[qteResultType]->SetAlphaValue(0.0f);
			m_QTEResultFrameIn->SetScale(1.0f);
			m_QTEResultFrameOut->SetScale(1.0f);
			m_QTEResultFrameIn->SetAlphaValue(0.0f);
			m_QTEResultFrameOut->SetAlphaValue(0.0f);
			m_resultTimer = 0.0f;

			return;
		}


		/**
		 * @brief QTEで使うスプライトを有効化する
		*/
		void CPlayerCatchEnemy::ActivateQTESprite()
		{
			m_onEnemyTimerBar->Activate();
			m_onEnemyTimerBarFrame->Activate();
			m_QTEButtonFraneSR->Activate();

			return;
		}

		/**
		 * @brief QTEで使うスプライトを有効化する
		*/
		void CPlayerCatchEnemy::DeactivateQTESprite()
		{
			m_onEnemyTimerBar->Deactivate();
			m_onEnemyTimerBarFrame->Deactivate();
			m_QTEButtonFraneSR->Deactivate();

			return;
		}

		/**
		 * @brief 敵を捕まえる処理が終了した時の処理
		*/
		void CPlayerCatchEnemy::EndCatchEnemy()
		{
			// エフェクトタイマーをリセットする
			m_ninnjyutuEFTimer = 0.0f;
			// ステートをリセットする
			m_catchEnemyState = enCE_FindTarget;
			// 敵の上に乗っているタイマーをリセットする
			m_onEnemyTimer = 0.0f;
			// ターゲットの敵がいないようにする
			m_targetRef = nullptr;
			// 敵の上に乗っているクラスをリセットする
			m_playerRef->ResetOnEnemy();
			// プレイヤーのステートを、歩きと走りに遷移する。
			m_playerRef->ChangeWalkAndRunState();
			// コマンド入力クラスを終了させる
			m_commandInput->EndCommandInput();
			// 前フレームのコマンド進行度をリセットする
			m_oldCommandProgress = 0;

			// スプライトたちをすべて非表示にする
			for (auto& qteButtonSR : m_QTEButtonSRs)
			{
				qteButtonSR->Deactivate();
			}
			DeactivateQTESprite();

			return;
		}

		/**
		 * @brief ステートを遷移する
		 * @param[in] newState 新しいステート
		*/
		void CPlayerCatchEnemy::ChangeState(const nsPlayerConstData::nsCatchEnemyConstData::EnCatchEnemyState newState)
		{
			if (m_catchEnemyState == newState)
			{
				return;
			}


			m_catchEnemyState = newState;

			switch (m_catchEnemyState)
			{
			case enCE_FindTarget:
				break;

			case enCE_GoOnEnemy:
				break;

			case enCE_InputingCommand:
				// コマンドを初期化する
				m_commandInput->InitCommand(4, nsPlayerConstData::nsCommandInputConstData::enCT_type3);
				// QTEに使うボタンのスプライトを再初期化
				ReInitQTEBUttonSprite(m_commandInput->GetCommandButtonTypeArray());
				// QTEで使うスプライトの有効化
				ActivateQTESprite();
				// ボタンの枠のスプライトの座標を、一番左のボタンのスプライトの座標と同じにする。
				m_QTEButtonFraneSR->SetPosition(m_QTEButtonSRs[0]->GetPosition());
				break;

			case enCE_SuccessCommand:
				// スプライトたちをすべて非表示にする
				for (auto& qteButtonSR : m_QTEButtonSRs)
				{
					qteButtonSR->Deactivate();
				}
				DeactivateQTESprite();

				// 忍術のエフェクトの座標と回転を設定して、再生する。
				m_ninjyutuEF->SetPosition(m_targetRef->GetPosition() + Vector3::Up * kNinjyutuEffectPosBufHeight);
				m_ninjyutuEF->SetRotation(m_targetRef->GetRotation());
				m_ninjyutuEF->Play();

				m_fireStartSC->Play(false);
				break;

			case enCE_FailureCommand:
				// コマンドミスの回数をカウントする
				nsGameState::CGameMainState::GetInstance()->CountMissCommand();
				// スプライトたちをすべて非表示にする
				for (auto& qteButtonSR : m_QTEButtonSRs)
				{
					qteButtonSR->Deactivate();
				}
				DeactivateQTESprite();

				// 一定時間入力不可にする
				m_playerRef->SetIsInputtable(false);
				// 起き上がり中にする
				m_isStandUp = true;

				m_failureVoiceSC[(*m_failureVoiceRand)(*m_mt)]->Play(false);

				break;

			case enCE_End:
				// 敵を捕まえる処理が終了した時の処理
				EndCatchEnemy();
				break;
			}

			return;
		}

	}
}