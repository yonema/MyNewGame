#pragma once
#include "NatureConstData.h"
#include "ModelRender.h"


namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic { namespace nsModel { class CModelRender; } }	// モデルレンダラークラス

	/**
	 * @brief 自然関連のネームスペース
	*/
	namespace nsNature
	{
		/**
		 * @brief スカイキューブクラス
		*/
		class CSkyCube : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CSkyCube() = default;
			/**
			 * @brief デストラクタ
			*/
			~CSkyCube() = default;
		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief スタート関数
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 破棄されたときに呼ばれる関数
			*/
			void OnDestroy() override final;

		public:		// メンバ関数

			/**
			 * @brief 初期化関数
			 * @attention オブジェクトを生成してすぐに呼ばないと効果がありません。
			 * @param[in] skyCubeType スカイキューブのタイプ
			 * @param[in] isIBL IBLを行うか？
			*/
			void Init(const nsSkyCubeConstData::EnSkyCubeType skyCubeType, const bool isIBL = true);

			/**
			 * @brief 座標を設定する
			 * @param[in] pos 座標
			*/
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
				if (m_skyModelRender)
					m_skyModelRender->SetPosition(pos);
			}

			/**
			 * @brief 座標を得る
			 * @return 座標
			*/
			const Vector3& GetPosition() const
			{
				return m_position;
			}

			/**
			 * @brief 拡大率を設定する（Vector3）
			 * @param[in] scale 拡大率
			*/
			void SetScale(const Vector3& scale)
			{
				m_scale = scale;
				if (m_skyModelRender)
					m_skyModelRender->SetScale(m_scale);
			}

			/**
			 * @brief 拡大率を設定する（float）
			 * @param[in] scale 拡大率
			*/
			void SetScale(const float scale)
			{
				m_scale.x = scale;
				m_scale.y = scale;
				m_scale.z = scale;
				if (m_skyModelRender)
					m_skyModelRender->SetScale(m_scale);
			}

		private:	// privateなメンバ関数

			/**
			 * @brief テクスチャの初期化関数
			*/
			void InitTexture();

			/**
			 * @brief モデルレンダラーの初期化関数
			*/
			void InitModelRender();


		private:	// データメンバ
			nsGraphic::nsModel::CModelRender* m_skyModelRender = nullptr;	//!< スカイのモデルレンダラー
			Texture m_skyTexture;											//!< スカイのテクスチャ

			Vector3 m_position = Vector3::Zero;								//!< 座標
			Vector3 m_scale = nsSkyCubeConstData::kDefaultSkySclae;			//!< 拡大率
			
		};

	}
}