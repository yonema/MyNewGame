#pragma once
#include "Noncopyable.h"

// 前方宣言
class TkmFile;	// tkmファイルクラス

namespace nsMyGame
{
	/**
	 * @brief ジオメトリ関連のネームスペース
	*/
	namespace nsGeometry
	{
		/**
		 * @brief AABBクラス
		*/
		class CAABB : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CAABB() = default;
			/**
			 * @brief デストラクタ
			*/
			~CAABB() = default;

		public:		// publicな列挙型

			/**
			 * @brief AABBの8頂点の情報
			*/
			enum EnVertexInfo
			{
				enVertPos_X0Y0Z0,	//!< 左下手前の場所の頂点
				enVertPos_X1Y0Z0,	//!< 右下手前の場所の頂点
				enVertPos_X0Y1Z0,	//!< 左上手前の場所の頂点
				enVertPos_X1Y1Z0,	//!< 右上手前の場所の頂点
				enVertPos_X0Y0Z1,	//!< 左下奥の場所の頂点
				enVertPos_X1Y0Z1,	//!< 右下奥の場所の頂点
				enVertPos_X0Y1Z1,	//!< 左上奥の場所の頂点
				enVertPos_X1Y1Z1,	//!< 右上奥の場所の頂点
				enVertNum			//!< 頂点の数
			};

		public:		// メンバ関数

			/**
			 * @brief AABBの情報を初期化
			 * @param[in] vMax 最大値
			 * @param[in] vMin 最小値
			*/
			void Init(const Vector3& vMax, const Vector3& vMin);

			/**
			 * @brief tkmファイルからAABBの情報を初期化
			 * @param[in] tkmFile tkmファイル
			*/
			void InitFromTkmFile(const TkmFile& tkmFile);

			/**
			 * @brief AABBを構成する8頂点のワールド空間での座標を計算する
			 * @param[out] pos_out 8頂点の座標を格納
			 * @param[in] mWorld ワールド行列
			*/
			void CalcVertexPositions(Vector3 pos_out[enVertNum], const Matrix& mWorld);


		private:	// データメンバ
			Vector3 m_max;							//!< AABBを構成するボックスの最大座標
			Vector3 m_min;							//!< AABBを構成するボックスの最小座標
			Vector3 m_vertexPosition[enVertNum];	//!< AABBを構成する8頂点の座標

		};
	}

}

