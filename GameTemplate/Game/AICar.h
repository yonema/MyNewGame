#pragma once
#include "AICharacterBase.h"
#include "AICharacterConstData.h"
#include "AIField.h"
#include "Path.h"


namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }
	}
	namespace nsSound { class CSoundCue; }


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

			/**
			 * @brief 座標を取得
			 * @return 座標
			*/
			const Vector3& GetPosition() const
			{
				return m_modelRender->GetPosition();
			}

			/**
			 * @brief 回転を取得
			 * @return 回転
			*/
			const Quaternion& GetRotation() const
			{
				return m_modelRender->GetRotation();
			}

			/**
			 * @brief 捕まえた状態にする
			*/
			void BeCaptured()
			{
				m_isCaptured = true;
			}

			/**
			 * @brief 捕まっているか？を得る
			 * @return 捕まっているか？
			*/
			bool IsCaputred() const
			{
				return m_isCaptured;
			}

			/**
			 * @brief 忍術のエフェクトの参照を設定する
			 * @param[in.out] effectRef 忍術のエフェクトの参照
			*/
			void SetNinjyutuEffectRef(Effect* effectRef)
			{
				m_ninjyutuEFRef = effectRef;
			}

		private:	// privateなデータメンバ

			/**
			 * @brief 乱数の初期化
			*/
			void InitRand();

			/**
			 * @brief エフェクトの初期化
			*/
			void InitEffect();

			/**
			 * @brief サウンドの初期化
			*/
			void InitSound();

			/**
			 * @brief 捕まっている時の更新
			 * @return この後も更新処理を行うか？
			*/
			bool UpdateOnIsCaptured();

			/**
			 * @brief 煙のエフェクトの更新
			*/
			void UpdateSmokeEffect();

			/**
			 * @brief 次のウェイポイントを探す
			*/
			void FindNextWayPoint();

			/**
			 * @brief ウェイポイント上を移動する
			*/
			void MoveOnWayPoint();

			/**
			 * @brief 回転を更新
			*/
			void UpdateRotating();

			/**
			 * @brief サウンドを更新
			*/
			void UpdateSound();

			/**
			 * @brief サウンドの音量を計算する
			 * @return 計算されたサウンドの音量
			*/
			float CalcSoundVolume();

			/**
			 * @brief このクラスのサウンドの停止
			*/
			void StopSound();

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

			/**
			 * @brief カーブの座標を作成する
			 * @param candidateTPType ターゲットポイントへの候補の種類
			 * @param targetPoint ターゲットポイント
			*/
			void MakeCurve(
				const nsAICharacterConstData::EnCandidateTargetPointType candidateTPType,
				const Vector3& targetPoint
			);


		private:	// データメンバ
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< モデルレンダラー
			Vector3 m_moveDir = Vector3::Front;							//!< 移動方向
			const std::vector<Vector3>* m_wayPointsRef = nullptr;		//!< ウェイポイントの参照
			nsAI::CPath m_path;											//!< パス
			bool m_isMoveEnd = true;									//!< 移動が終了したか？
			Vector3 m_curveCenterPosition = Vector3::Zero;				//!< カーブの中心座標
			Vector3 m_fromCurveCenterToCurveStartVec = Vector3::Zero;	//!< カーブ中心座標からカーブ開始座標へのベクトル
			float m_moveSpeed = 0.0f;
			bool m_isCaptured = false;									//!< 捕まったか？
			Effect* m_explotionEF = nullptr;							//!< 爆発のエフェクト
			Effect* m_smokeEFs[nsAICharacterConstData::kSmokeNum] = {};	//!< 煙のエフェクト
			Effect* m_smokeExplotionEF = nullptr;						//!< 爆発時の煙のエフェクト
			float m_isCaptureTimer = 0.0f;								//!< 捕まっている時のタイマー
			Effect* m_ninjyutuEFRef = nullptr;							//!< 忍術のエフェクトの参照
			std::unique_ptr<std::uniform_int_distribution<>> m_rand;	//!< 範囲付きの一様乱数
			std::unique_ptr<std::mt19937> m_mt;							//!< メルセンヌツイスターの32ビット版

			// サウンド
			nsSound::CSoundCue* m_driveSC = nullptr;		//!< 運転中のサウンド
			nsSound::CSoundCue* m_driftSC = nullptr;		//!< ドリフトのサウンド
			nsSound::CSoundCue* m_fireExplosionSC = nullptr;	//!< 火遁の爆発のサウンド
			nsSound::CSoundCue* m_carExplosionSC = nullptr;		//!< 車の爆発のサウンド
			bool m_driftSoundFlag = false;					//!< ドリフト音を再生できるか？
			float m_driveTimer = 0.0f;						//!< 運転中に使用するタイマー
		};

	}
}