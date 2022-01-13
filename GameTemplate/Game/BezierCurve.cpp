#include "stdafx.h"
#include "BezierCurve.h"

namespace nsNinjaAttract
{
	/**
	 * @brief 曲線関連のネームスペース
	*/
	namespace nsCurve
	{

		/**
		 * @brief 制御点を追加する
		 * @param[in] controlPoint 制御点
		*/
		void CBezierCurve::AddControlPoint(const Vector3& controlPoint)
		{
			m_controlPoints.emplace_back(controlPoint);

			return;
		}

		/**
		 * @brief 制御点をクリアする
		*/
		void CBezierCurve::ClearControlPoints()
		{
			m_controlPoints.clear();

			return;
		}

		/**
		 * @brief ベジェ曲線を計算する
		 * @param[in] t パラメータ（0.0f～1.0f）
		 * @param[out] pointOut 座標出力用
		*/
		void CBezierCurve::CalcBezierCurve(const float t, Vector3* pointOut)
		{
			//////// メモ ////////

			// 「ベジェ曲線の式」
			// 2点の場合
			// P = (1-t)*P1 + t*P2
			// 3点の場合
			// P = (1−t)^2*P1 + 2*(1−t)*t*P2 + t^2*P3
			// 4点の場合
			// P = (1−t)^3*P1 + 3*(1−t)^2*t*P2 + 3*(1−t)*t^2*P3 + t^3*P4

			// 4点の場合の項の遷移
			// P1 -> P2 -> P3 -> P4
			// (1-t)^3 -> (1-t)^2 -> (1-t)^1 -> (1-t)^0
			// t^0 -> t^1 -> t^2 -> t^3
			// 1 -> 3 -> 3 -> 1

			// 「一般化」
			// （条件）n = 制御点の数, (0 <= i < n)
			// P[i]
			// (1-t)^((n-1)-i)
			// t^i
			// if (i == 0 || (i+1) == n){ 1 } else{ n-1 }

			////////////////

			// 制御点の数
			const int controlPointNum = m_controlPoints.size();

			if (controlPointNum <= 1)
			{
				// 制御点が1つ以下なら何もしない。
				return;
			}

			// 最終座標
			Vector3 p = Vector3::Zero;
			// (1-t)
			const float rT = 1.0f - t;

			for (int i = 0; i < controlPointNum; i++)
			{
				// それぞれの項ごとに計算

				// if (i == 0 || (i+1) == n){ 1 } else{ n-1 }
				int a = controlPointNum - 1;
				if (i == 0 || (i + 1) == controlPointNum)
				{
					// 最初と最後の時
					a = 1;
				}

				// (1-t)^3 -> (1-t)^2 -> (1-t)^1 -> (1-t)^0
				float b = pow(rT, (controlPointNum - 1) - i);
				// t^0 -> t^1 -> t^2 -> t^3
				float c = pow(t, i);

				// P1 -> P2 -> P3 -> P4
				// に掛け合わせる
				p += m_controlPoints[i] * a * b * c;
			}

			*pointOut = p;

			return;
		}

	}
}