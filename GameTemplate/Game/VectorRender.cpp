#include "stdafx.h"
#include "VectorRender.h"
#include "ModelRender.h"
#include "RenderingEngine.h"


namespace nsMyGame
{
	/**
	 * @brief デバック関連のネームスペース
	*/
	namespace nsDebug
	{
		// デバックの定数データを使用可能にする
		using namespace nsDebugConstData;
		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CVectorRender::Start()
		{
			// モデルレンダラーの生成と初期化
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			m_modelRender->Init(kVectorModel);
			// 無効化
			m_modelRender->Deactivate();

			// レンダラークラスに2D描画関数を設定する
			m_render.SetOnRender2DFunc([&](RenderContext& rc) { this->Render2D(rc); });

			// フォントのシャドウパラメータを設定
			m_font.SetShadowParam(true, kVectorFontShadowOffset, Vector4::Black);

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CVectorRender::OnDestroy()
		{
			DeleteGO(m_modelRender);

			return;
		}

		/**
		 * @brief 常に呼ばれる更新処理
		*/
		void CVectorRender::AlwaysUpdate()
		{
			// ベクトルレンダラーのデータ集が空か？
			if (m_vectorRenderDatas.empty())
			{
				// 空のため、無効化
				m_modelRender->Deactivate();

				// 早期リターン
				return;
			}

			// 有効化
			m_modelRender->Activate();
			
			// ベクトルレンダラーのデータ集からすべて取り出して、モデルのパラメータを更新する
			for (const auto& vectorRenderData : m_vectorRenderDatas)
			{
				// 回転
				Quaternion rot;
				// ベクトルの向きに回転させる
				rot.SetRotation(Vector3::AxisZ, vectorRenderData.vector);

				// 拡大率
				// 現在のベクトルモデルの長さが100のため、100で除算
				float scale = vectorRenderData.vector.Length() / kVectorModelLength;
				// 幅と高さの倍率は下げる
				float wideAndHeight = scale * kVectorModelWidthAndHeightScale;

				// ベクトルモデルのトランスフォームを設定する
				m_modelRender->SetPosition(vectorRenderData.origin);
				m_modelRender->SetRotation(rot);
				m_modelRender->SetScale({ wideAndHeight, wideAndHeight, scale });
			}

			return;
		}

		/**
		 * @brief 描画オブジェクト登録の入口
		*/
		void CVectorRender::AddRenderEntrance()
		{
			// 描画オブジェクトに登録する
			nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject(&m_render);
		}

		/**
		 * @brief 2D描画関数
		 * @param[in,out] rc レンダリングコンテキスト
		*/
		void CVectorRender::Render2D(RenderContext& rc)
		{
			// フォント描画開始
			m_font.Begin(rc);

			// ベクトルレンダラーのデータ集からすべて取り出して、フォントのパラメータを更新する
			for (const auto& vectorRenderData : m_vectorRenderDatas)
			{
				Vector4 pos;
				pos.Set(vectorRenderData.origin);
				const auto& vpMatrix = g_camera3D->GetViewProjectionMatrix();
				vpMatrix.Apply(pos);
				pos.x /= pos.w;
				pos.y /= pos.w;
				pos.z /= pos.w;

				//スクリーン空間に変換する。
				float half_w = g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
				float half_h = g_graphicsEngine->GetFrameBufferHeight() * 0.5f;

				pos.x *= half_w;
				pos.y *= half_h;

				// ベクトルのテキスト表示用パラメータ
				const Vector2 pivot = { 0.0f,1.0f };
				wchar_t text[256];
				wchar_t wc[126];
				mbstowcs(wc, vectorRenderData.name.c_str(), vectorRenderData.name.length() + 1);
				Vector2 textPos = { pos.x, pos.y - 4.0f };
				float between = 40.0f * m_fontScale;

				// ベクトルの名前を表示
				swprintf_s(text, L"%s", wc);
				m_font.Draw(text, textPos, Vector4::White, 0.0f, m_fontScale, pivot);

				//ベクトルデータを表示。
				memset(text, 0, sizeof(text));
				textPos.y -= between;
				swprintf_s(
					text,
					L"vec[ %.3f, %.3f, %.3f ]",
					vectorRenderData.vector.x,
					vectorRenderData.vector.y,
					vectorRenderData.vector.z
				);
				m_font.Draw(text, textPos, Vector4::White, 0.0f, m_fontScale, pivot);

				//向きデータを表示。
				auto dir = vectorRenderData.vector;
				textPos.y -= between;
				dir.Normalize();
				memset(text, 0, sizeof(text));
				swprintf_s(
					text,
					L"dir[ %.3f, %.3f, %.3f ]",
					dir.x,
					dir.y,
					dir.z
				);
				m_font.Draw(text, textPos, Vector4::White, 0.0f, m_fontScale, pivot);

				//スカラーを表示。
				memset(text, 0, sizeof(text));
				textPos.y -= between;
				swprintf_s(
					text,
					L"scalar[ %.3f ]",
					vectorRenderData.vector.Length()
				);
				m_font.Draw(text, textPos, Vector4::White, 0.0f, m_fontScale, pivot);
			}

			// フォントの描画終了
			m_font.End(rc);
			
			// ベクトルのデータ集をクリアする
			m_vectorRenderDatas.clear();
			return;
		}

	}
}