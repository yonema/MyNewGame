#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"
#include "ModelRender.h"


namespace nsMyGame
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
			 * @brief スイング中のアニメーションステートを更新
			*/
			void UpdateSwingAnimationState();



		private:	// データメンバ
			const CPlayer* m_playerRef = nullptr;						//!< プレイヤーの参照
			nsGraphic::nsModel::CModelRender* m_playerModel = nullptr;	//!< プレイヤーのモデルレンダラー

			//!< アニメーションクリップスクラス
			AnimationClip m_animationClips[nsPlayerConstData::nsModelAnimationConstData::enAnim_num];
			//!< アニメーションステート
			nsPlayerConstData::nsModelAnimationConstData::EnAnimationClips m_animState =
				nsPlayerConstData::nsModelAnimationConstData::enAnim_idle;
			//!< アニメーション補完時間
			float m_AnimInterpolateTime = 
				nsPlayerConstData::nsModelAnimationConstData::kDefaultAnimInterpolateTime;

			bool m_walkFlag = false;
			bool m_runFlag = false;
			bool m_jumpFlag = false;
			bool m_swingFlag = false;

			//!< スイング中のアニメーションステート
			nsPlayerConstData::nsModelAnimationConstData::EnSwingAnimState m_swingAnimSate =
				nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_swing;
		};

	}
}
