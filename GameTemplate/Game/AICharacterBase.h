#pragma once
#include "AICharacterConstData.h"
#include "Path.h"
#include "AIField.h"


namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }
	}
	namespace nsAI {
		class CNaviMesh;
		class CPathFinding;
		struct SAICharacterInitData;
	}


	/**
	 * @brief AIキャラクター関連のネームスペース
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief AIキャラクターの基底クラス
		*/
		class CAICharacterBase : public IGameObject
		{
		public:		// 構造体

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CAICharacterBase() = default;
			/**
			 * @brief デストラクタ
			*/
			~CAICharacterBase() = default;

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

		protected:	// 仮想関数

			/**
			 * @brief 派生クラス用のUpdateの直前で呼ばれる開始処理
			 * @return アップデートを行うか？
			*/
			virtual bool StartSub() { return true; };

			/**
			 * @brief 派生クラス用の消去される時に呼ばれる処理
			*/
			virtual void OnDestroySub() {};

			/**
			 * @brief 派生クラス用の更新処理
			*/
			virtual void UpdateSub() {};

		public:		// メンバ関数

			/**
			 * @brief 初期化処理
			 * @param[in] naviMeshRef ナビメッシュ
			 * @param[in,,out] pathFindingRef パス検索処理
			*/
			void Init(nsAI::SAICharacterInitData* AICharaInitData);

			/**
			 * @brief 移動方向に回転する
			 * @param[in] frontQRotOut 前方向の回転の格納先
			 * @param[in] posBefoerMove 移動前の座標
			 * @param[in] posAfterMove 移動後の座標
			*/
			void Rotating(Quaternion* frontQRotOut, const Vector3& posBeforeMove, const Vector3& posAfterMove);

		protected:	// protectedなメンバ関数

			/**
			 * @brief 
			 * @param filePath 
			 * @param pos 
			 * @param rot 
			 * @param scale 
			*/
			void InitModel(
				const char* filePath,
				const Vector3& pos,
				const Quaternion& rot,
				const Vector3& scale
			);

			/**
			 * @brief パス検索処理
			 * @param[in] targetPos パスの目標座標
			*/
			void PathFinding(const Vector3& targetPos);

			/**
			 * @brief ターゲットの中からランダムでパス検索処理
			*/
			void RandomTargetPathFinding();

			/**
			 * @brief 安全運転を心掛けたパス検索処理
			*/
			void TargetPathFindingToSafeDrive();

			/**
			 * @brief パス上を移動する
			 * @param[in] moveSpeed 移動速度
			 * @param[in] physicsWorld 物理ワールド。
			*/
			void MoveOnPath(
				const float moveSpeed,
				const PhysicsWorld* physicsWorld = nullptr
			);

			/**
			 * @brief パス移動が終了したか？を得る
			 * @return パス移動が終了したか？
			*/
			bool IsEndPathMove() const
			{
				return m_isEndPathMove;
			}

		private:	// privateなメンバ関数

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


		private:	// データメンバ
			nsAI::CPath m_path;		//!< パス
			const nsAI::CNaviMesh* m_naviMeshRef = nullptr;		//!< ナビメッシュの参照
			nsAI::CPathFinding* m_pathFindingRef = nullptr;		//!< パス検索処理の参照
			std::vector<Vector3>* m_naviMeshTargetPointsRef = nullptr;	//!< ナビメッシュのターゲットの参照
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< モデルレンダラー
			bool m_isEndPathMove = true;						//!< パス移動が終了したか？
			Vector3 m_moveDir = Vector3::Front;					//!< 移動方向

			std::unique_ptr<std::mt19937> m_mt;							//!< メルセンヌツイスターの32ビット版
			std::unique_ptr<std::uniform_int_distribution<>> m_rand;	//!< 範囲付きの一様乱数
			//!< 安全運転時に使用する範囲付きの一様乱数
			std::unique_ptr<std::uniform_int_distribution<>> m_randForSafeDrive;
		};
	}
}
