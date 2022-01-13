#pragma once
#include "GaussianBlur.h"
#include "Noncopyable.h"

namespace nsNinjaAttract
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief シャドウ関連のネームスペース
		*/
		namespace nsShadow
		{
			/**
			 * @brief プレイヤー専用のシャドウマップ
			*/
			class CPlayerShadowMap : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CPlayerShadowMap();
				/**
				 * @brief デストラクタ
				*/
				~CPlayerShadowMap() = default;

			public:		// メンバ関数

				/**
				 * @brief 描画関数
				 * @param[in] rc レンダーコンテキスト
				 * @param[in] ligNo ライトの番号
				 * @param[in] lightDirection ライトの方向
				*/
				void Render(RenderContext& rc,const int ligNo, const Vector3& lightDirection);

				/**
				 * @brief プレイヤーレンダラーを設定
				 * @param[in] playerRender プレイヤーレンダラー
				*/
				void SetPlayerRender(CRender* playerRender)
				{
					m_playerRender = playerRender;
				}

				/**
				 * @brief シャドウマップを取得する
				 * @return シャドウマップのテクスチャの参照
				*/
				Texture& GetShadowMap()
				{
					return m_blur.GetBokeTexture();
				}

				/**
				 * @brief ライトビュープロジェクション行列を取得
				 * @return ライトビュープロジェクション行列
				*/
				const Matrix& GetLVPMatrix() const
				{
					return m_lvpMatrix;
				}

			private:	// privateなメンバ関数

				/**
				 * @brief 初期化処理
				*/
				void Init();

				/**
				 * @brief ライトビュープロジェクション行列を計算する
				 * @param[out] lvpMatrixOut ライトビュープロジェクションの出力
				 * @param[in] lightDirection ライトの方向
				*/
				void CalcLightViewProjectionMatrix(Matrix* lvpMatrixOut, const Vector3& lightDirection);

			private:	// データメンバ

				RenderTarget m_shadowMap;	//!< プレイヤーの影を落とすシャドウマップ
				CGaussianBlur m_blur;		//!< シャドウマップにかけるガウシアンブラークラス
				CRender* m_playerRender = nullptr;	//!< プレイヤーのレンダラー
				Matrix m_lvpMatrix = Matrix::Identity;	//!< ライトビュープロジェクション行列

			};

		}
	}
}