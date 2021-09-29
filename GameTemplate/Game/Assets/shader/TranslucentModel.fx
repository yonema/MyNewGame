/*!
 * @brief	シンプルなモデルシェーダー。
 */

//モデルの頂点シェーダー関係の共通ヘッダー
 #include "ModelVSCommon.h"

// レジスタ1の定数バッファはPBRLighting.hで使用

// ディファードライティング用の定数バッファ
// RenderingEngineConstData.hのSDefferdLightingCBと同じ構造体にする
cbuffer defferdLightingCb : register(b2)
{
	float4x4 mViewProjInv;      //!< ビュープロジェクション行列の逆行列
	int isIBL;					//!< IBLを行うか？1：行う。0：行わない。
	float IBLLuminance;			//!< IBLの明るさ
}

 ///////////////////////////////////////
 // 定数
 ///////////////////////////////////////
 // LightConstData.hと同じ値。変更したら、一緒に変更する。
static const int kMaxDirectionalLightNum = 4;	//!< ディレクションライトの最大数
static const int kMaxPointLightNum = 16;		//!< ポイントライトの最大数
static const int kMaxSpotLightNum = 16;			//!< スポットライトの最大数
static const int kMaxShadowMapNum = 3;	//!< シャドウマップの数


////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////

//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos : SV_POSITION;	// 座標。
	float3 normal : NORMAL;		// 法線。
	float3 tangent : TANGENT;	// 接ベクトル。
	float3 biNormal : BINORMAL;	// 従ベクトル。
	float2 uv : TEXCOORD0;		// UV座標。
	float3 worldPos : TEXCOORD1;	// ワールド座標
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedoMap	: register(t0);		// アルベドマップ
Texture2D<float4> g_normalMap	: register(t1);		// 法線
Texture2D<float4> g_spacularMap : register(t2);		//スペキュラマップ
Texture2D<float4> g_shadowMap[kMaxDirectionalLightNum][kMaxShadowMapNum] : register(t10);  //シャドウマップ。
TextureCube<float4> g_skyCubeMap : register(t22);

// PBRのライティングのヘッダー
#include "PBRLighting.h"

// ポイントライトとスポットライトのライティングの計算に使用する関数ヘッダー
#include "PointAndSpotLightFunc.h"


////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal)
{
	SPSIn psIn;

	psIn.pos = mul(mWorldLocal, vsIn.pos); // モデルの頂点をワールド座標系に変換
	// 頂点シェーダーからワールド座標を出力
	psIn.worldPos = psIn.pos;

	psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
	psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
	psIn.normal = normalize(mul(mWorldLocal, vsIn.normal));
	psIn.tangent = normalize(mul(mWorldLocal, vsIn.tangent));
	psIn.biNormal = normalize(mul(mWorldLocal, vsIn.biNormal));
	psIn.uv = vsIn.uv;

	return psIn;
}
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, mWorld);
}
SPSIn VSSkinMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, CalcSkinMatrix(vsIn));
}

//法線マップから法線を得る
float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

    return newNormal;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	// アルベドカラーをサンプリング
	float4 albedoColor = g_albedoMap.Sample(g_sampler, psIn.uv);
	//法線をサンプリング。
	float3 normal = GetNormal(psIn.normal,psIn.tangent,psIn.biNormal, psIn.uv);
	//ワールド座標をサンプリング。
	float3 worldPos = psIn.worldPos;
	//スペキュラカラーをサンプリング。
	float3 specColor = albedoColor.xyz;
	//金属度をサンプリング。
	float metaric = g_spacularMap.SampleLevel(g_sampler, psIn.uv, 0).r;
	//スムース
	float smooth = g_spacularMap.SampleLevel(g_sampler, psIn.uv, 0).a;
	if (smooth >= 1.0f)
		smooth = 0.0f;

	// 視線に向かって伸びるベクトルを計算する
	float3 toEye = normalize(eyePos - worldPos);

	float3 lig = 0.0f;

	// ディレクションライトのライティングの計算
	for (int ligNo = 0; ligNo < directionalLightNum; ligNo++)
	{
		// 影の落ち具合を計算する。
		float shadow = 0.0f;
		if (directionalLightData[ligNo].castShadow == 1) {
			//影を生成するなら。
			shadow = CalcShadowRate(ligNo, worldPos) * 1.0f /*shadowParam*/;
		}
		// PBRのライティングを計算
		lig += CalcLighting(
			directionalLightData[ligNo].direction,
			directionalLightData[ligNo].color,
			normal,
			toEye,
			albedoColor,
			metaric,
			smooth,
			specColor
		) * (1.0f - shadow);
	}

	// ポイントライトのライティングの計算
	for (int ligNo = 0; ligNo < pointLightNum; ligNo++)
	{
		// 距離による影響率を計算する
		float affect = 
			GetAffectOfDistance(worldPos, pointLightData[ligNo].position, pointLightData[ligNo].range);

		// 影響率が0以下だったら計算する必要なし
		if (affect <= 0.0f)
			continue;

		// ライトの入射方向
		float3 ligDir = worldPos - pointLightData[ligNo].position;
		ligDir = normalize(ligDir);

		// 距離による影響率を踏まえて、PBRのライティングを計算
		lig += CalcLighting(
			ligDir,
			pointLightData[ligNo].color,
			normal,
			toEye,
			albedoColor,
			metaric,
			smooth,
			specColor
		) * affect;
	}

	// スポットライトのライティングの計算
	for (int ligNo = 0; ligNo < spotLightNum; ligNo++)
	{
		// 距離による影響率を計算する
		float affectOfDistance = GetAffectOfDistance(
			worldPos,
			spotLightData[ligNo].pointLightData.position,
			spotLightData[ligNo].pointLightData.range
		);
		// 影響率が0以下だったら計算する必要なし
		if (affectOfDistance <= 0.0f)
			continue;

		// ライトの入射方向
		float3 ligDir = worldPos - spotLightData[ligNo].pointLightData.position;
		ligDir = normalize(ligDir);

		// 角度による影響率の計算
		float affectOfAngle =
			GetAffectOfAngle(ligDir, spotLightData[ligNo].direction, spotLightData[ligNo].angle);
		// 影響率が0以下だったら計算する必要なし
		if (affectOfAngle <= 0.0f)
			continue;

		// 距離による影響率と角度による影響率を踏まえて、PBRのライティングを計算
		lig += CalcLighting(
			ligDir,
			spotLightData[ligNo].pointLightData.color,
			normal,
			toEye,
			albedoColor,
			metaric,
			smooth,
			specColor
		) * affectOfDistance * affectOfAngle;
	}


	// IBLを行うか？
	if (isIBL == 1)
	{
		// 行う
		// 視線からの反射ベクトルを求める。
		float3 v = reflect(toEye * -1.0f, normal);
		int level = lerp(0, 12, 1 - smooth);
		lig += albedoColor * g_skyCubeMap.SampleLevel(g_sampler, v, level) * IBLLuminance;
	}
	else
	{
		// 行わない
		// 環境光による底上げ
		lig += ambientLight * albedoColor;
	}


	float4 finalColor = 1.0f;
	finalColor.xyz = lig;

	// 自己発光カラーを加える
	finalColor.xyz += emissionColor.xyz;
	// 乗算カラーをかける
	finalColor *= mulColor;

	return finalColor;

}
