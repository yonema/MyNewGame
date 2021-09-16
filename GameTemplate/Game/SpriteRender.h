#pragma once
#include "SpriteRenderConstData.h"

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief スプライト関連のネームスペース
		*/
		namespace nsSprite
		{
			/**
			 * @brief スプライト描画クラス
			*/
			class CSpriteRender : public IGameObject
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CSpriteRender() = default;

				/**
				 * @brief デストラクタ
				*/
				~CSpriteRender() = default;

			public:		// オーバーライドしたメンバ関数
				/**
				 * @brief スタート関数
				 * @return アップデートを行うか？
				*/
				bool Start() override final { return true; };

				/**
				 * @brief 破棄されたときの処理
				*/
				void OnDestroy() override final;

				/**
				 * @brief 常に呼ばれるアップデート関数
				*/
				void AlwaysUpdate() override final;

				/**
				 * @brief 描画処理関数
				 * @param[in] rc レンダーコンテキスト
				*/
				void Render(RenderContext & rc) override final;

			public:		// メンバ関数
				/**
				 * @brief 初期化用関数。幅の引数が<float>バージョン。最初に呼んでね。
				 * @attention 幅はUINTにキャストされるので、小数点以下のデータは失われる。
				 * @param[in] texFilePath ddsファイルパス
				 * @param[in] w スプライトの横幅
				 * @param[in] h スプライトの縦幅
				 * @param[in] pivot ピボット（基点）
				 * @param[in] alphaBlendMode アルファブレンディングモード
				*/
				void Init(
					const char* texFilePath,
					const float w,
					const float h,
					const Vector2& pivot = nsSpriteConstData::kDefaultPivot,
					const AlphaBlendMode alphaBlendMode = AlphaBlendMode_None
				);

				/**
				 * @brief 初期化用関数。幅の引数が<int>バージョン。最初に呼んでね。
				 * @param[in] texFilePath ddsファイルパス
				 * @param[in] w スプライトの横幅
				 * @param[in] h スプライトの縦幅
				 * @param[in] pivot ピボット（基点）
				 * @param[in] alphaBlendMode アルファブレンディングモード
				*/
				void Init(
					const char* texFilePath,
					const int w,
					const int h,
					const Vector2& pivot = nsSpriteConstData::kDefaultPivot,
					const AlphaBlendMode alphaBlendMode = AlphaBlendMode_None
				);

				/**
				 * @brief 座標を設定
				 * @param[in] pos 座標
				*/
				void SetPosition(const Vector3& pos)
				{
					m_position = pos;
				}

				/**
				 * @brief 座標を取得
				 * @return 座標
				*/
				const Vector3& GetPosition() const
				{
					return m_position;
				}

				/**
				 * @brief  拡大率を設定
				 * @param[in] scale 拡大率
				*/
				void SetScale(const Vector3& scale)
				{
					m_scale = scale;
				}

				/**
				 * @brief 拡大率を取得
				 * @return 拡大率
				*/
				const Vector3& GetScale() const
				{
					return m_scale;
				}

				/**
				 * @brief 回転を設定
				 * @param[in] rot 回転
				*/
				void SetRotation(const Quaternion& rot)
				{
					m_rotation = rot;
				}

				/**
				 * @brief 回転を取得
				 * @return 回転
				*/
				const Quaternion& GetRotation() const
				{
					return m_rotation;
				}

			private:	// privateなメンバ関数

				/**
				 * @brief 初期化用関数のメインコア。Init関数の中で呼ばれる。
				 * @param[in] texFilePath ddsファイルパス
				 * @param[in] w スプライトの横幅
				 * @param[in] h スプライトの縦幅
				 * @param[in] pivot ピボット（基点）
				 * @param[in] alphaBlendMode アルファブレンディングモード
				*/
				void InitMainCore(
					const char* texFilePath,
					const UINT w,
					const UINT h,
					const Vector2& pivot,
					const AlphaBlendMode alphaBlendMode
				);

			private:	// データメンバ
				Sprite m_sprite;		//!< スプライトクラス
				Vector3 m_position = Vector3::Zero;
				Quaternion m_rotation = Quaternion::Identity;
				Vector3 m_scale = Vector3::One;
				Vector2 m_pivot = nsSpriteConstData::kDefaultPivot;


			};
		}
	}
}
