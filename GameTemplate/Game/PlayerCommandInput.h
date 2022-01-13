#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsPlayer { class CPlayer; }
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		/**
		 * @brief プレイヤーのコマンド入力クラス
		*/
		class CPlayerCommandInput : private nsUtil::Noncopyable
		{
		private:	// エイリアス宣言
			// QTEに使用するボタンの種類
			using EnQTEButtonType = nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType;

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayerCommandInput();
			/**
			 * @brief デストラクタ
			*/
			~CPlayerCommandInput() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in] playerRef プレイヤーの参照
			*/
			void Init(const CPlayer& playerRef)
			{
				m_playerRef = &playerRef;
			}

			/**
			 * @brief コマンドを初期化する
			 * @param[in] comNum コマンドの数
			 * @param[in] comType コマンドタイプ
			*/
			void InitCommand(
				const int comNum,
				const nsPlayerConstData::nsCommandInputConstData::EnCommandType comType
			);

			/**
			 * @brief コマンド入力を実行する
			*/
			void Execute();

			/**
			 * @brief コマンド入力が終わったか？を得る
			 * @return コマンド入力が終わったか？trueで終了。falseで終了していない。
			*/
			bool IsEndCommandInput() const
			{
				return m_isEndCommandInput;
			}
			
			/**
			 * @brief コマンド入力を終了させる
			*/
			void EndCommandInput()
			{
				m_isEndCommandInput = true;
			}

			/**
			 * @brief コマンドの配列を得る
			 * @return コマンドの配列
			*/
			const std::vector<EnQTEButtonType>& GetCommandButtonTypeArray() const
			{
				return m_commandButtonTypeArray;
			}

			/**
			 * @brief コマンドの進行度を得る
			 * @return コマンドの進行度
			*/
			int GetCommandProgress() const
			{
				return m_commandProgress;
			}

			/**
			 * @brief コマンドの結果を得る
			 * @return コマンドの結果
			*/
			nsPlayerConstData::nsCommandInputConstData::EnCommandResult GetCommandResult() const
			{
				return m_commondResult;
			}

		private:	// privateなメンバ関数

			/**
			 * @brief 乱数を初期化
			*/
			void InitRand();

		private:	// データメンバ
			std::vector<EnQTEButtonType> m_commandButtonTypeArray;			//!< コマンドの配列

			std::unique_ptr<std::mt19937> m_mt;							//!< メルセンヌツイスターの32ビット版
			//!< 範囲付きの一様乱数、コマンドの種類別の乱数
			std::unique_ptr<std::uniform_int_distribution<>>
				m_randByType[nsPlayerConstData::nsCommandInputConstData::enCT_typeNum];
			//!< 範囲付きの一様乱数、コマンドの種類を選ぶの乱数
			std::unique_ptr<std::uniform_int_distribution<>> m_randSelectType;

			int m_commandProgress = 0;	//!< コマンドの進行度
			bool m_isEndCommandInput = true;	//!< コマンド入力が終了したか？
			//!< コマンドの結果
			nsPlayerConstData::nsCommandInputConstData::EnCommandResult m_commondResult =
				nsPlayerConstData::nsCommandInputConstData::enCR_None;
			const CPlayer* m_playerRef = nullptr;
		};

	}
}