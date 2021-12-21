#include "stdafx.h"
#include "MiniMap.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "GameMainState.h"
#include "UIConstData.h"
#include "Player.h"
#include "RenderingEngine.h"

namespace nsMyGame
{
	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		using namespace nsGameMainUIConstData;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CMiniMap::Start()
		{
			// ゲームステートの参照を引っ張ってくる
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enMainGameState]
				);
		
			// ミニマップのスプライトの初期化
			InitMiniMapSprite();

			// プレイヤーのアイコンのスプライトの初期化
			InitPlayerIconSprite();

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CMiniMap::OnDestroy()
		{
			DeleteGO(m_miniMapSR);
			DeleteGO(m_miniMapBackSR);
			DeleteGO(m_playerIconSR);

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CMiniMap::Update()
		{
			// ミニマップの基点を更新
			UpdateMiniMapPivot();

			// ミニマップの回転を更新
			UpdateMiniMapRotate();

			// プレイヤーのアイコンを回転を更新
			UpdatePlayerIconRotate();

			return;
		}


		/**
		 * @brief ミニマップのスプライトの初期化
		*/
		void CMiniMap::InitMiniMapSprite()
		{
			// ミニマップの背景用スプライトの生成と初期化
			m_miniMapBackSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);
			m_miniMapBackSR->Init(kMiniMapBackSpriteFilePath, kMiniMapDrawWidth, kMiniMapDrawHeight);

			// ミニマップのスプライトの生成
			m_miniMapSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);

			// スプライトの定数データを使用可能にする
			using namespace nsGraphic::nsSprite::nsSpriteConstData;
			// ミニマップ用のスプライトの初期化データ
			SpriteInitData initData;
			initData.m_ddsFilePath[0] = kMiniMapSpriteFilePath;
			initData.m_width = kMiniMapSpriteWidth;
			initData.m_height = kMiniMapSpriteHeight;
			initData.m_fxFilePath = kMiniMapFxFilePath;
			initData.m_psEntryPoinFunc = kPsEntryPointFunc;
			initData.m_vsEntryPointFunc = kVsEntryPointFunc;
			// メインレンダリングターゲットのカラーバッファーフォーマットと合わせる
			initData.m_colorBufferFormat[0] = nsMyEngine::CRenderingEngine::GetInstance()->GetMainRenderTargetFormat();
			// ミニマップの最小座標と最大座標を計算
			CalcMinAndMaxPos();
			// 定数バッファを設定
			initData.m_expandConstantBuffer[0] = &m_miniMapSpriteCB;
			initData.m_expandConstantBufferSize[0] = sizeof(m_miniMapSpriteCB);
			// ミニマップのスプライトの初期化
			m_miniMapSR->Init(initData);

			// 座標を設定
			m_miniMapBackSR->SetPosition(kMiniMapSpritePosition);
			m_miniMapSR->SetPosition(kMiniMapSpritePosition);

			return;
		}

		/**
		 * @brief プレイヤーのアイコンのスプライトの初期化
		*/
		void CMiniMap::InitPlayerIconSprite()
		{
			// プレイヤーのアイコンのスプライトの生成と初期化
			m_playerIconSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);
			m_playerIconSR->Init(
				kPlayerIconSpriteFilePath,
				kPlayerIconSpriteWidth,
				kPlayerIconSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			m_playerIconSR->SetPosition(kPlayerIconSpritePosition);

			return;
		}

		/**
		 * @brief ミニマップの最小座標と最大座標を計算
		*/
		void CMiniMap::CalcMinAndMaxPos()
		{
			// ミニマップのスプライトの中心座標を計算
			Vector2 centerPos = {
				g_graphicsEngine->GetFrameBufferWidth() * 0.5f,
				g_graphicsEngine->GetFrameBufferHeight() * 0.5f
			};
			centerPos.x += kMiniMapSpritePosition.x;
			centerPos.y -= kMiniMapSpritePosition.y;
			// ミニマップの最小座標を計算
			m_miniMapSpriteCB.minPos = centerPos;
			m_miniMapSpriteCB.minPos.x -= kMiniMapDrawWidth * 0.5f;
			m_miniMapSpriteCB.minPos.y -= kMiniMapDrawHeight * 0.5f;
			// ミニマップの最大座標を計算
			m_miniMapSpriteCB.maxPos = centerPos;
			m_miniMapSpriteCB.maxPos.x += kMiniMapDrawWidth * 0.5f;
			m_miniMapSpriteCB.maxPos.y += kMiniMapDrawHeight * 0.5f;

			return;
		}

		/**
		 * @brief ミニマップの基点を更新
		*/
		void CMiniMap::UpdateMiniMapPivot()
		{
			const Vector3& playerPos = m_gameState->GetPlayer().GetPosition();

			float xRate = (playerPos.x + 35000.0f) / 70000.0f;
			float yRate = (playerPos.z + 2800.0f) / 70000.0f;

			m_miniMapSR->SetPivot({ xRate, yRate });

			return;
		}

		/**
		 * @brief ミニマップの回転を更新
		*/
		void CMiniMap::UpdateMiniMapRotate()
		{
			// Y成分を抜いたカメラの前方向
			Vector3 cameraForward = g_camera3D->GetForward();
			cameraForward.y = 0.0f;
			cameraForward.Normalize();

			// ラジアン角度を出す
			float angle = acosf(Dot(Vector3::Front, cameraForward));

			Quaternion qRot = Quaternion::Identity;
			// Xが負の時、反転させる
			if (cameraForward.x <= 0.0f)
			{
				angle = 3.14f * 2.0f - angle;
			}
			qRot.SetRotationZ(angle);

			// ミニマップを回転させる
			m_miniMapSR->SetRotation(qRot);

			return;
		}

		/**
		 * @brief プレイヤーのアイコンを回転を更新
		*/
		void CMiniMap::UpdatePlayerIconRotate()
		{
			// Y成分を抜いたカメラの前方向
			Vector3 cameraForward = g_camera3D->GetForward();
			cameraForward.y = 0.0f;
			cameraForward.Normalize();

			// Y成分を抜いたプレイヤーの前方向
			const Quaternion& playerRot = m_gameState->GetPlayer().GetRotation();
			Vector3 playerForward = Vector3::Front;
			playerRot.Apply(playerForward);
			playerForward.y = 0.0f;
			playerForward.Normalize();

			// ラジアン角度を出す
			float angle = acosf(Dot(playerForward, cameraForward));

			// カメラの右方向のベクトル
			Vector3 CameraRight = Cross(Vector3::Up, cameraForward);
			// プレイヤーの前方向とカメラの右方向の内積を取る
			float dotPlForwardAndCamRight = Dot(playerForward, CameraRight);

			Quaternion qRot = Quaternion::Identity;
			// プレイヤーの前方向が、カメラの右方向と同じ方向を向いていたら、反転する。
			if (dotPlForwardAndCamRight >= 0.0f)
			{
				angle = 3.14f * 2.0f - angle;
			}
			qRot.SetRotationZ(angle);

			// ミニマップを回転させる
			m_playerIconSR->SetRotation(qRot);

			return;
		}

	}
}