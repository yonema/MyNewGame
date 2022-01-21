#pragma once
#include "PlayerInput.h"
#include "SavedPlayerInputDataConstData.h"

namespace nsNinjaAttract
{
	/**
	 * @brief 外部データ用ネームスペース
	*/
	namespace nsExternalData
	{
		/**
		 * @brief プレイヤーの入力情報の保存データクラス
		*/
		class CSavedPlayerInputData
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CSavedPlayerInputData() = default;
			/**
			 * @brief デストラクタ
			*/
			~CSavedPlayerInputData() = default;

		public:		// メンバ関数

			/**
			 * @brief 保存する情報を追加する
			 * @param[in] saveData 保存するデータ
			*/
			void PushBuckData(const nsPlayer::SPlayerInputDataAndDeltaTime& saveData);

			/**
			 * @brief 収集したプレイヤーの入力情報を保存する
			 * @param[in] saveFileType 保存ファイルの種類
			*/
			void SaveData(const nsSavedPlayerInputDataConstData::EnSavedFileType savedFileType);

			/**
			 * @brief 収集したプレイヤーの入力情報をクリアする
			*/
			void ClearSaveData();

			/**
			 * @brief 保存してあるプレイヤーの入力情報をロードする
			 * @param[in] saveFileType 保存ファイル
			*/
			void LoadData(const nsSavedPlayerInputDataConstData::EnSavedFileType savedFileType);

			/**
			 * @brief ロードしたデータをクリアする
			*/
			void ClearLoadData();

			/**
			 * @brief ロードしたデータを読み込んで、進める。
			 * @return 現在の進捗のロードしたデータ
			*/
			nsPlayer::SPlayerInputDataAndDeltaTime* GetAndProgressLoadData();

			/**
			 * @brief ロードしたデータの進捗をリセット
			*/
			void ResetLoadDataProgress()
			{
				m_loadDataProgress = 0;
			}

			/**
			 * @brief ロードしたデータの進捗が、最後まで行っているか？を得る。
			 * @return ロードしたデータの進捗が、最後まで行っているか？
			*/
			bool IsEndLoadDataProgress() const
			{
				// 進捗が、サイズを超えていたら、もう最後まで進んでいる。
				return m_loadDataProgress >= m_loadData.size();
			}

		private:	// データメンバ
			std::list<nsPlayer::SPlayerInputDataAndDeltaTime> m_saveData;	//!< 保存用データ
			std::vector<nsPlayer::SPlayerInputDataAndDeltaTime> m_loadData;	//!< ロードしたデータ
			int m_loadDataProgress = 0;										//!< ロードしたデータの進捗
		};

	}
}