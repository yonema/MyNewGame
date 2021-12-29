#include "stdafx.h"
#include "EnemyCatchUI.h"
#include "SpriteRender.h"
#include "FontRender.h"
#include "GameMainState.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "AICar.h"
#include "GameTime.h"


namespace nsMyGame
{
	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		using namespace nsEnemyCatchUIConstData;
		using nsPlayer::nsPlayerConstData::nsCatchEnemyConstData::kCanCatchMaxLength;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CEnemyCatchUI::Start()
		{
			// ゲームステートの参照を引っ張ってくる
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enMainGameState]
				);

			// 車たちの参照を引っ張ってくる
			m_aiCarsRef = m_gameState->GetAICar();

			// 敵を捕まえることができる合図のスプライトの初期化
			InitCanCatchEnemeySprite();

			// ターゲットを示すスプライトの初期化
			InitTargetSprite();

			// ターゲットとの距離を示すフォントの初期化
			InitTargetLengthFont();

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CEnemyCatchUI::OnDestroy()
		{
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				DeleteGO(m_canCatchEnemySR[i]);
			}
			DeleteGO(m_targetSR);
			DeleteGO(m_targetLengthFR);

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CEnemyCatchUI::Update()
		{
			// ターゲットを探す
			FindTarget();

			// このクラスて使用する共通のデータを更新
			UpdateCommonData();

			// 敵を捕まえることができる合図のスプライトの更新
			UpdateCanCatchEnemySprite();

			// ターゲットを示すスプライトの更新
			UpdateTargetSprite();

			// ターゲットとの距離を示すフォントの更新
			UpdateTargetLengthFont();

			return;
		}


		/**
		 * @brief 敵を捕まえることができる合図のスプライトの初期化
		*/
		void CEnemyCatchUI::InitCanCatchEnemeySprite()
		{
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				m_canCatchEnemySR[i] = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
				m_canCatchEnemySR[i]->Init(
					kCanCatchEnemySpriteFilePath[i],
					kCanCatchEnemySpriteWidht[i],
					kCanCatchEnemySpriteHeight[i],
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
			}

			return;
		}

		/**
		 * @brief ターゲットを示すスプライトの初期化
		*/
		void CEnemyCatchUI::InitTargetSprite()
		{
			m_targetSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_targetSR->Init(
				kTargetSpriteFilePath,
				kTargetSpriteWidth,
				kTargetSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			return;
		}

		/**
		 * @brief ターゲットとの距離を示すフォントの初期化
		*/
		void CEnemyCatchUI::InitTargetLengthFont()
		{
			m_targetLengthFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityThird);
			m_targetLengthFR->SetParam(
				L"",
				Vector2::Zero,
				Vector4::White,
				0.0f,
				0.5f
			);
			return;
		}

		/**
		 * @brief ターゲットを探す
		*/
		void CEnemyCatchUI::FindTarget()
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
			for (const auto& enemy : m_aiCarsRef)
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
		void CEnemyCatchUI::CheckContinueTarget()
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
		 * @brief このクラスて使用する共通のデータを更新
		*/
		void CEnemyCatchUI::UpdateCommonData()
		{
			if (m_targetRef == nullptr)
			{
				// ターゲットが見つかっていない場合は、これ以下の処理は行わない。
				return;
			}

			// プレイヤーから、ターゲットへの距離を計算する
			Vector3 playerToDiff = m_targetRef->GetPosition() - m_gameState->GetPlayer().GetPosition();
			m_targetLength = playerToDiff.Length();

			// ターゲットの画面上の2D座標を計算する
			g_camera3D->CalcScreenPositionFromWorldPosition(m_taraget2DPos, m_targetRef->GetPosition());

			return;
		}

		/**
		 * @brief 敵を捕まえることができる合図のスプライトの更新
		*/
		void CEnemyCatchUI::UpdateCanCatchEnemySprite()
		{
			if (m_targetRef == nullptr || 
				m_targetLength > kCanCatchMaxLength)
			{
				// ターゲットが見つかっていない、または、
				// ターゲットとの距離が一定以下ではないとき

				for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
				{
					// すべて非表示
					m_canCatchEnemySR[i]->Deactivate();
				}
				// 早期リターン
				return;
			}


			// 補間率
			float t = 0.0f;
			// 枠1の角度
			float Frame1angle = 0.0f;
			// 枠2の拡大率
			float Frame2scale = 1.0f;
			
			if (m_canCatchEnemyTimer <= kCanCatchTime1)
			{
				// タイマーがタイム1以下の時
				// ゆっくりと、時計回りに回転して、拡大していく。
				t = m_canCatchEnemyTimer / kCanCatchTime1;
				Frame1angle = Math::Lerp<float>(t, 0.0f, 360.0f);
				Frame2scale = Math::Lerp<float>(t, 1.0f, kCanCatchMaxScale);
			}
			else if (m_canCatchEnemyTimer <= (kCanCatchTime1 + kCanCatchTime2))
			{
				// タイマーがタイム2以下の時
				// 速く、反時計回りに回転して、縮小していく。
				t = (m_canCatchEnemyTimer - kCanCatchTime1) / kCanCatchTime2;
				Frame1angle = Math::Lerp<float>(t, 360.0f, 0.0f);
				Frame2scale = Math::Lerp<float>(t, kCanCatchMaxScale, 1.0f);
			}
			else
			{
				// タイムを過ぎたら、タイマーをリセット
				m_canCatchEnemyTimer = 0.0f;
			}

			// タイマーを進める
			m_canCatchEnemyTimer += nsTimer::GameTime().GetFrameDeltaTime();

			// 敵を捕まえることができる合図のスプライトを更新する
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				// スプライトの回転
				Quaternion qRot = Quaternion::Identity;
				// スプライトの拡大率
				Vector3 scale = Vector3::One;
				if (i == enCanCatchEnemyFrame1)
				{
					// 枠1の時は、回転を変更する。
					qRot.SetRotationDegZ(Frame1angle);
				}
				else if (i == enCanCatchEnemyFrame2)
				{
					// 枠2の時は、拡大率を変更する。
					scale.Scale(Frame2scale);
				}
				m_canCatchEnemySR[i]->SetPosition(m_taraget2DPos);
				m_canCatchEnemySR[i]->SetRotation(qRot);
				m_canCatchEnemySR[i]->SetScale(scale);

				// 表示する
				m_canCatchEnemySR[i]->Activate();
			}

			return;
		}

		/**
		 * @brief ターゲットを示すスプライトの更新
		*/
		void CEnemyCatchUI::UpdateTargetSprite()
		{
			if (m_targetRef == nullptr ||
				m_targetLength <= kCanCatchMaxLength)
			{
				// ターゲットが見つかっていない、または、
				// ターゲットとの距離が一定以下のとき

				// 非表示
				m_targetSR->Deactivate();
				// 早期リターン
				return;
			}

			// 補間率
			float t = (m_targetLength - kCanCatchMaxLength) / (kTargetMaxLength - kCanCatchMaxLength);
			// 拡大率が低い値の時が多くなるように、反転させる。
			t = 1.0f - t;
			// 指数関数的変化にさせる。
			t = pow(t, kTargetSpritePowPower);
			// 拡大率を線形補完で求める
			float scale = Math::Lerp<float>(t, kTargetSpriteMinScale, kTargetSpriteMaxScale);

			// 表示する
			m_targetSR->Activate();

			m_targetSR->SetPosition(m_taraget2DPos);
			m_targetSR->SetScale(scale);

			return;
		}

		/**
		 * @brief ターゲットとの距離を示すフォントの更新
		*/
		void CEnemyCatchUI::UpdateTargetLengthFont()
		{
			if (m_targetRef == nullptr ||
				m_targetLength <= kCanCatchMaxLength)
			{
				// ターゲットが見つかっていない、または、
				// ターゲットとの距離が一定以下のとき

				// 非表示
				m_targetLengthFR->Deactivate();
				// 早期リターン
				return;
			}

			// 表示
			m_targetLengthFR->Activate();

			// 表示するテキスト
			wchar_t text[16];
			// 単位（ m）
			wchar_t unit[16];
			// ターゲットまでの距離。デフォルトではcmなのでmに直す。
			const float targetLength = m_targetLength * 0.1f;
			// 4桁、小数なしの数字
			swprintf_s(text, L"%4.0f", targetLength);
			// 単位
			swprintf_s(unit, L" m");
			// 単位をテキストに足す。
			wcscat(text, unit);
			// テキストをフォントレンダラーに設定
			m_targetLengthFR->SetText(text);
			// 座標を計算する。
			Vector2 pos = m_taraget2DPos;
			pos.x += kTargetLengthFontRenderPosBuff.x;
			pos.y += kTargetLengthFontRenderPosBuff.y;

			// フォントの座標を設定する
			m_targetLengthFR->SetPosition(pos);

			return;
		}

	}
}