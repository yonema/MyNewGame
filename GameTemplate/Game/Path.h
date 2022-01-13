#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief AI関連のネームスペース
	*/
	namespace nsAI
	{
		/**
		 * @brief パスを扱うクラス
		*/
		class CPath
		{
			// 追加。
		public:
			/**
			 * @brief セクションのタグ
			*/
			enum EnSectionTag
			{
				enNone,		//!< 何もなし
				enStraight,	//!< 直進
				enLeftCurveStart,
				enRightCurveStart,
				enLeftCurve,	//!< 左カーブ
				enRightCurve,	//!< 右カーブ
				enLeftCurveEnd,
				enRightCurveEnd,
			};
		private:	// privateな構造体
			/**
			 * @brief パスのセクション
			*/
			struct SSection {
				Vector3 startPos;	// セクションの開始座標。
				Vector3 endPos;		// セクションの終了座標。
				Vector3 direction;	// セクションの方向。
				float length;		// セクションの長さ。
				EnSectionTag sectionTag = enNone;	// セクションのタグ
			};
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPath() = default;
			/**
			 * @brief デストラクタ
			*/
			~CPath() = default;

		public:		// メンバ関数

			/**
			 * @brief パス上を移動する
			 * @details 物理ワールドが指定されていると、パス移動後に地面にレイキャストを行い、
			 * 座標を地面にスナップします。
			 * @param[in] pos 移動させる座標
			 * @param[in] moveSpeed 移動速度
			 * @param[out] isEnd パス移動終了したらtrueが設定される
			 * @param[in] physicsWorld 物理ワールド。
			 * @return 移動後の座標
			*/
			Vector3 Move(
				Vector3 pos,
				const float moveSpeed,
				bool& isEnd,
				const PhysicsWorld* physicsWorld = nullptr
			);

			/**
			 * @brief クリア
			*/
			void Clear()
			{
				m_sectionNo = 0;
				m_pointArray.clear();
				m_sectionArray.clear();
				m_sectionTagArray.clear();
			}

			/**
			 * @brief ポイントの追加。
			 * @param[in] point ポイント
			 * @param[in] sectionType セクションの種類
			*/
			void AddPoint(const Vector3& point, const EnSectionTag sectionTag = enNone)
			{
				m_pointArray.emplace_back(point);
				m_sectionTagArray.emplace_back(sectionTag);
			}
			
			/**
			 * @brief 現在のセクションのタグを取得
			 * @return 現在のセクションの種類
			*/
			EnSectionTag GetCurrentSectionTag() const
			{
				return m_currentSectionTag;
			}

			/**
			 * @brief 現在のセクションを取得
			 * @return 現在のセクション
			*/
			const SSection& GetCurrentSection() const
			{
				return m_sectionArray.at(m_sectionNo);
			}

			/**
			 * @brief パスを構築
			*/
			void Build();

		private:	// データメンバ
			std::vector<Vector3>	m_pointArray;	//!< ポイントの配列
			std::vector<EnSectionTag> m_sectionTagArray;	//!< セクションのタグの配列
			std::vector< SSection >	m_sectionArray;	//!< セクションの配列。
			int m_sectionNo = 0;					//!< セクション番号。
			EnSectionTag m_currentSectionTag = enNone;	//!< 現在のセクションのtag
		};

	}
}