#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"
#include "ModelRender.h"


namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic
	{
		namespace nsModel { class CModelRender; }	//モデルレンダラークラス
	}

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 前方宣言
		class CPlayer;	// プレイヤークラス

		/**
		 * @brief プレイヤーモデルアニメーションクラス
		*/
		class CPlayerModelAnimation : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayerModelAnimation() = default;
			/**
			* @brief デストラクタ
			*/
			~CPlayerModelAnimation();

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in] player プレイヤーの参照
			*/
			void Init(const CPlayer& player);

			/**
			 * @brief プレイヤーモデルクラスのアップデートを実行する
			*/
			void ExecuteUpdate();

			/**
			 * @brief スイング中のアニメーションステートを設定
			 * @param[in] state ステート
			*/
			void SetSwingAnimState(const nsPlayerConstData::nsModelAnimationConstData::EnSwingAnimState state)
			{
				m_swingAnimSate = state;
			}

			/**
			 * @brief アニメーションステートを得る
			 * @return アニメーションステート
			*/
			nsPlayerConstData::nsModelAnimationConstData::EnAnimationClips GetAnimationState() const
			{
				return m_animState;
			}

			/**
			 * @brief 手のボーンのワールドマトリクスを得る
			 * @return 手のボーンのワールドマトリクス
			*/
			const Matrix& GetHandBoneMatrix() const
			{
				return m_playerModel->GetBone(m_handBoneID)->GetWorldMatrix();
			}

		private:	// privateなメンバ関数

			/**
			 * @brief アニメーション初期化
			*/
			void InitAnimation();

			/**
			 * @brief モデル初期化
			*/
			void InitModel();

			/**
			 * @brief モデル解放
			*/
			void ReleaseModel();

			/**
			 * @brief トランスフォームを更新
			*/
			void UpdateTransform();

			/**
			 * @brief アニメーション遷移を更新
			*/
			void UpdateAnimationTransition();

			/**
			 * @brief アニメーションステートを更新
			*/
			void UpdateAnimationState();

			/**
			 * @brief クリア演出のアニメーション
			 * @param 早期リターンを行うか？
			*/
			bool UpdateClearDirecting();

			/**
			 * @brief スイング中のアニメーションステートを更新
			 * @param 早期リターンを行うか？
			*/
			bool UpdateSwingAnimationState();

			/**
			 * @brief コマンド入力が失敗した時のアニメーションステートを更新
			 * @return 早期リターンを行うか？
			*/
			bool UpdateCommandFailed();

			/**
			 * @brief 敵の上に乗ってるときのアニメーションステートを更新
			 * @return 早期リターンを行うか？
			*/
			bool UpdateOnEnemyAnimationState();

			/**
			 * @brief 最初の落下のアニメーションの更新
			 * @return 早期リターンを行うか？
			*/
			bool UpdateStartFall();

			/**
			 * @brief ジャンプと空中でのアニメーションの更新
			 * @return 早期リターンを行うか？
			*/
			bool UpdateJumpAndAir();

			/**
			 * @brief アイドル状態と歩きと走りのアニメーションの更新
			 * @return 早期リターンを行うか？
			*/
			bool UpdateIdleWalkAndRun();


		private:	// データメンバ
			const CPlayer* m_playerRef = nullptr;						//!< プレイヤーの参照
			nsGraphic::nsModel::CModelRender* m_playerModel = nullptr;	//!< プレイヤーのモデルレンダラー

			int m_handBoneID = -1;	//!< 手のボーンID

			//!< アニメーションクリップスクラス
			AnimationClip m_animationClips[nsPlayerConstData::nsModelAnimationConstData::enAnim_num];
			//!< アニメーションステート
			nsPlayerConstData::nsModelAnimationConstData::EnAnimationClips m_animState =
				nsPlayerConstData::nsModelAnimationConstData::enAnim_idle;
			//!< アニメーション補完時間
			float m_animInterpolateTime = 
				nsPlayerConstData::nsModelAnimationConstData::kDefaultAnimInterpolateTime;
			//!< アニメーションスピード
			float m_animSpeed = nsPlayerConstData::nsModelAnimationConstData::kDefaultAnimSpeed;

			//!< スイング中のアニメーションステート
			nsPlayerConstData::nsModelAnimationConstData::EnSwingAnimState m_swingAnimSate =
				nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_swing;
		};

	}
}
