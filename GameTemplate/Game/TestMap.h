#pragma once

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic { 
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
		namespace nsFont { class CFontRender; }		// フォントレンダラークラス
		namespace nsSprite { class CSpriteRender; }	// スプライトレンダラークラス
	}
	namespace nsSound { class CSoundCue; }			// サウンドキュークラス
	namespace nsEffect { class CEffectPlayer; }		// エフェクトプレイヤークラス
	namespace nsLight { class CPointLight; }		// ポイントライトクラス
	namespace nsNature { class CSkyCube; }			// スカイキューブクラス

	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief テストマップ（レベル）用のネームスペース
		*/
		namespace nsTestMaps
		{
			/**
			 * @brief テスト用のマップ
			*/
			class CTestMap : public IGameObject
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CTestMap() = default;

				/**
				 * @brief デストラクタ
				*/
				~CTestMap() = default;

			public:		// オーバーライドしたメンバ関数
				/**
				 * @brief スタート関数
				 * @return アップデートを行うか？
				*/
				bool Start() override final;

				/**
				 * @brief 破棄した時に呼ばれる
				*/
				void OnDestroy() override final;

				/**
				 * @brief アップデート関数
				*/
				void Update() override final;

			private:	// 列挙型
				enum EnAnimationClips
				{
					enAnim_idle,
					enAnim_walk,
					enAnim_num
				};

			private:	// データメンバ
				nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< モデルレンダラー
				AnimationClip m_animationClip[enAnim_num];
				EnAnimationClips m_animState = enAnim_idle;
				nsGraphic::nsFont::CFontRender* m_fontRender = nullptr;		//!< フォントレンダラー
				nsGraphic::nsSprite::CSpriteRender* m_spriteRender = nullptr;	//!< スプライトレンダラー
				nsSound::CSoundCue* m_soundCue = nullptr;					//!< サウンドキュー
				nsEffect::CEffectPlayer* m_effectPlayer = nullptr;			//!< エフェクトプレイヤー
				nsNature::CSkyCube* m_skyCube = nullptr;					//!< スカイキューブ

				nsGraphic::nsModel::CModelRender* m_lightModel = nullptr;
				nsLight::CPointLight* m_pointLight = nullptr;
				const char* m_pointLigName = "PointLight";
				const char* m_pointLigModelName = "PointLightModel";
				const Vector4 m_pointLigColor = { 100.0f,0.0f,0.0f,1.0f };
			};
		}
	}
}