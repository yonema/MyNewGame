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
		// AIキャラクターの定数データを使用可能にする
		using namespace nsAICharacterConstData;

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
			// 安全運転時に使用する範囲付きの一様乱数
			m_randForSafeDrive = std::make_unique<std::uniform_int_distribution<>>(0, 2);

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

			return;
		}

		/**
		 * @brief 安全運転を心掛けたパス検索処理
		*/
		void CAICharacterBase::TargetPathFindingToSafeDrive()
		{
			// ターゲットポイントの候補
			std::unordered_map<EnCandidateTargetPointType,Vector3> candidateTargetPoints;
			// 自身から右折用のターゲットポイントへの距離
			float posToTurnRightTPLen[enCandidateTargetPointTypeNum] = {};
			for (int candidateTPType = 0; candidateTPType < enCandidateTargetPointTypeNum; candidateTPType++)
			{
				switch (candidateTPType)
				{
				case enTurnRight:
					posToTurnRightTPLen[candidateTPType] = FLT_MIN;
					break;
				case enTurnLeft:
				case enGoStraight:
					posToTurnRightTPLen[candidateTPType] = FLT_MAX;
					break;
				}
			}

			// ターゲットポイントの中から次の移動対象のターゲットポイントを探し出す。
			for (const auto& targetPos : *m_naviMeshTargetPointsRef)
			{
				// 除外するか？
				bool isExclude = false;

				// 自身の座標からターゲット座標へのベクトル
				const Vector3 posToTargetPosVec = targetPos - m_modelRender->GetPosition();
				// 自身の座標からターゲット座標への距離
				const float posToTargetPosLen = posToTargetPosVec.Length();
				// 移動方向とターゲット座標への方向のラジアン角
				float moveToPTTAngleRad = 0.0f;

				//////// 1.一定以上の距離があるターゲット座標を除外する ////////

				// 距離によるターゲットポイントの除外
				isExclude = ExcludeForDistance(posToTargetPosLen);

				// 除外
				if (isExclude)
				{
					continue;
				}

				//////// 2. 一定以上の角度より上のターゲットは除外 ////////

				// 自身の座標からターゲット座標への方向ベクトル
				Vector3 posToTargetPosDir = posToTargetPosVec;
				posToTargetPosDir.Normalize();

				// 角度によるターゲットポイントの除外
				isExclude = ExcludeForAngle(&moveToPTTAngleRad, posToTargetPosDir);

				// 除外
				if (isExclude)
				{
					continue;
				}

				//////// 3.候補に入れるターゲットがあれば候補に入れる ////////

				BeCandidateTargetPoint(
					&candidateTargetPoints,
					targetPos,
					posToTurnRightTPLen,
					posToTargetPosLen,
					posToTargetPosDir
				);
			}

			// 候補が見つからなかった場合は何もしない
			if (candidateTargetPoints.empty())
			{
				return;
			}

			// ターゲットポイントを探せたか？
			bool IsFindedTargetPoint = false;
			EnCandidateTargetPointType randCandidateTPType = enTurnRight;

			while (IsFindedTargetPoint == false)
			{
				// ランダムなターゲットポイントの番号
				int randTargetNum = (*m_randForSafeDrive)(*m_mt);
				randCandidateTPType = static_cast<EnCandidateTargetPointType>(randTargetNum);

				// あったらターゲットポイントを探せた
				if (candidateTargetPoints.count(randCandidateTPType))
				{
					IsFindedTargetPoint = true;
				}
			}

			PathFinding(candidateTargetPoints[randCandidateTPType]);

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
		 * @brief 初期化処理
		 * @param[in] naviMeshRef ナビメッシュ
		 * @param[in,,out] pathFindingRef パス検索処理
		*/
		void CAICharacterBase::Init(nsAI::SAICharacterInitData* AICharaInitData)
		{
			m_naviMeshRef = AICharaInitData->naviMeshRef;
			m_pathFindingRef = AICharaInitData->pathFindingRef;
			m_naviMeshTargetPointsRef = AICharaInitData->naviMeshTargetPointsRef;
		}

		/**
		 * @brief 移動方向に回転する
		 * @param[in] frontQRotOut 前方向の回転の格納先
		 * @param[in] posBefoerMove 移動前の座標
		 * @param[in] posAfterMove 移動後の座標
		*/
		void CAICharacterBase::Rotating(Quaternion* frontQRotOut, const Vector3& posBeforeMove, const Vector3& posAfterMove)
		{
			m_moveDir = posAfterMove - posBeforeMove;
			m_moveDir.y = 0.0f;
			const float moveLenSq = m_moveDir.LengthSq();
			m_moveDir.Normalize();

			if (moveLenSq <= 10.0f)
			{
				return;
			}

			frontQRotOut->SetRotation(Vector3::Front, m_moveDir);

			return;
		}

		/**
		 * @brief 距離によるターゲットポイントの除外
		 * @param distance 自身とターゲットポイントの距離
		 * @return 除外したか？true戻ってきたら除外した
		*/
		bool CAICharacterBase::ExcludeForDistance(const float distance)
		{
			// 距離が一定以上のターゲットを除外
			if (distance > kBetweenTargetPoint)
			{
				return true;
			}

			return false;
		}

		/**
		 * @brief 角度によるターゲットポイントの除外
		 * @param[out] moveToPTTAngleRad  移動方向とターゲット座標への方向のラジアン角
		 * @param[in] posToTargetPosDir 自身からターゲットへのベクトル
		 * @return 除外したか？trueが戻ってきたら除外した
		*/
		bool CAICharacterBase::ExcludeForAngle(float* moveToPTTAngleRadOut, const Vector3& posToTargetPosDir)
		{
			// 移動方向とターゲット座標への方向を比べる
			const float dotMoveAndPTTDir = Dot(m_moveDir, posToTargetPosDir);
			// 移動方向とターゲット座標への方向のラジアン角
			*moveToPTTAngleRadOut = acosf(dotMoveAndPTTDir);

			// 一定の角度より上のターゲットは対象外
			if (*moveToPTTAngleRadOut > kAngleThreshold)
			{
				return true;
			}

			return false;
		}

		/**
		 * @brief 候補に入れるターゲットポイントを調べる処理
		 * @param[in,out] candidateTPsOut ターゲットポイントの候補
		 * @param[in] targetPos ターゲットポイント
		 * @param[in,out] posToCandidateTPLenOut 自身から候補のターゲットポイントへの距離
		 * @param[in] posToTargetPosLen 自身からターゲットポイントへの距離
		 * @param[in] posToTargetPosDir 自身からターゲットポイントへの方向
		*/
		void CAICharacterBase::BeCandidateTargetPoint(
			std::unordered_map<nsAICharacterConstData::EnCandidateTargetPointType, Vector3>* candidateTPsOut,
			const Vector3& targetPos,
			float posToCandidateTPLenOut[nsAICharacterConstData::enCandidateTargetPointTypeNum],
			const float posToTargetPosLen,
			const Vector3& posToTargetPosDir
		)
		{
			for (int candidateTPType = 0; candidateTPType < enCandidateTargetPointTypeNum; candidateTPType++)
			{
				// 目標となる移動先への方向
				Vector3 moveTargetDir = Vector3::Front;
				switch (candidateTPType)
				{
				case enTurnRight:
					moveTargetDir = Cross(m_moveDir, Vector3::Up);
					break;
				case enTurnLeft:
					moveTargetDir = Cross(Vector3::Up, m_moveDir);
					break;
				case enGoStraight:
					moveTargetDir = m_moveDir;
					break;
				}

				moveTargetDir.Normalize();

				// 目標となる移動先への方向とターゲット座標への方向を比べる
				const float dotMoveTargetAndPTTDir = Dot(moveTargetDir, posToTargetPosDir);
				// 目標となる移動先への方向とターゲット座標への方向のラジアン角
				const float moveTargetToPTTAngleRad = acosf(dotMoveTargetAndPTTDir);

				// 目標となる移動先への方向から一定以上角度があるターゲットポイントは対象外
				if (moveTargetToPTTAngleRad > kAngleThreshold)
				{
					continue;
				}

				// 始めての候補の場合
				if (candidateTPsOut->count(enTurnRight) == 0)
				{
					// 候補にする
					candidateTPsOut->insert({ enTurnRight, targetPos });
					// 距離を保持しておく
					*posToCandidateTPLenOut = posToTargetPosLen;
					
					// 決まったから処理を終了する
					return;
				}

				// 既に候補がある場合

				// 候補のターゲットポイントを更新するか？
				bool doUpdateCandidateTP = false;
				switch (candidateTPType)
				{
				case enTurnRight:
					if (posToTargetPosLen > *posToCandidateTPLenOut)
					{
						// 右折は遠い方を候補にする
						doUpdateCandidateTP = true;
					}
					break;
				case enTurnLeft:
				case enGoStraight:
					if (posToTargetPosLen <= *posToCandidateTPLenOut)
					{
						// 左折と直進は遠い方を候補にする
						doUpdateCandidateTP = true;
					}
					break;
				}

				if (doUpdateCandidateTP)
				{
					// 右折だから、遠い方を候補にする
					(*candidateTPsOut)[enTurnRight] = targetPos;
					// 距離も更新する
					*posToCandidateTPLenOut = posToTargetPosLen;

					// 決まったから処理を終了する
					return;
				}

			}

			return;
		}

	}
}