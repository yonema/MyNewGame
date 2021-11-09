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

			void SetAnimState(nsPlayerConstData::nsPlayerModelRenderConstData::EnAnimationClips animState)
			{
				m_animState = animState;
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



		private:	// データメンバ
			const CPlayer* m_playerRef = nullptr;						//!< プレイヤーの参照
			nsGraphic::nsModel::CModelRender* m_playerModel = nullptr;	//!< プレイヤーのモデルレンダラー

			//!< アニメーションクリップスクラス
			AnimationClip m_animationClips[nsPlayerConstData::nsPlayerModelRenderConstData::enAnim_num];
			//!< アニメーションステート
			nsPlayerConstData::nsPlayerModelRenderConstData::EnAnimationClips m_animState =
				nsPlayerConstData::nsPlayerModelRenderConstData::enAnim_idle;
			//!< アニメーション補完時間
			float m_AnimInterpolateTime = 
				nsPlayerConstData::nsPlayerModelRenderConstData::kDefaultAnimInterpolateTime;

			bool m_walkFlag = false;
			bool m_runFlag = false;
			bool m_jumpFlag = false;
			bool m_swingFlag = false;

		};

	}
}
