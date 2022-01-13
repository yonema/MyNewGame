#include "stdafx.h"
#include "SpriteRender.h"
#include "RenderingEngine.h"

namespace nsNinjaAttract
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
			 * @brief コンストラクタ
			*/
			CSpriteRender::CSpriteRender()
			{
				m_render.SetOnRender2DFunc([&](RenderContext& rc) {this->Render2D(rc); });

				return;
			}

			/**
			 * @brief 破棄されたときの処理
			*/
			void CSpriteRender::OnDestroy()
			{
				return;
			}

			/**
			 * @brief 常に呼ばれるアップデート関数
			*/
			void CSpriteRender::AlwaysUpdate()
			{
				//スプライトの更新
				m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
				return;
			}

			/**
			 * @brief 描画オブジェクト登録の入口
			*/
			void CSpriteRender::AddRenderEntrance()
			{
				// 描画オブジェクトに登録する
				nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject(&m_render);

				return;
			}

			/**
			 * @brief 2D描画関数
			 * @param rc レンダリングコンテキスト
			*/
			void CSpriteRender::Render2D(RenderContext& rc)
			{
				//スプライトの描画
				m_sprite.Draw(rc);

				return;
			}

			/**
			 * @brief 初期化用関数。幅の引数が<float>バージョン。最初に呼んでね。
			 * @attention 幅はUINTにキャストされるので、小数点以下のデータは失われる。
			 * @param [in] texFilePath ddsファイルパス
			 * @param [in] w スプライトの横幅
			 * @param [in] h スプライトの縦幅
			 * @param [in] pivot ピボット（基点）
			 * @param [in] alphaBlendMode アルファブレンディングモード
			*/
			void CSpriteRender::Init(
				const char* texFilePath,
				const float w,
				const float h,
				const Vector2& pivot,
				const AlphaBlendMode alphaBlendMode
			)
			{
				//初期化用関数のメインコア
				InitMainCore(texFilePath, static_cast<UINT>(w), static_cast<UINT>(h), pivot, alphaBlendMode);

				return;
			}

			/**
			 * @brief 初期化用関数。幅の引数が<int>バージョン。最初に呼んでね。
			 * @param [in] texFilePath ddsファイルパス
			 * @param [in] w スプライトの横幅
			 * @param [in] h スプライトの縦幅
			 * @param [in] pivot ピボット（基点）
			 * @param [in] alphaBlendMode アルファブレンディングモード
			*/
			void CSpriteRender::Init(
				const char* texFilePath,
				const int w,
				const int h,
				const Vector2& pivot,
				const AlphaBlendMode alphaBlendMode
			)
			{
				//初期化用関数のメインコア
				//intをfloatにキャストしてから渡す
				InitMainCore(texFilePath, static_cast<UINT>(w), static_cast<UINT>(h), pivot, alphaBlendMode);

				return;
			}

			/**
			 * @brief 初期化データによる初期化処理。最初に呼んでね。
			 * @param[in] spriteInitData スプライトの初期化データ
			*/
			void CSpriteRender::Init(const SpriteInitData& spriteInitData)
			{
				//スプライトの初期化
				m_sprite.Init(spriteInitData);

				//スプライトの更新
				m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);

				return;
			}

			/**
			 * @brief 初期化用関数のメインコア。Init関数の中で呼ばれる。
			 * @param [in] texFilePath ddsファイルパス
			 * @param [in] w スプライトの横幅
			 * @param [in] h スプライトの縦幅
			 * @param [in] pivot ピボット（基点）
			 * @param [in] alphaBlendMode アルファブレンディングモード
			*/
			void CSpriteRender::InitMainCore(
				const char* texFilePath,
				const UINT w,
				const UINT h,
				const Vector2& pivot,
				const AlphaBlendMode alphaBlendMode
			)
			{
				using namespace nsSpriteConstData;
				//スプライトの初期化用データ
				SpriteInitData initData;
				initData.m_ddsFilePath[0] = texFilePath;
				initData.m_width = w;
				initData.m_height = h;
				initData.m_fxFilePath = kDefaultFxFilePath;
				initData.m_psEntryPoinFunc = kPsEntryPointFunc;
				initData.m_vsEntryPointFunc = kVsEntryPointFunc;
				initData.m_alphaBlendMode = alphaBlendMode;
				// メインレンダリングターゲットのカラーバッファーフォーマットと合わせる
				initData.m_colorBufferFormat[0] = nsMyEngine::CRenderingEngine::GetInstance()->GetMainRenderTargetFormat();

				m_pivot = pivot;

				//スプライトの初期化
				m_sprite.Init(initData);

				//スプライトの更新
				m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);

				return;
			}

		}
	}
}