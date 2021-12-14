#pragma once
#include "AICharacterBase.h"

namespace nsMyGame
{
	/**
	 * @brief AIキャラクター関連のネームスペース
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief AIの車クラス
		*/
		class CAICar : public CAICharacterBase
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CAICar() = default;
			/**
			 * @brief デストラクタ
			*/
			~CAICar() = default;

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief 派生クラス用のUpdateの直前で呼ばれる開始処理
			 * @return アップデートを行うか？
			*/
			bool StartSub() override final;

			/**
			 * @brief 派生クラス用の消去される時に呼ばれる処理
			*/
			void OnDestroySub() override final {};

			/**
			 * @brief 派生クラス用の更新処理
			*/
			void UpdateSub() override final;

		public:		// メンバ関数

		private:	// データメンバ

			
		};

	}
}