#pragma once
#include "Render.h"

namespace nsMyGame
{
	namespace nsGraphic
	{
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
	}

	/**
	 * @brief デバック関連のネームスペース
	*/	
	namespace nsDebug
	{
		/**
		 * @brief ベクトル描画クラス
		*/
		class CVectorRender : public IGameObject
		{
		public:		// 構造体

			/**
			 * @brief ベクトル描画データ
			*/
			struct SVectorRenderData {
				std::string name = "vector";		//!< 名前
				Vector3 vector = Vector3::One;		//!< ベクトル。
				Vector3 origin = Vector3::Zero;		//!< 基点。
			};

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CVectorRender() = default;
			/**
			 * @brief デストラクタ
			*/
			~CVectorRender() = default;

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief Updateの直前で呼ばれる開始処理
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 消去される時に呼ばれる処理
			*/
			void OnDestroy() override final;

			/**
			 * @brief 常に呼ばれる更新処理
			*/
			void AlwaysUpdate() override final;

			/**
			 * @brief 描画オブジェクト登録の入口
			*/
			void AddRenderEntrance() override final;
			
		public:		// メンバ関数

			/**
			 * @brief 描画するベクトルを追加
			 * @param renderData 
			*/
			void AddVector(const SVectorRenderData& renderData)
			{
				m_vectorRenderDatas.emplace_back(std::move(renderData));
			}

			/**
			 * @brief 
			*/
			void ResetVectorRenderDatas()
			{
				m_vectorRenderDatas.clear();
			}

		private:	// privateなメンバ関数

			/**
			 * @brief 2D描画関数
			 * @param rc レンダリングコンテキスト
			*/
			void Render2D(RenderContext& rc);

		private:	// データメンバ
			std::vector<SVectorRenderData> m_vectorRenderDatas;
			nsGraphic::CRender m_render;										//!< レンダラークラス
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;
			Font m_font;

		};

	}
}