/*!
 * @brief	ポイントライトとスポットライトのライティングの計算に使用する関数ヘッダー。
 */

/**
 * @brief 距離による影響率を計算する
 * @param worldPos 描画先のワールド座標
 * @param ligPos ライトのワールド座標
 * @param ligRange ライトの影響範囲
 * @return 影響率
*/
float GetAffectOfDistance(float3 worldPos, float3 ligPos, float ligRange)
{
	// 距離による影響率を計算する

	// ポイントライトから描画先の座標への距離
	float3 distance = length(worldPos - ligPos);
	// 距離とポイントライトの影響範囲から影響率を計算する
	float affect = 1.0f - 1.0f / ligRange * distance;

	// 影響率が0以下だったら計算する必要なし
	if (affect <= 0.0f)
		return 0.0f;

	// 影響率を、線形な変化から指数関数的な変化へ
	affect = pow(affect, 3.0f);

	return affect;
}

/**
 * @brief 角度による影響率を計算する
 * @param ligDir ライトの入射方向
 * @param ligEmmisionDir ライトの射出方向
 * @param ligAngle ライトの射出角度
 * @return 影響率
*/
float GetAffectOfAngle(float3 ligDir, float3 ligEmmisionDir, float ligAngle)
{
	// ライトの入社方向と射出方向の角度を計算する。

	// ライトの入射方向と射出方向の内積を求める
	float angle = dot(ligDir, ligEmmisionDir);
	// 内積から角度を求める
	angle = abs(acos(angle));

	// 角度による影響率を求める
	// 角度に比例して小さくなっていく影響率を計算する
	float affect = 1.0f - 1.0f / ligAngle * angle;

	// 影響率が0以下だったら計算する必要なし
	if (affect <= 0.0f)
		return 0.0f;

	// 影響率を、線形な変化から指数関数的な変化へ
	affect = pow(affect, 3.0f);

	return affect;
}