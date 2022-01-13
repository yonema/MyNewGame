#pragma once
#include "Noncopyable.h"

namespace nsNinjaAttract
{
	/**
	 * @brief カメラ関連のネームスペース
	*/
	namespace nsCamera
	{
		/**
		 * @brief カメラコリジョン解決クラス
		*/
		class CCameraCollisionSolver : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CCameraCollisionSolver() = default;
			/**
			 * @brief デストラクタ
			*/
			~CCameraCollisionSolver() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param radius カメラのバウンディングスフィアの半径
			*/
			void Init(const float radius);

			/**
			 * @brief コリジョン解決の実行
			 * @param[out] result コリジョン解決を行った視点の座標が格納される
			 * @param[in] position コリジョン解決を行う前の視点の座標
			 * @param[in] target カメラの注視点
			 * @return コリジョン解決を行ったか？
			*/
			bool Execute(Vector3& result_out, const Vector3& position, const Vector3& target);

		private:	// データメンバ
			SphereCollider m_collider;	//!< コライダー
			float m_radius = 0.0f;		//!< 半径
			bool m_isInited = false;	//!< 初期化済みか？

		};

	}
}