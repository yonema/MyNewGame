#pragma once

namespace nsMyGame
{
	/**
	 * @brief 曲線関連のネームスペース
	*/
	namespace nsCurve
	{
		/**
		 * @brief ベジェ曲線クラス
		*/
		class CBezierCurve
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CBezierCurve() = default;
			/**
			 * @brief デストラクタ
			*/
			~CBezierCurve() = default;

		public:		// メンバ関数

			/**
			 * @brief 制御点を追加する
			 * @param[in] controlPoint 制御点
			*/
			void AddControlPoint(const Vector3& controlPoint);

			/**
			 * @brief 制御点をクリアする
			*/
			void ClearControlPoints();

			/**
			 * @brief ベジェ曲線を計算する
			 * @param[in] t パラメータ（0.0f～1.0f）
			 * @param[out] pointOut 座標出力用
			*/
			void CalcBezierCurve(const float t, Vector3* pointOut);

		private:	// データメンバ
			std::vector<Vector3> m_controlPoints;
		};

	}
}