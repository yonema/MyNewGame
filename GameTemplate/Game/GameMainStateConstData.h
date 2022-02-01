#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief ゲームステート関連のネームスペース
	*/
	namespace nsGameState
	{
		/**
		 * @brief ゲームメインのステートの定数データ
		*/
		namespace nsGameMainStateConstData
		{
			/**
			 * @brief ゲームメインのステートのステート
			*/
			enum EnGameMainStateState
			{
				enGS_startDirecting,
				enGS_inGame,
				enGS_beforeClearDirecting,
				enGS_startFadeOutToClearDirecting,
				enGS_fadeOutToClearDirecting,
				enGS_clearDirecting,
				enGS_result,
				enGS_lastJump,
				enGS_goTitle
			};

			constexpr float kStartClearDirectingTime = 5.5f;
		}
	}
}