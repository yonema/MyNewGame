#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "Player.h"
#include "GameMainState.h"
#include "AICar.h"
#include "SpriteRender.h"
#include "PlayerCommandInput.h"

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

			// コマンド入力クラスを生成する
			m_commandInput = std::make_unique<CPlayerCommandInput>();

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

			m_commandInput->Init(*m_playerRef);


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
			// QTEに使うボタンの枠のスプライトの生成と初期化
			m_onQTEButtonFraneSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_onQTEButtonFraneSR->Init(
				kQTEButtonFrameSpriteFilePath,
				kQTEButtonFrameSpriteWidth,
				kQTEButtonFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// 非表示
			m_onQTEButtonFraneSR->Deactivate();

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
		 * @brief 敵の上に乗っている時の更新
		*/
		void CPlayerCatchEnemy::OnEnemyUpdate()
		{
			if (m_playerRef->GetPlayerMovement().GetPlayerOnEnemy().GetOnEnemyState() != 
				nsPlayerConstData::nsOnEnemyConstData::enOnEnemy)
			{
				// 敵の上にまだ乗っていなかったら、早期リターン。
				return;
			}

			if (m_onEnemyTimer <= FLT_EPSILON)
			{
				// タイマーが進んでいない、最初だけ呼ばれる処理

				// コマンドを初期化する
				m_commandInput->InitCommand(4, nsPlayerConstData::nsCommandInputConstData::enCT_type3);
				// QTEに使うボタンのスプライトを再初期化
				ReInitQTEBUttonSprite(m_commandInput->GetCommandButtonTypeArray());
				// QTEで使うスプライトの有効化
				QTESpriteActivate();
				// ボタンの枠のスプライトの座標を、一番左のボタンのスプライトの座標と同じにする。
				m_onQTEButtonFraneSR->SetPosition(m_QTEButtonSRs[0]->GetPosition());
			}

			// コマンド入力を実行する
			m_commandInput->Execute();

			if (m_commandInput->IsEndCommandInput() != true)
			{
				// コマンド入力が、まだ終わっていない。

				if (m_oldCommandProgress != m_commandInput->GetCommandProgress())
				{
					// 前フレームのコマンド進行度と違ったら

					// 現在のスプライトを非表示にする
					m_QTEButtonSRs[m_oldCommandProgress]->Deactivate();
					// 次のスプライトをアクティブモードの乗算カラーと拡大率にする
					m_QTEButtonSRs[m_oldCommandProgress + 1]->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
					m_QTEButtonSRs[m_oldCommandProgress + 1]->SetScale(kQTEButtonSpriteActionScale);
					// ボタンの枠を次のボタンの位置にする
					m_onQTEButtonFraneSR->SetPosition(m_QTEButtonSRs[m_oldCommandProgress + 1]->GetPosition());
				}
			}
			else
			{
				// コマンド入力が終了した。コマンド入力成功。

				// 敵を捕まえる処理が終了した時の処理
				EndCatchEnemy();
				return;
			}

			// 前フレームのコマンド進行度を更新する
			m_oldCommandProgress = m_commandInput->GetCommandProgress();

			// タイマーを進める
			m_onEnemyTimer += nsTimer::GameTime().GetFrameDeltaTime();

			// バーの拡大率。徐々に小さくしていく。
			const float barRate = 1.0f - (m_onEnemyTimer / kOnEnemyTime);
			// バーの拡大率を設定
			m_onEnemyTimerBar->SetScale({ barRate ,1.0f,1.0f });


			if (m_onEnemyTimer >= kOnEnemyTime)
			{
				// 敵を捕まえる処理が終了した時の処理
				EndCatchEnemy();
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


		/**
		 * @brief QTEで使うスプライトを有効化する
		*/
		void CPlayerCatchEnemy::QTESpriteActivate()
		{
			m_onEnemyTimerBar->Activate();
			m_onEnemyTimerBarFrame->Activate();
			m_onQTEButtonFraneSR->Activate();

			return;
		}

		/**
		 * @brief QTEで使うスプライトを有効化する
		*/
		void CPlayerCatchEnemy::QTESpriteDeactivate()
		{
			m_onEnemyTimerBar->Deactivate();
			m_onEnemyTimerBarFrame->Deactivate();
			m_onQTEButtonFraneSR->Deactivate();

			return;
		}

		/**
		 * @brief 敵を捕まえる処理が終了した時の処理
		*/
		void CPlayerCatchEnemy::EndCatchEnemy()
		{
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
			QTESpriteDeactivate();

			return;
		}

	}
}