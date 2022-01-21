#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief 外部データ用ネームスペース
	*/
	namespace nsExternalData
	{

		/**
		 * @brief プレイヤーの入力情報の保存データクラスの定数データ
		*/
		namespace nsSavedPlayerInputDataConstData
		{
			/**
			 * @brief 保存ファイルの種類
			*/
			enum EnSavedFileType
			{
				enST_titleDirecting,
				enSavedFileTypeNum
			};


			//!< 書き出しファイルパス
			static const char* const kOutputFilePath[enSavedFileTypeNum]
			{
				"Assets/outputData/playerInputData/titleDirecting.dat"
			};

		}
	}
}