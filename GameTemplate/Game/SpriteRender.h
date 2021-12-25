#pragma once
#include "SpriteRenderConstData.h"
#include "Render.h"

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
				CSpriteRender();

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
				 * @brief 描画オブジェクト登録の入口
				*/
				void AddRenderEntrance() override final;

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
				 * @brief 初期化データによる初期化処理。最初に呼んでね。
				 * @param[in] spriteInitData スプライトの初期化データ
				*/
				void Init(const SpriteInitData& spriteInitData);

				/**
				 * @brief 座標を設定
				 * @param[in] pos 座標
				*/
				void SetPosition(const Vector3& pos)
				{
					m_position = pos;
				}

				/**
				 * @brief 座標を設定（Vector2）
				 * @param[in] pos 座標
				*/
				void SetPosition(const Vector2& pos)
				{
					m_position.x = pos.x;
					m_position.y = pos.y;
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

				/**
				 * @brief 乗算カラーを設定する
				 * @param[in] mulColor 乗算カラー
				*/
				void SetMulColor(const Vector4& mulColor)
				{
					m_sprite.SetMulColor(mulColor);
				}

				/**
				 * @brief 乗算カラーを得る
				 * @return 乗算カラー
				*/
				const Vector4& GetMulColor() const
				{
					return m_sprite.GetMulColor();
				}

				/**
				 * @brief スプライトのアルファ値を設定する
				 * @param[in] alphaValue アルファ値
				*/
				void SetAlphaValue(const float alphaValue)
				{
					m_sprite.SetAlphaValue(alphaValue);
				}

				/**
				 * @brief 基点を設定
				 * @param[in] pivot 基点
				*/
				void SetPivot(const Vector2& pivot)
				{
					m_pivot = pivot;
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

				/**
				 * @brief 2D描画関数
				 * @param rc レンダリングコンテキスト
				*/
				void Render2D(RenderContext& rc);

			private:	// データメンバ
				Sprite m_sprite;		//!< スプライトクラス
				CRender m_render;		//!< レンダラークラス
				Vector3 m_position = Vector3::Zero;						//!< 座標
				Quaternion m_rotation = Quaternion::Identity;			//!< 回転
				Vector3 m_scale = Vector3::One;							//!< 拡大率
				Vector2 m_pivot = nsSpriteConstData::kDefaultPivot;		//!< ピボット


			};
		}
	}
}
