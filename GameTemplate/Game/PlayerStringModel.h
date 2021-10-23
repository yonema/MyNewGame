#pragma once
#include "ModelRender.h"

namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic
	{
		namespace nsModel { class CModelRender; }	//モデルレンダラークラス
	}

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 前方宣言
		class CPlayer;		// プレイヤークラス

		/**
		 * @brief プレイヤーの糸のモデルクラス
		*/
		class CPlayerStringModel : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayerStringModel() = default;
			/**
			 * @brief デストラクタ
			*/
			~CPlayerStringModel() = default;

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief Updateの直前で呼ばれる開始処理
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 更新処理
			*/
			void OnDestroy() override final;
			
			/**
			 * @brief 消去される時に呼ばれる処理
			*/
			void Update() override final;

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in] player プレイヤー
			*/
			void Init(const CPlayer& player)
			{
				m_playerRef = &player;
			}

			/**
			 * @brief 指定した座標に向かって伸びる処理を開始する
			 * @param[in] pos 伸びる先の座標
			*/
			void StartStretchToPos(const Vector3& pos);

			/**
			 * @brief 指定した座標に向かって伸びる処理を終了する
			*/
			void EndStretchToPos();

			/**
			 * @brief 伸びきっているか？を得る
			 * @return 伸びきっているか？
			*/
			bool IsStretched() const
			{
				return m_isStretched;
			}

			/**
			 * @brief 糸の長さを得る
			 * @return 糸の長さ
			*/
			float GetStringLength() const
			{
				return m_modelRender->GetScale().z;
			}

		private:	// privateなメンバ関数

			/**
			 * @brief モデルを回転させる処理
			*/
			void ModelRotation();

		private:	// データメンバ
			const CPlayer* m_playerRef = nullptr;						//!< プレイヤーの参照
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< モデルレンダラー
			bool m_isStretched = true;									//!< 伸びきっているか？
			Vector3 m_toStretchPos = Vector3::Zero;						//!< 伸びる先の座標
			float m_stretchSpeed = 0.0f;								//!< 伸びる速度
		};

	}
}