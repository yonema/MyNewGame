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
		/**
		 * @brief プレイヤーモデルクラス
		*/
		class CPlayerModel : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayerModel() = default;
			/**
			* @brief デストラクタ
			*/
			~CPlayerModel();

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in] startPos スタート座標
			 * @param[in] startRot スタート回転
			*/
			void Init(const Vector3& startPos, const Quaternion& startRot);

			/**
			 * @brief プレイヤーモデルクラスのアップデートを実行する
			*/
			void ExecuteUpdate();


		private:	// privateなメンバ関数

			/**
			 * @brief アニメーション初期化
			*/
			void InitAnimation();

			/**
			 * @brief モデル初期化
			 * @param[in] startPos スタート座標
			 * @param[in] startRot スタート回転
			*/
			void InitModel(const Vector3& startPos, const Quaternion& startRot);

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
			nsGraphic::nsModel::CModelRender* m_playerModel = nullptr;	//!< プレイヤーのモデルレンダラー
			//!< アニメーションクリップスクラス
			AnimationClip m_animationClips[	nsPlayerModelRenderConstData::enAnim_num];

			//!< アニメーションステート
			nsPlayerModelRenderConstData::EnAnimationClips m_animState =
				nsPlayerModelRenderConstData::enAnim_idle;

			//!< アニメーション補完時間
			float m_AnimInterpolateTime = nsPlayerModelRenderConstData::kDefaultAnimInterpolateTime;

			const Vector3* m_playerPosition = nullptr;		//!< プレイヤーの座標の参照
			const Quaternion* m_playerRotation = nullptr;	//!< プレイヤーの回転の参照

		};

	}
}
