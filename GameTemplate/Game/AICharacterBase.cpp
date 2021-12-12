#include "stdafx.h"
#include "AICharacterBase.h"
#include "ModelRender.h"
#include "NaviMesh.h"
#include "PathFinding.h"

namespace nsMyGame
{
	/**
	 * @brief AIキャラクター関連のネームスペース
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CAICharacterBase::Start()
		{
			// 基底クラスの開始処理
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// 派生クラスの開始処理
			return StartSub();
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CAICharacterBase::OnDestroy()
		{
			// 派生クラスの消去される時に呼ばれる処理
			OnDestroySub();

			// 基底クラスの消去される時に呼ばれる処理
			DeleteGO(m_modelRender);

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CAICharacterBase::Update()
		{
			// 基底クラスの更新処理

			// 派生クラスの更新処理
			UpdateSub();

			return;
		}

		/**
		 * @brief
		 * @param filePath
		 * @param pos
		 * @param rot
		 * @param scale
		*/
		void CAICharacterBase::InitModel(
			const char* filePath,
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale
		)
		{
			m_modelRender->SetPosition(pos);
			m_modelRender->SetRotation(rot);
			m_modelRender->SetScale(scale);
			m_modelRender->Init(filePath);

			return;
		}

		/**
		 * @brief パス検索処理
		 * @param[in] targetPos 移動目標座標
		*/
		void CAICharacterBase::PathFinding(const Vector3& targetPos)
		{
			// パス検索
			m_pathFindingRef->Execute(
				m_path,							// 構築されたパスの格納先
				*m_naviMeshRef,					// ナビメッシュ
				m_modelRender->GetPosition() ,	// 開始座標
				targetPos,						// 移動目標座標
				PhysicsWorld::GetInstance(),	// 物理エンジン	
				50.0f,							// AIエージェントの半径
				200.0f							// AIエージェントの高さ。
			);

			m_isEndPathMove = false;

			return;
		}

		/**
		 * @brief パス上を移動する
		 * @param[in] moveSpeed 移動速度
		 * @param[in] physicsWorld 物理ワールド。
		*/
		void CAICharacterBase::MoveOnPath(
			const float moveSpeed,
			const PhysicsWorld* physicsWorld
		)
		{
			if (m_isEndPathMove == true)
			{
				return;
			}

			Vector3 pos = m_path.Move(m_modelRender->GetPosition(), moveSpeed, m_isEndPathMove, physicsWorld);
			m_modelRender->SetPosition(pos);

			return;
		}

	}
}