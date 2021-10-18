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
		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CVectorRender::Start()
		{
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			m_modelRender->Init("Assets/modelData/preset/vector.tkm");
			m_modelRender->Deactivate();

			m_render.SetOnRender2DFunc([&](RenderContext& rc) { this->Render2D(rc); });

			m_font.SetShadowParam(true, 1.0f, Vector4::Black);

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
			if (m_vectorRenderDatas.empty())
			{
				m_modelRender->Deactivate();
			}
			else
			{
				m_modelRender->Activate();
			}

			for (const auto& vectorRenderData : m_vectorRenderDatas)
			{
				Quaternion rot;
				rot.SetRotation(Vector3::AxisZ, vectorRenderData.vector);
				float s = vectorRenderData.vector.Length() / 100.0f;
				float wideAndHeight = s * 0.2f;
				wideAndHeight = max(wideAndHeight, 1.0f);
				m_modelRender->SetPosition(vectorRenderData.origin);
				m_modelRender->SetRotation(rot);
				m_modelRender->SetScale({ wideAndHeight, wideAndHeight, s });
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
		 * @param rc レンダリングコンテキスト
		*/
		void CVectorRender::Render2D(RenderContext& rc)
		{
			m_font.Begin(rc);

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

				//ベクトルデータを表示。
				wchar_t text[256];
				swprintf_s(
					text,
					L"vec[ %.3f, %.3f, %.3f ]",
					vectorRenderData.vector.x,
					vectorRenderData.vector.y,
					vectorRenderData.vector.z
				);
				m_font.Draw(text, { pos.x, pos.y - 4.0f }, Vector4::White, 0.0f, 0.4f, { 0.0f, 1.0f });
				//向きデータを表示。
				auto dir = vectorRenderData.vector;
				dir.Normalize();
				memset(text, 0, sizeof(text));
				swprintf_s(
					text,
					L"dir[ %.3f, %.3f, %.3f ]",
					dir.x,
					dir.y,
					dir.z
				);
				m_font.Draw(text, { pos.x, pos.y - 20.0f }, Vector4::White, 0.0f, 0.4f, { 0.0f, 1.0f });
				//スカラーを表示。
				memset(text, 0, sizeof(text));
				swprintf_s(
					text,
					L"scalar[ %.3f ]",
					vectorRenderData.vector.Length()
				);
				m_font.Draw(text, { pos.x, pos.y - 36.0f }, Vector4::White, 0.0f, 0.4f, { 0.0f, 1.0f });
			}
			m_font.End(rc);
			
			m_vectorRenderDatas.clear();
			return;
		}

	}
}