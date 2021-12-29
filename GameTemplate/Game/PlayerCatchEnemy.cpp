#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "Player.h"
#include "GameMainState.h"
#include "AICar.h"
#include "SpriteRender.h"

namespace nsMyGame
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

			return;
		}


		/**
		 * @brief デストラクタ
		*/
		CPlayerCatchEnemy::~CPlayerCatchEnemy()
		{
			DeleteGO(m_onEnemyTimerBar);
			DeleteGO(m_onEnemyTimerBarFrame);

			for (int i = 0; i < enQTEButtouTypeNum; i++)
			{
				DeleteGO(m_onQTEButtonSRs[i]);
			}

			DeleteGO(m_onQTEButtonFraneSR);

			return;
		}

		/**
		 * @brief 初期化処理
		 * @param[in,out] playerRef プレイヤーの参照
		*/
		void CPlayerCatchEnemy::Init(CPlayer* playerRef)
		{
			m_playerRef = playerRef;

			// ゲームステートの参照を引っ張ってくる
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enMainGameState]
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
			if (m_playerRef->GetState() == nsPlayerConstData::enOnEnemy)
			{
				// 敵の上に乗っている時の更新
				OnEnemyUpdate();
				// 早期リターン
				return;
			}

			// ターゲットを探す
			FindTarget();

			// このクラスで使う共通データの更新
			UpdateCommonData();

			// 敵を捕まえるかどうか調べる
			CheckCatchEnemy();

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
			for (int i = 0; i < enQTEButtouTypeNum; i++)
			{
				// QTEに使うボタンたちのスプライトの生成と初期化
				m_onQTEButtonSRs[i] = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
				m_onQTEButtonSRs[i]->Init(
					kQTEButtonSpriteFilePath[i],
					kQTEButtonSpriteWidth[i] * kQTEButtonSizeScale[i],
					kQTEButtonSpriteHeight[i] * kQTEButtonSizeScale[i],
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				Vector2 pos = { -400.0f,0.0f };
				pos.x += i * 66.0f;
				m_onQTEButtonSRs[i]->SetPosition(pos);
				if (i != 0)
				{
					constexpr float mul = 0.3f;
					m_onQTEButtonSRs[i]->SetMulColor({ mul,mul,mul,1.0f });
				}
				else
				{
					m_onQTEButtonSRs[i]->SetScale(1.25f);
				}
				//m_onQTEButtonSRs[i]->Deactivate();
					
			}

			// QTEに使うボタンの枠のスプライトの生成と初期化
			m_onQTEButtonFraneSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_onQTEButtonFraneSR->Init(
				kQTEButtonFrameSpriteFilePath,
				kQTEButtonFrameSpriteWidth,
				kQTEButtonFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			m_onQTEButtonFraneSR->SetPosition({ -400.0f,0.0f });
			//m_onQTEButtonFraneSR->Deactivate();

			return;
		}

		/**
		 * @brief 敵の上に乗っている時の更新
		*/
		void CPlayerCatchEnemy::OnEnemyUpdate()
		{
			if (m_playerRef->GetCamera().IsOnEnemyCamera() != true)
			{
				// 敵の上にまだ乗っていなかったら、早期リターン。
				return;
			}

			m_onEnemyTimer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_onEnemyTimer >= 5.0f)
			{
				m_onEnemyTimer = 0.0f;
				m_targetRef = nullptr;
				m_playerRef->ResetOnEnemy();
				m_playerRef->ChangeWalkAndRunState();
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

			// エネミー1体ずつ調べる
			for (auto& enemy : *m_aiCarsRef)
			{
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
		 * @brief このクラスで使う共通データの更新
		*/
		void CPlayerCatchEnemy::UpdateCommonData()
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


	}
}