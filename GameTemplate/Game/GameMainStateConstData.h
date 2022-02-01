#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief �Q�[���X�e�[�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGameState
	{
		/**
		 * @brief �Q�[�����C���̃X�e�[�g�̒萔�f�[�^
		*/
		namespace nsGameMainStateConstData
		{
			/**
			 * @brief �Q�[�����C���̃X�e�[�g�̃X�e�[�g
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