#pragma once
#include "../../ExEngine/effect/Effect.h"

namespace nsNinjaAttract
{
	/**
	 * @brief エフェクト関連のネームスペース
	*/
	namespace nsEffect
	{
		/**
		 * @brief エフェクト再生クラス
		*/
		class CEffectPlayer : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			CEffectPlayer() = default;	// コンストラクタ
			~CEffectPlayer() = default;	// デストラクタ

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief スタート関数
			 * @return アップデートを行うか
			*/
			bool Start() override final { return true; };

			/**
			 * @brief 常に呼ばれるアップデート関数
			*/
			void AlwaysUpdate() override final;

		public:		// メンバ関数

			/**
			 * @brief 初期化関数
			 * @param[in] filePath ファイルパス
			*/
			void Init(const char16_t* filePath);

			/**
			 * @brief エフェクトを再生する
			*/
			void Play();

			/**
			 * @brief エフェクトを停止する
			*/
			void Stop()
			{
				m_effect.Stop();
			}

			/**
			 * @brief 座標を設定
			 * @param[in] pos 座標
			*/
			void SetPosition(const Vector3& pos)
			{
				m_effect.SetPosition(pos);
			}

			/**
			 * @brief 座標を取得
			 * @return 座標
			*/
			const Vector3& GetPosition() const
			{
				return m_effect.GetPosition();
			}

			/**
			 * @brief 回転を設定
			 * @param[in] rot 回転
			*/
			void SetRotation(const Quaternion& rot)
			{
				m_effect.SetRotation(rot);
			}

			/**
			 * @brief 回転を取得
			 * @return 回転
			*/
			const Quaternion& GetRotation() const
			{
				return m_effect.GetRotation();
			}

			/**
			 * @brief 拡大率を設定（Vector3）
			 * @param[in] scale 拡大率
			*/
			void SetScale(const Vector3& scale)
			{
				m_effect.SetScale(scale);
			}

			/**
			 * @brief 拡大率を設定（float）
			 * @param[in] scale 拡大率
			*/
			void SetScale(const float scale)
			{
				m_effect.SetScale(scale);
			}

			/**
			 * @brief 拡大率を取得
			 * @return 拡大率
			*/
			const Vector3& GetScale() const
			{
				return m_effect.GetScale();
			}

			/**
			 * @brief エフェクトが再生中か？
			 * @return 再生中か？
			*/
			bool IsPlay() const
			{
				return m_effect.IsPlay();
			}

		private:	//データメンバ
			Effect m_effect;	//!< エフェクトクラス
		};
	}
}


