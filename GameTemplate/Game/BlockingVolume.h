#pragma once
#include "Noncopyable.h"

namespace nsNinjaAttract
{
	/**
	 * @brief ジオメトリ関連のネームスペース
	*/
	namespace nsGeometry
	{

		/**
		 * @brief ブロッキングボリュームクラス
		*/
		class CBlockingVolume : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ

			/**
			 * @brief コンストラクタ
			*/
			CBlockingVolume() = default;

			/**
			 * @brief デストラクタ
			*/
			~CBlockingVolume() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in] pos 座標
			 * @param[in] rot 回転
			 * @param[in] scale 拡大
			*/
			void Init(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

		private:	// staticなデータメンバ

			//!< ブロッキングボリュームのファイルパス
			static const char* const m_kBlockingVolumeModelFilePath;

		private:	// データメンバ
			std::unique_ptr<PhysicsStaticObject> m_physicsStaticObject;
		};

	}
}

