#pragma once

namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic { 
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
		namespace nsFont { class CFontRender; }		// フォントレンダラークラス
		namespace nsSprite { class CSpriteRender; }	// スプライトレンダラークラス
	}
	namespace nsSound { class CSoundSource; }	//サウンドソース


	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
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
			nsSound::CSoundSource* m_soundSource = nullptr;
		};

	}
}