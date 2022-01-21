#include "stdafx.h"
#include "SavedPlayerInputData.h"

namespace nsNinjaAttract
{
	/**
	 * @brief 外部データ用ネームスペース
	*/
	namespace nsExternalData
	{
		using namespace nsSavedPlayerInputDataConstData;

		/**
		 * @brief 保存する情報を追加する
		 * @param[in] saveData 保存するデータ
		*/
		void CSavedPlayerInputData::PushBuckData(const nsPlayer::SPlayerInputDataAndDeltaTime& saveData)
		{
			m_saveData.emplace_back(saveData);

			return;
		}

		/**
		 * @brief 収集したプレイヤーの入力情報を書きだす
		 * @param[in] saveFileType 保存ファイルの種類
		*/
		void CSavedPlayerInputData::SaveData(const EnSavedFileType savedFileType)
		{
			// ファイルをバイナリ書き込みモードで開く
			auto fp = fopen(kOutputFilePath[savedFileType], "wb");

			// 最初にデータの数を書き込む
			const size_t dataNum = m_saveData.size();
			fwrite(&dataNum, sizeof(dataNum), 1, fp);

			for (const auto& saveData : m_saveData)
			{
				// データの中身を書き込む
				fwrite(&saveData, sizeof(saveData), 1, fp);
			}

			//ファイルを閉じる
			fclose(fp);
			return;
		}

		/**
		 * @brief 収集したプレイヤーの入力情報をクリアする
		*/
		void CSavedPlayerInputData::ClearSaveData()
		{
			m_saveData.clear();

			return;
		}

		/**
		 * @brief 保存してあるプレイヤーの入力情報をロードする
		 * @param[in] saveFileType 保存ファイル
		*/
		void CSavedPlayerInputData::LoadData(
			const nsSavedPlayerInputDataConstData::EnSavedFileType savedFileType
		)
		{
			if (m_loadData.empty() != true)
			{
				// ロードデータが空ではなかったら、クリアする。
				ClearLoadData();
			}

			// ファイルをバイナリ読み込みモードで開く
			auto fp = fopen(kOutputFilePath[savedFileType], "rb");

			if (fp == nullptr)
			{
				// ファイルオープンに失敗。
				return;
			}

			// 最初にデータの数を読み込む
			size_t dataNum = 0;
			fread(&dataNum, sizeof(dataNum), 1, fp);

			// データの数だけリサーブ
			m_loadData.reserve(dataNum);

			for (int i = 0; i < dataNum; i++)
			{
				// データの中身を読み込む
				nsPlayer::SPlayerInputDataAndDeltaTime loadData;
				fread(&loadData, sizeof(loadData), 1, fp);
				m_loadData.emplace_back(loadData);
			}

			// ファイルをクローズ
			fclose(fp);

			return;
		}

		/**
		 * @brief ロードしたデータをクリアする
		*/
		void CSavedPlayerInputData::ClearLoadData()
		{
			m_loadData.clear();

			// ロードしたデータの進捗をリセット
			ResetLoadDataProgress();

			return;
		}

		/**
		 * @brief ロードしたデータを読み込んで、進める。
		 * @return 現在の進捗のロードしたデータ
		*/
		nsPlayer::SPlayerInputDataAndDeltaTime* CSavedPlayerInputData::GetAndProgressLoadData()
		{
			// 現在の進捗のロードしたデータ
			nsPlayer::SPlayerInputDataAndDeltaTime* loadData = nullptr;

			if (IsEndLoadDataProgress())
			{
				// 最後まで進んでいる
				return loadData;
			}

			// 現在の進捗のデータを格納する
			loadData = &m_loadData[m_loadDataProgress];

			// 進捗を進める
			m_loadDataProgress++;

			return loadData;
		}
	}
}