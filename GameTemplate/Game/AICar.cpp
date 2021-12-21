#include "stdafx.h"
#include "AICar.h"
#include "AICharacterConstData.h"
#include "ModelRender.h"
#include "Level3D.h"
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
		 * @brief コンストラクタ
		*/
		CAICar::CAICar()
		{
			// 生成した瞬間に呼んでほしい処理

			// モデルレンダラーの生成
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
		}

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CAICar::Start()
		{
			// モデルの初期化
			m_modelRender->Init(kCarModelFilePath);

			// 乱数の初期化
			InitRand();

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CAICar::OnDestroy()
		{
			DeleteGO(m_modelRender);

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CAICar::Update()
		{
			if (m_isMoveEnd)
			{
				// 移動終了

				// 次に進むウェイポイントを探す
				FindNextWayPoint();
			}
			else
			{
				// 移動中

				// ウェイポイント上を移動する
				MoveOnWayPoint();
			}

			return;
		}

		/**
		 * @brief 初期化処理
		 * @param[in] AICharaInitData AIキャラクターの初期化データ
		 * @param[in] pos 座標
		 * @param[in] rot 回転
		 * @param[in] scale 拡大
		*/
		void CAICar::Init(
			const nsAI::SAICharacterInitData& AICharaInitData,
			const Vector3& pos,
			const Quaternion rot,
			const Vector3& scale
		)
		{
			// ウェイポイントの参照を得る
			m_wayPointsRef = AICharaInitData.naviMeshTargetPointsRef;

			m_modelRender->SetPosition(pos);
			m_modelRender->SetRotation(rot);
			m_modelRender->SetScale(scale);

			return;
		}

		/**
		 * @brief 乱数の初期化
		*/
		void CAICar::InitRand()
		{
			// 非決定的な乱数生成器でシード生成機を生成
			std::random_device rnd;
			// メルセンヌツイスターの32ビット版、引数は初期シード
			m_mt = std::make_unique<std::mt19937>(rnd());
			// 範囲の一様乱数
			m_rand = std::make_unique<std::uniform_int_distribution<>>(0, enCandidateTargetPointTypeNum - 1);

			return;
		}

		/**
		 * @brief 次のウェイポイントを探す
		*/
		void CAICar::FindNextWayPoint()
		{
			// ターゲットポイントの候補
			std::unordered_map<EnCandidateTargetPointType, Vector3> candidateTargetPoints;
			// 自身からターゲットポイントの候補への距離
			float posToCandidateTPLen[enCandidateTargetPointTypeNum] = {};
			for (int candidateTPType = 0; candidateTPType < enCandidateTargetPointTypeNum; candidateTPType++)
			{
				switch (candidateTPType)
				{
				case enTurnRight:
					posToCandidateTPLen[candidateTPType] = FLT_MIN;
					break;
				case enTurnLeft:
				case enGoStraight:
					posToCandidateTPLen[candidateTPType] = FLT_MAX;
					break;
				}
			}

			// ターゲットポイントの中から次の移動対象のターゲットポイントを探し出す。
			for (const auto& targetPos : *m_wayPointsRef)
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
					posToCandidateTPLen,
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
				int randTargetNum = (*m_rand)(*m_mt);
				randCandidateTPType = static_cast<EnCandidateTargetPointType>(randTargetNum);

				// あったらターゲットポイントを探せた
				if (candidateTargetPoints.count(randCandidateTPType))
				{
					IsFindedTargetPoint = true;
				}
			}

			// 移動終了フラグをリセットする
			m_isMoveEnd = false;

			// パスを構築
			PathBuilding(randCandidateTPType, candidateTargetPoints[randCandidateTPType]);

			return;
		}

		/**
		 * @brief ウェイポイント上を移動する
		*/
		void CAICar::MoveOnWayPoint()
		{
			const Vector3 posBeforeMove = m_modelRender->GetPosition();

			const Vector3 posAfterMove = m_path.Move(
				posBeforeMove,
				2000.0f * nsTimer::CGameTime().GetFrameDeltaTime(),
				m_isMoveEnd,
				PhysicsWorld::GetInstance()
			);

			const Vector3 moveVec = posAfterMove - posBeforeMove;
			m_moveDir = moveVec;
			m_moveDir.Normalize();

			m_modelRender->SetPosition(posAfterMove);
			Quaternion qRot;
			qRot.SetRotation(Vector3::Front, m_moveDir);
			m_modelRender->SetRotation(qRot);

		}

		/**
		 * @brief 距離によるターゲットポイントの除外
		 * @param distance 自身とターゲットポイントの距離
		 * @return 除外したか？true戻ってきたら除外した
		*/
		bool CAICar::ExcludeForDistance(const float distance)
		{
			// 距離が一定以上のターゲットを除外
			if (distance > kBetweenTargetPoint)
			{
				return true;
			}

			// 距離が一定以下
			if (distance < kMinBetweenTargetPoint)
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
		bool CAICar::ExcludeForAngle(float* moveToPTTAngleRadOut, const Vector3& posToTargetPosDir)
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
		void CAICar::BeCandidateTargetPoint(
			std::unordered_map<nsAICharacterConstData::EnCandidateTargetPointType, Vector3>* candidateTPsOut,
			const Vector3& targetPos,
			float posToCandidateTPLenOut[nsAICharacterConstData::enCandidateTargetPointTypeNum],
			const float posToTargetPosLen,
			const Vector3& posToTargetPosDir
		)
		{
			for (int candidateTPTypeNum = 0; candidateTPTypeNum < enCandidateTargetPointTypeNum; candidateTPTypeNum++)
			{
				// 目標となる移動先への方向
				Vector3 moveTargetDir = Vector3::Front;
				float angleThreshold = kAngleThreshold;
				EnCandidateTargetPointType candidateTPType = static_cast<EnCandidateTargetPointType>(candidateTPTypeNum);

				switch (candidateTPType)
				{
				case enTurnRight:
					// moveTargetDir = Cross(m_moveDir, Vector3::Up);
					moveTargetDir = Cross(Vector3::Up, m_moveDir);
					break;
				case enTurnLeft:
					// moveTargetDir = Cross(Vector3::Up, m_moveDir);
					moveTargetDir = Cross(m_moveDir, Vector3::Up);
					break;
				case enGoStraight:
					moveTargetDir = m_moveDir;
					angleThreshold = kStraightAngleThreshold;
					break;
				}

				moveTargetDir.Normalize();

				// 目標となる移動先への方向とターゲット座標への方向を比べる
				const float dotMoveTargetAndPTTDir = Dot(moveTargetDir, posToTargetPosDir);
				// 目標となる移動先への方向とターゲット座標への方向のラジアン角
				const float moveTargetToPTTAngleRad = acosf(dotMoveTargetAndPTTDir);

				// 目標となる移動先への方向から一定以上角度があるターゲットポイントは対象外
				if (moveTargetToPTTAngleRad > angleThreshold)
				{
					continue;
				}

				// 始めての候補の場合
				if (candidateTPsOut->count(candidateTPType) == 0)
				{
					// 候補にする
					candidateTPsOut->insert({ candidateTPType, targetPos });
					// 距離を保持しておく
					posToCandidateTPLenOut[candidateTPType] = posToTargetPosLen;

					// 決まったから処理を終了する
					return;
				}

				// 既に候補がある場合

				// 候補のターゲットポイントを更新するか？
				bool doUpdateCandidateTP = false;
				switch (candidateTPType)
				{
				case enTurnRight:
					if (posToTargetPosLen > posToCandidateTPLenOut[candidateTPType])
					{
						// 右折は遠い方を候補にする
						doUpdateCandidateTP = true;
					}
					break;
				case enTurnLeft:
				case enGoStraight:
					if (posToTargetPosLen <= posToCandidateTPLenOut[candidateTPType])
					{
						// 左折と直進は近い方を候補にする
						doUpdateCandidateTP = true;
					}
					break;
				}

				if (doUpdateCandidateTP)
				{
					(*candidateTPsOut)[candidateTPType] = targetPos;
					// 距離も更新する
					posToCandidateTPLenOut[candidateTPType] = posToTargetPosLen;

					// 決まったから処理を終了する
					return;
				}

			}

			return;
		}

		/**
		 * @brief パスを構築する
		 * @param candidateTPType ターゲットポイントへの候補の種類
		 * @param targetPoint ターゲットポイント
		*/
		void CAICar::PathBuilding(
			const nsAICharacterConstData::EnCandidateTargetPointType candidateTPType,
			const Vector3& targetPoint
		)
		{
			// パスをクリア
			m_path.Clear();

			// スタート地点を追加
			m_path.AddPoint(m_modelRender->GetPosition());

			if (candidateTPType == enTurnLeft || candidateTPType == enTurnRight)
			{
				// 右折か左折なら中継地点を追加する

				// ターゲットポイントへのベクトル
				const Vector3 toTargetPointVec = targetPoint - m_modelRender->GetPosition();
				// ターゲットポイントへのベクトルが射影された移動方向ベクトル
				const float toTPVecProjMoveDir = Dot(m_moveDir, toTargetPointVec);
				// ターゲットポイントの中継地点
				const Vector3 relayTargetPos = 
					m_modelRender->GetPosition() +m_moveDir * toTPVecProjMoveDir;
				// 中継地点を追加
				m_path.AddPoint(relayTargetPos);
			}

			// ゴール地点を追加
			m_path.AddPoint(targetPoint);

			// パスを構築
			m_path.Build();

			return;
		}

	}
}