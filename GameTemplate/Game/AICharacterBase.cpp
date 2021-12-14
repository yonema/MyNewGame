#include "stdafx.h"
#include "AICharacterBase.h"
#include "ModelRender.h"
#include "NaviMesh.h"
#include "PathFinding.h"
#include "GameTime.h"

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

			// 非決定的な乱数生成器でシード生成機を生成
			std::random_device rnd;
			// メルセンヌツイスターの32ビット版、引数は初期シード
			m_mt = std::make_unique<std::mt19937>(rnd());
			// [0, 99] 範囲の一様乱数
			m_rand = std::make_unique<std::uniform_int_distribution<>>(0, 99);

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
		 * @param[in] targetPos パスの目標座標
		*/
		void CAICharacterBase::PathFinding(const Vector3& targetPos)
		{
			// パス検索
			m_pathFindingRef->Execute(
				m_path,							// 構築されたパスの格納先
				*m_naviMeshRef,					// ナビメッシュ
				m_modelRender->GetPosition() ,	// 開始座標
				targetPos,						// 移動目標座標
				//PhysicsWorld::GetInstance(),	// 物理エンジン
				nullptr,
				5.0f,							// AIエージェントの半径
				200.0f							// AIエージェントの高さ。
			);

			m_isEndPathMove = false;

			return;
		}

		/**
		 * @brief ターゲットの中からランダムでパス検索処理
		*/
		void CAICharacterBase::RandomTargetPathFinding()
		{
			const int randTargetNum = (*m_rand)(*m_mt);
			const Vector3 randTargetPos = (*m_naviMeshTargetPointsRef)[randTargetNum];

			PathFinding(randTargetPos);
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

			const Vector3 posBeforeMove = m_modelRender->GetPosition();
			const Vector3 posAfterMove = m_path.Move(
				posBeforeMove,
				moveSpeed * nsTimer::CGameTime().GetFrameDeltaTime(),
				m_isEndPathMove,
				physicsWorld
			);
			Quaternion frontQRot = Quaternion::Identity;


			Rotating(&frontQRot, posBeforeMove, posAfterMove);

			m_modelRender->SetPosition(posAfterMove);
			m_modelRender->SetRotation(frontQRot);

			return;
		}

		/**
		 * @brief 移動方向に回転する
		 * @param[in] frontQRotOut 前方向の回転の格納先
		 * @param[in] posBefoerMove 移動前の座標
		 * @param[in] posAfterMove 移動後の座標
		*/
		void CAICharacterBase::Rotating(Quaternion* frontQRotOut, const Vector3& posBeforeMove, const Vector3& posAfterMove)
		{
			Vector3 moveDir = posAfterMove - posBeforeMove;
			if (moveDir.LengthSq() <= 1.0f)
			{
				return;
			}
			moveDir.Normalize();
			frontQRotOut->SetRotation(Vector3::Front, moveDir);

			return;
		}

	}
}