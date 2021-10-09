#include "stdafx.h"
#include "PlayerStringModel.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "ModelRender.h"

namespace nsMyGame
{


	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// プレイヤーの糸クラスの定数データを使用可能にする
		using namespace nsPlayerConstData::nsPlayerStringModelConstData;

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

			return true;
		}

		/**
		 * @brief 更新処理
		*/
		void CPlayerStringModel::OnDestroy()
		{
			DeleteGO(m_modelRender);

			return;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CPlayerStringModel::Update()
		{
			// 伸びきっているか？
			if (m_isStretched == true)
			{
				// 伸びきってる
				// 早期リターン
				return;
			}

			// 伸びきってない

			// モデルを回転させる処理
			ModelRotation();
			
			// 伸ばす処理

			Vector3 scale = m_modelRender->GetScale();

			scale.z += 50.0f;

			Vector3 distVec = m_toStretchPos - m_playerRef->GetPosition();

			const float distLen = distVec.Length();

			if (scale.z > distLen)
			{
				scale.z = distLen;
			}

			m_modelRender->SetScale(scale);
			m_modelRender->SetPosition(m_playerRef->GetPosition());

			



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

			// 自身を有効化する
			Activate();

			// モデルを有効化する
			m_modelRender->Activate();

			// モデルを回転させる処理
			ModelRotation();

			m_modelRender->SetScale(Vector3::One);
			m_modelRender->SetPosition(m_playerRef->GetPosition());

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

			return;
		}

		/**
		 * @brief モデルを回転させる処理
		*/
		void CPlayerStringModel::ModelRotation()
		{
			// 伸ばす先への方向
			Vector3 toStretchPosDir = m_toStretchPos - m_playerRef->GetPosition();
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

			return;
		}



	}
}