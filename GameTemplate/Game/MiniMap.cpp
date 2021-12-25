#include "stdafx.h"
#include "MiniMap.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "GameMainState.h"
#include "UIConstData.h"
#include "Player.h"
#include "RenderingEngine.h"
#include "AICar.h"

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

			// 車たちの参照を引っ張ってくる
			m_aiCarsRef = m_gameState->GetAICar();

			// プレイヤーの座標の参照を引っ張ってくる
			m_playerPositionRef = &m_gameState->GetPlayer().GetPosition();
		
			// ミニマップのスプライトの初期化
			InitMiniMapSprite();

			// プレイヤーのアイコンのスプライトの初期化
			InitPlayerIconSprite();

			// 車のアイコンのスプライトの初期化
			InitCarIconSprite();

			// 車のミニアイコンのスプライトの初期化
			InitCarMiniIconSprite();

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

			for (auto& carIcon : m_carIconSRs)
			{
				DeleteGO(carIcon);
			}

			for (auto& carMiniIcon : m_carMiniIconSRs)
			{
				DeleteGO(carMiniIcon);
			}

			for (auto& carMiniIconOut : m_carMiniIconOutSRs)
			{
				DeleteGO(carMiniIconOut);
			}

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CMiniMap::Update()
		{

			// クラス内のいろんなところで共通して使用されるデータの更新
			UpdateCommonData();

			// ミニマップの基点を更新
			UpdateMiniMapPivot();

			// ミニマップの回転を更新
			UpdateMiniMapRotate();

			// プレイヤーのアイコンを回転を更新
			UpdatePlayerIconRotate();

			// 車のアイコンを更新
			UpdateCarIcon();

			// 車のミニアイコンを更新
			UpdateCarMiniIcon();

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

			// 座標は最初の位置で固定
			m_playerIconSR->SetPosition(kPlayerIconSpritePosition);

			return;
		}

		/**
		 * @brief 車のアイコンのスプライトの初期化
		*/
		void CMiniMap::InitCarIconSprite()
		{
			// 車の数だけリサーブ
			m_carIconSRs.reserve(m_aiCarsRef.size());

			// 車の数だけスプライトを生成
			for (int i = 0; i < m_aiCarsRef.size();i++)
			{
				m_carIconSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond));
				m_carIconSRs[i]->Init(
					kCarIconSpriteFilePath,
					kCarIconSpriteWidth,
					kCarIconSpriteHeight, 
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
			}


			return;
		}

		/**
		 * @brief 車のミニアイコンのスプライトの初期化
		*/
		void CMiniMap::InitCarMiniIconSprite()
		{
			// 車の数だけリサーブ
			m_carMiniIconSRs.reserve(m_aiCarsRef.size());

			// 車の数だけスプライトを生成
			for (int i = 0; i < m_aiCarsRef.size(); i++)
			{
				m_carMiniIconSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond));
				m_carMiniIconSRs[i]->Init(
					kCarMiniIconSpriteFilePath,
					kCarMiniIconSpriteWidth,
					kCarMiniIconSpriteHeight,
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				m_carMiniIconOutSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond));
				m_carMiniIconOutSRs[i]->Init(
					kCarMiniIconOutSpriteFilePath,
					kCarMiniIconOutSpriteWidth,
					kCarMiniIconOutSpriteHeight,
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
			}

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
		 * @brief クラス内のいろんなところで共通して使用されるデータの更新
		*/
		void CMiniMap::UpdateCommonData()
		{
			// XZ平面でのカメラの前方向更新
			m_cameraForwardXZ = g_camera3D->GetForward();
			// Y成分を消去
			m_cameraForwardXZ.y = 0.0f;
			// 正規化しなおす
			m_cameraForwardXZ.Normalize();

			// 上方向とカメラのXZ平面の前方向の外積から、
			// XZ平面のカメラの右方向のベクトルを作成
			m_cameraRightXZ = Cross(Vector3::Up, m_cameraForwardXZ);
			// 正規化する
			m_cameraRightXZ.Normalize();

			return;
		}

		/**
		 * @brief ミニマップの基点を更新
		*/
		void CMiniMap::UpdateMiniMapPivot()
		{
			// ステージの幅とプレイヤーの座標から、
			// プレイヤーがステージのどの辺にいるのか、正規化した値を出す
			float xRate = (m_playerPositionRef->x + kPlayerCenterBuffWidth) / kStageWidth;
			float yRate = (m_playerPositionRef->z + kPlayerCenterBuffHeight) / kStageHeight;

			// プレイヤーの位置をマップの中心にするため、マップの基点をプレイヤーの位置に合わせる
			m_miniMapSR->SetPivot({ xRate, yRate });

			return;
		}

		/**
		 * @brief ミニマップの回転を更新
		*/
		void CMiniMap::UpdateMiniMapRotate()
		{
			// ラジアン角度を出す
			float angle = acosf(Dot(Vector3::Front, m_cameraForwardXZ));

			// Xが負の時、反転させる
			if (m_cameraForwardXZ.x <= 0.0f)
			{
				angle = 3.14f * 2.0f - angle;
			}

			// 角度からZ軸周りのクォータニオンを設定
			m_miniMapRotatioin.SetRotationZ(angle);

			// ミニマップを回転させる
			m_miniMapSR->SetRotation(m_miniMapRotatioin);

			return;
		}

		/**
		 * @brief プレイヤーのアイコンを回転を更新
		*/
		void CMiniMap::UpdatePlayerIconRotate()
		{
			// プレイヤーのXZ平面での前方向のベクトルを作成する

			// プレイヤーの回転情報
			const Quaternion& playerRot = m_gameState->GetPlayer().GetRotation();
			// Y成分を抜いたプレイヤーの前方向
			Vector3 playerForward = Vector3::Front;
			// 回転で前方向ベクトルを回す
			playerRot.Apply(playerForward);
			// Y成分を消去
			playerForward.y = 0.0f;
			// 正規化する
			playerForward.Normalize();

			// プレイヤーの前方向とカメラの前方向のラジアン角度を出す
			float angle = acosf(Dot(playerForward, m_cameraForwardXZ));

			// プレイヤーの前方向とカメラの右方向の内積を取る
			float dotPlForwardAndCamRight = Dot(playerForward, m_cameraRightXZ);

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

		/**
		 * @brief 車のアイコンを更新
		*/
		void CMiniMap::UpdateCarIcon()
		{
	
			// インデックス
			int i = 0;
			for (const auto& carIconSR : m_carIconSRs)
			{
				Vector2 iconPos = Vector2::Zero;

				g_camera3D->CalcScreenPositionFromWorldPosition(
					iconPos,
					m_aiCarsRef[i++]->GetPosition()
				);
				carIconSR->SetPosition(iconPos);
			}

			return;
		}

		/**
		 * @brief 車のミニアイコンを更新
		*/
		void CMiniMap::UpdateCarMiniIcon()
		{
			// 車全部分更新
			int i = 0;
			for (const auto& carMiniIconSR : m_carMiniIconSRs)
			{
				//////// 1.車のアイコンの座標を計算する ////////

				// プレイヤーから車へのベクトル
				Vector3 playerToCarVec = m_aiCarsRef[i]->GetPosition() - *m_playerPositionRef;
				// 正規化されたプレイヤーから車へのベクトル
				float xRate = (playerToCarVec.x) / kStageWidth;
				float yRate = (playerToCarVec.z) / kStageHeight;

				// 正規化されたベクトルから、マップに合わせたベクトルに変換
				Vector3 toCarVecOnMap = Vector3::Zero;
				toCarVecOnMap.x = kMiniMapSpriteWidth * xRate;
				toCarVecOnMap.y = kMiniMapSpriteHeight * yRate;
				// ミニマップの回転でベクトルを回す
				m_miniMapRotatioin.Apply(toCarVecOnMap);
				// 車のアイコンの座標
				// プレイヤーのアイコンの座標から、マップに合わせた車へのベクトルを足す。
				Vector2 iconPos = kPlayerIconSpritePosition;
				iconPos.x += toCarVecOnMap.x;
				iconPos.y += toCarVecOnMap.y;

				//////// 2.車の座標がミニマップ外なら端っこに固定する ////////
 
				// プレイヤーアイコンから車アイコンへの線分のベクトル
				Vector2 pIconToCIconVec = Vector2::Zero;
				pIconToCIconVec.x = iconPos.x - kPlayerIconSpritePosition.x;
				pIconToCIconVec.y = iconPos.y - kPlayerIconSpritePosition.y;

				// 交差しているか？
				bool isIntersect = false;
				// 線分と、ミニマップの四角形の当たり判定を取る
				for (int i = 0; i < 4; i++)
				{
					// Vector2同士の交差判定
					isIntersect = IsIntercetVector2ToVector2(
						kPlayerIconSpritePosition,
						pIconToCIconVec,
						kMiniMapVert[i],
						kMiniMapEdge[i],
						&iconPos
					);
					if (isIntersect)
					{
						// 交差していたら、即ループを抜ける
						break;
					}
				}

				// 車のミニアイコンの座標を設定する
				carMiniIconSR->SetPosition(iconPos);

				//////// 3.車のミニアイコンの画面はみだし用を更新する ////////

				// 車のミニアイコンの画面はみだし用を更新
				UpdateCarMiniIconOut(i, isIntersect, playerToCarVec, iconPos);

				i++;
			}

			return;
		}

		/**
		 * @brief 車のミニアイコンの画面はみだし用を更新。
		 * この関数は、UpdateCarMiniIconで呼ばれる。
		 * @param[in] index 車のインデックス
		 * @param[in] isIntersect 交差しているか？
		 * @param[in] playerToCarVec プレイヤーから車へのベクトル
		 * @param[in] iconPos 車のアイコンの座標
		*/
		void CMiniMap::UpdateCarMiniIconOut(
			const int index,
			const bool isIntersect,
			const Vector3& playerToCarVec,
			const Vector2& iconPos)
		{
			if (isIntersect != true)
			{
				// 交差してなかったら非表示
				m_carMiniIconOutSRs[index]->Deactivate();
				// 早期リターン
				return;
			}

			// 交差していたら表示
			m_carMiniIconOutSRs[index]->Activate();

			// XZ平面でのプレイヤーから車への方向ベクトル
			Vector3 playerToCarXZVec = playerToCarVec;
			playerToCarXZVec.y = 0.0f;
			playerToCarXZVec.Normalize();

			// ラジアン角度を出す
			float angle = acosf(Dot(playerToCarXZVec, m_cameraForwardXZ));

			// レイヤーから車への方向とカメラの右方向の内積を取る
			float dotPlToCarAndCamRight = Dot(playerToCarXZVec, m_cameraRightXZ);

			Quaternion qRot = Quaternion::Identity;
			// プレイヤーの前方向が、カメラの右方向と同じ方向を向いていたら、反転する。
			if (dotPlToCarAndCamRight >= 0.0f)
			{
				angle = 3.14f * 2.0f - angle;
			}
			qRot.SetRotationZ(angle);

			// 普通の車アイコンとちょっとずらす
			Vector3 up = Vector3::Up;
			qRot.Apply(up);
			up.Scale(kCarMiniIconOutBuff);
			Vector2 iconOutPos = iconPos;
			iconOutPos.x += up.x;
			iconOutPos.y += up.y;

			m_carMiniIconOutSRs[index]->SetRotation(qRot);
			m_carMiniIconOutSRs[index]->SetPosition(iconOutPos);

			return;
		}

	}
}