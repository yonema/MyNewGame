#pragma once

namespace nsMyGame
{

	/**
	 * @brief 糸を使ったアクションのターゲット関連のネームスペース
	*/
	namespace nsStringActionTarget
	{
		// 前方宣言
		namespace nsSwingTarget 
		{ 
			class CSwingTarget;		// スイングのターゲットクラス
		}

		/**
		 * @brief 一番近いスイングターゲットのポイントの座標を探す処理のデータ
		*/
		struct SFindNearestSwingTargetPointData
		{
			Vector3 findSwingTargetOrigin = Vector3::Zero;			//!< スイングターゲットを探す原点
			float findSwingTargetScopeRadius = 0.0f;				//!< スイングターゲットを探す有効範囲
			Vector3 findSwingTargetPointOrigin = Vector3::Zero;		//!< スイングターゲットのポイントを探す原点
			float findSwingTargetPointScopeRadius = 0.0f;			//!< スイングターゲットのポイントを探す有効範囲
			float heightLowerLimit = 0.0f;							//!< 高さの下限
			Vector3 forwardLimitDir = Vector3::Zero;				//!< 前方向の制限の方向ベクトル
			Vector3 forwardLimitOrigin = Vector3::Zero;				//!< 前方向の制限の原点
		};

		/**
		 * @brief 糸を使ったアクションのターゲット関連のマネージャー
		 * @note シングルトンパターンを使用
		*/
		class CStringActionTargetManager
		{
		private:	// コンストラクタとデストラクタ。
					// シングルトンパターンのためprivateに隠す
			/**
			 * @brief コンストラクタ
			*/
			CStringActionTargetManager();
			/**
			 * @brief デストラクタ
			*/
			~CStringActionTargetManager() = default;

		private:	// staticなデータメンバ
			static CStringActionTargetManager* m_instance;		//!< 唯一のインスタンス		
		
		public:		// staticなメンバ関数

			/**
			 * @brief 唯一のインスタンスを生成する
			 * @note シングルトンパターン
			*/
			static void CreateInstance()
			{
				m_instance = new CStringActionTargetManager;
			}

			/**
			 * @brief 唯一のインスタンスを破棄する
			 * @note シングルトンパターン
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief 唯一のインスタンスの参照を得る
			 * @return 唯一のインスタンスの参照
			 * @note シングルトンパターン
			*/
			static CStringActionTargetManager* GetInstance()
			{
				return m_instance;
			}

		public:		// メンバ関数

			/**
			 * @brief スイングターゲットを登録する
			 * @param[in,out] swingTarget 登録するスイングターゲット
			*/
			void AddSwingTarget(nsSwingTarget::CSwingTarget* swingTarget)
			{
#ifdef MY_DEBUG
				if (m_swingTargets.size() >= m_kSwingTargetNum)
					MessageBoxA(nullptr, "スイングのターゲットが予想より多く作られている", "警告", MB_OK);
#endif
				m_swingTargets.emplace_back(swingTarget);
			}

			/**
			 * @brief スイングターゲットの登録を解除する
			 * @param[in] swingTarget 解除するスイングターゲット
			*/
			void RemoveSwingTarget(const nsSwingTarget::CSwingTarget* swingTarget);

			/**
			 * @brief 全てのスイングターゲットにクエリを実行
			 * @param[in] func 実行する関数
			*/
			void QuerySwingTarget(const std::function<void(nsSwingTarget::CSwingTarget* swingTarget)>func)
			{
				for (const auto& st : m_swingTargets)
				{
					func(st);
				}

				return;
			}



		private:	// 定数

			static const int m_kSwingTargetNum = 256;	// スイングのターゲット

		private:	// データメンバ
			std::vector<nsSwingTarget::CSwingTarget*> m_swingTargets;	// スイングのターゲットのコンテナ

		};

		/**
		 * @brief 指定された座標から有効範囲内にあるスイングターゲットにクエリを実行
		 * @param[in] fromPos 指定座標
		 * @param[in] scopeRadius 有効範囲
		 * @param[in] func 実行する関数
		*/
		void QuerySwingTargetWithinScope(
			const Vector3& fromPos,
			const float scopeRadius,
			const std::function<void(nsSwingTarget::CSwingTarget* swingTarget)>func
		);

		/**
		 * @brief 指定された座標から有効範囲内にあるスイングターゲットの座標の中で一番近い座標を探してくる
		 * @param[in] data 一番近いスイングターゲットのポイントの座標を探す処理のデータ
		 * @return スイングターゲットの座標。有効範囲内に無ければnullptrを戻す。
		*/
		const Vector3* const FindNearestSwingTargetPoint(
			const SFindNearestSwingTargetPointData& data
		);
	}
}
