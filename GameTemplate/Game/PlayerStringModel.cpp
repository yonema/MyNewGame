#include "stdafx.h"
#include "PlayerStringModel.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "GameTime.h"

namespace nsMyGame
{


	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// プレイヤーの糸クラスの定数データを使用可能にする
		using namespace nsPlayerConstData::nsStringModelConstData;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CPlayerStringModel::Start()
		{
			// モデルレンダラーの生成
			m_modelRender = NewGO <nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			// 初期化
			m_modelRender->Init(kStringModelFilePath);
			// 無効化する
			m_modelRender->Deactivate();

			// モデルレンダラーの生成
			m_tipModelRender = NewGO <nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			// 初期化
			m_tipModelRender->Init(kStringModelFilePath);
			// 無効化する
			m_tipModelRender->Deactivate();

			return true;
		}

		/**
		 * @brief 更新処理
		*/
		void CPlayerStringModel::OnDestroy()
		{
			DeleteGO(m_modelRender);
			DeleteGO(m_tipModelRender);

			return;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CPlayerStringModel::Update()
		{
			// 伸びる開始座標の計算
			m_startStretchPos = m_playerRef->GetPosition();
			m_startStretchPos.y += kStartStretchHeight;

			// モデルを回転させる処理
			ModelRotation();
			
			// 伸ばす処理

			// 伸ばす率を増やす
			if (m_isStretched != true)
			{
				m_stretchRate += nsTimer::GameTime().GetFrameDeltaTime() / kStretchedTime;
			}

			if (m_stretchRate >= 1.0f)
			{
				// 伸びきった
				m_isStretched = true;
				// 最大率を上限にする
				m_stretchRate = 1.0f;
			}

			// 伸ばす座標へのベクトル
			const Vector3 toStretchPosVec = m_toStretchPos - m_startStretchPos;
			// 伸ばす座標への距離
			const float toStretchPosDist = toStretchPosVec.Length();
			// 伸ばす率で、伸ばす長さを決める
			float stretchLength = Math::Lerp<float>(m_stretchRate, 0.0f, toStretchPosDist);

			// 手元側のモデルの拡大率
			float atHandModelScale = stretchLength;

			// 手元側のモデルの拡大率が最大値より大きいか？
			if (atHandModelScale > 500.0f)
			{
				// 最大値に固定する
				atHandModelScale = 500.0f;

				// 先端側のモデルを有効化する
				m_tipModelRender->Activate();

				// 先端側のモデルの拡大率を計算
				float tipModelScale = stretchLength - atHandModelScale;
				m_tipModelRender->SetScale({ 1.0f,1.0f,tipModelScale });

				Vector3 toTipVec = Vector3::Front;
				m_tipModelRender->GetRotation().Apply(toTipVec);
				toTipVec.Normalize();
				toTipVec.Scale(500.0f - 60.0f);
				m_tipModelRender->SetPosition(m_startStretchPos + toTipVec);
			}

			// モデルの拡大率と座標を設定する
			m_modelRender->SetScale({1.0f,1.0f,atHandModelScale });
			m_modelRender->SetPosition(m_startStretchPos);
						
			return;
		}

		/**
		 * @brief 指定した座標に向かって伸びる処理を開始する
		 * @param[in] pos 伸びる先の座標
		*/
		void CPlayerStringModel::StartStretchToPos(const Vector3& pos)
		{
			// 伸びきっていないようにする
			m_isStretched = false;

			// 伸びる先の座標を設定する
			m_toStretchPos = pos;

			// 伸びる先へのベクトル
			Vector3 toStretchVec = m_toStretchPos - m_startStretchPos;
			// 伸びる速度を設定する
			m_stretchSpeed =
				toStretchVec.Length() * nsTimer::CGameTime().GetFrameDeltaTime() / kStretchedTime;

			// 自身を有効化する
			Activate();

			// モデルを有効化する
			m_modelRender->Activate();

			// モデルを回転させる処理
			ModelRotation();

			// モデルの拡大率を初期化
			m_modelRender->SetScale(Vector3::One);
			// モデルの座標を設定する
			m_modelRender->SetPosition(m_startStretchPos);

			return;
		}

		/**
		 * @brief 指定した座標に向かって伸びる処理を終了する
		*/
		void CPlayerStringModel::EndStretchToPos()
		{
			// 伸びきっているようにする
			m_isStretched = true;

			// 自身を無効化する
			Deactivate();

			// モデルを無効化する
			m_modelRender->Deactivate();

			m_tipModelRender->Deactivate();

			m_stretchRate = 0.0f;

			return;
		}

		/**
		 * @brief モデルを回転させる処理
		*/
		void CPlayerStringModel::ModelRotation()
		{
			// 伸ばす先への方向
			Vector3 toStretchPosDir = m_toStretchPos - m_startStretchPos;
			// 正規化する
			toStretchPosDir.Normalize();

			// 前方向と伸ばす先への方向の外積で、回転軸を求める。
			Vector3 rotAxis = Cross(Vector3::Front, toStretchPosDir);
			// 正規化する
			rotAxis.Normalize();

			// 前方向と伸ばす先への方向の内積を求める
			const float dotFrontAndToStretchPosDir = Dot(Vector3::Front, toStretchPosDir);
			// 内積からラジアン角を求める
			const float radAngle = acosf(dotFrontAndToStretchPosDir);

			// 回転クォータニオン
			Quaternion qRot;
			// 任意の軸周りで回転させる
			qRot.SetRotation(rotAxis, radAngle);

			// モデルに回転を設定する
			m_modelRender->SetRotation(qRot);

			m_tipModelRender->SetRotation(qRot);

			return;
		}



	}
}