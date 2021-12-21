#pragma once
#include "AICharacterBase.h"
#include "AIField.h"
#include "Path.h"


namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }
	}

	/**
	 * @brief AIキャラクター関連のネームスペース
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief AIの車クラス
		*/
		class CAICar : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CAICar();
			/**
			 * @brief デストラクタ
			*/
			~CAICar() = default;

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief Updateの直前で呼ばれる開始処理
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 消去される時に呼ばれる処理
			*/
			void OnDestroy() override final;

			/**
			 * @brief 更新処理
			*/
			void Update() override final;

		public:		// メンバ関数

			/**
			 * @brief 初期化処理
			 * @param[in] AICharaInitData AIキャラクターの初期化データ
			 * @param[in] pos 座標
			 * @param[in] rot 回転
			 * @param[in] scale 拡大
			*/
			void Init(
				const nsAI::SAICharacterInitData& AICharaInitData,
				const Vector3& pos, 
				const Quaternion rot, 
				const Vector3& scale
			);

		private:	// privateなデータメンバ

			/**
			 * @brief 乱数の初期化
			*/
			void InitRand();

			/**
			 * @brief 次のウェイポイントを探す
			*/
			void FindNextWayPoint();

			/**
			 * @brief ウェイポイント上を移動する
			*/
			void MoveOnWayPoint();

			/**
			 * @brief 距離によるターゲットポイントの除外
			 * @param[in] distance 自身とターゲットポイントの距離
			 * @return 除外したか？true戻ってきたら除外した
			*/
			bool ExcludeForDistance(const float distance);

			/**
			 * @brief 角度によるターゲットポイントの除外
			 * @param[out] moveToPTTAngleRad  移動方向とターゲット座標への方向のラジアン角
			 * @param[in] posToTargetPosDir 自身からターゲットへのベクトル
			 * @return 除外したか？trueが戻ってきたら除外した
			*/
			bool ExcludeForAngle(float* moveToPTTAngleRadOut, const Vector3& posToTargetPosDir);

			/**
			 * @brief 候補に入れるターゲットポイントを調べる処理
			 * @param[in,out] candidateTPsOut ターゲットポイントの候補
			 * @param[in] targetPos ターゲットポイント
			 * @param[in,out] posToCandidateTPLenOut 自身から候補のターゲットポイントへの距離
			 * @param[in] posToTargetPosLen 自身からターゲットポイントへの距離
			 * @param[in] posToTargetPosDir 自身からターゲットポイントへの方向
			*/
			void BeCandidateTargetPoint(
				std::unordered_map<nsAICharacterConstData::EnCandidateTargetPointType, Vector3>* candidateTPsOut,
				const Vector3& targetPos,
				float posToCandidateTPLenOut[nsAICharacterConstData::enCandidateTargetPointTypeNum],
				const float posToTargetPosLen,
				const Vector3& posToTargetPosDir
			);

			/**
			 * @brief パスを構築する
			 * @param candidateTPType ターゲットポイントへの候補の種類
			 * @param targetPoint ターゲットポイント
			*/
			void PathBuilding(
				const nsAICharacterConstData::EnCandidateTargetPointType candidateTPType,
				const Vector3& targetPoint
			);


		private:	// データメンバ
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< モデルレンダラー
			Vector3 m_moveDir = Vector3::Front;							//!< 移動方向
			const std::vector<Vector3>* m_wayPointsRef = nullptr;		//!< ウェイポイントの参照
			nsAI::CPath m_path;											//!< パス
			bool m_isMoveEnd = true;									//!< 移動が終了したか？

			std::unique_ptr<std::uniform_int_distribution<>> m_rand;	//!< 範囲付きの一様乱数
			std::unique_ptr<std::mt19937> m_mt;							//!< メルセンヌツイスターの32ビット版

			
		};

	}
}