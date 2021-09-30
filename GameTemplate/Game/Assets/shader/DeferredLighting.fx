///////////////////////////////////////
// PBRベースのディファードライティング
///////////////////////////////////////


///////////////////////////////////////
// 定数
///////////////////////////////////////
// LightConstData.hと同じ値。変更したら、一緒に変更する。
static const int kMaxDirectionalLightNum = 4;	//!< ディレクションライトの最大数
static const int kMaxPointLightNum = 16;		//!< ポイントライトの最大数
static const int kMaxSpotLightNum = 16;			//!< スポットライトの最大数
static const int kMaxShadowMapNum = 3;	//!< シャドウマップの数


///////////////////////////////////////
// 構造体。
///////////////////////////////////////

//頂点シェーダーへの入力構造体。
struct SVSIn
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};
//ピクセルシェーダーへの入力構造体。
struct SPSIn
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

///////////////////////////////////////
// 定数バッファ。
///////////////////////////////////////
//共通定数バッファ
cbuffer cb : register(b0)
{
    float4x4 mvp; 
    float4 mulColor;
    float4 screenParam;
};

// レジスタ1の定数バッファはPBRLighting.hで使用

// ディファードライティング用の定数バッファ
// RenderingEngineConstData.hのSDefferdLightingCBと同じ構造体にする
cbuffer defferdLightingCb : register(b2)
{
	float4x4 mViewProjInv;      //!< ビュープロジェクション行列の逆行列
}

// IBL用の定数バッファ
// RenderingEngineConstData.hのSIBLCBと同じ構造体にする
cbuffer IBLCb : register (b3)
{
	int isIBL;					//!< IBLを行うか？1：行う。0：行わない。
	float IBLLuminance;			//!< IBLの明るさ
}

///////////////////////////////////////
// テクスチャ
///////////////////////////////////////
Texture2D<float4> albedoTexture : register(t0);     // アルベド
Texture2D<float4> normalTexture : register(t1);     // 法線
Texture2D<float4> metallicShadowSmoothTexture : register(t2);   // メタリック、シャドウ、スムーステクスチャ。rに金属度、gに影パラメータ、aに滑らかさ。
Texture2D<float4> g_shadowMap[kMaxDirectionalLightNum][kMaxShadowMapNum] : register(t3);  //シャドウマップ。
TextureCube<float4> g_skyCubeMap : register(t15);
// タイルごとのポイントライトのインデックスのリスト
//StructuredBuffer<uint> pointLightListInTile : register(t20);

#include "PBRLighting.h"
#include "PointAndSpotLightFunc.h"

///////////////////////////////////////
// 関数
///////////////////////////////////////

//頂点シェーダー。
SPSIn VSMain(SVSIn In)
{
	SPSIn psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}


/*!
 * @brief	UV座標と深度値からワールド座標を計算する。
 *@param[in]	uv				uv座標
 *@param[in]	zInScreen		スクリーン座標系の深度値
 *@param[in]	mViewProjInv	ビュープロジェクション行列の逆行列。
 */
float3 CalcWorldPosFromUVZ( float2 uv, float zInScreen, float4x4 mViewProjInv )
{
	float3 screenPos;
	screenPos.xy = (uv * float2(2.0f, -2.0f)) + float2( -1.0f, 1.0f);
	screenPos.z = zInScreen;//depthMap.Sample(Sampler, uv).r;
	
	float4 worldPos = mul(mViewProjInv, float4(screenPos, 1.0f));
	worldPos.xyz /= worldPos.w;
	return worldPos.xyz;
}


/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    //G-Bufferの内容を使ってライティング
    //アルベドカラーをサンプリング。
    float4 albedoColor = albedoTexture.Sample(g_sampler, psIn.uv);
    //法線をサンプリング。
    float3 normal = normalTexture.Sample(g_sampler, psIn.uv).xyz;
    //ワールド座標をサンプリング。
    float3 worldPos = CalcWorldPosFromUVZ(psIn.uv, albedoColor.w, mViewProjInv);
    //スペキュラカラーをサンプリング。
    float3 specColor = albedoColor.xyz;
    //金属度をサンプリング。
    float metaric = metallicShadowSmoothTexture.SampleLevel(g_sampler, psIn.uv, 0).r;
    //スムース
    float smooth = metallicShadowSmoothTexture.SampleLevel(g_sampler, psIn.uv, 0).a;
    if (smooth >= 1.0f)
        smooth = 0.0f;

    //影生成用のパラメータ。
    float shadowParam = metallicShadowSmoothTexture.Sample(g_sampler, psIn.uv).g;

    // 視線に向かって伸びるベクトルを計算する
    float3 toEye = normalize(eyePos - worldPos);

    float3 lig = 0;
    
    // ディレクションライトのライティングの計算
    for (int ligNo = 0; ligNo < directionalLightNum; ligNo++)
    {
		// 影の落ち具合を計算する。
		float shadow = 0.0f;
		if (directionalLightData[ligNo].castShadow == 1) {
			//影を生成するなら。
			shadow = CalcShadowRate(ligNo, worldPos) * shadowParam;
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
	return finalColor;
}
