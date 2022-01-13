#include "stdafx.h"
#include "EffectPlayer.h"

namespace nsNinjaAttract
{
	/**
	 * @brief エフェクト関連のネームスペース
	*/
	namespace nsEffect
	{
		/**
		 * @brief 常に呼ばれるアップデート関数
		*/
		void CEffectPlayer::AlwaysUpdate() 
		{
			// エフェクトの更新
			m_effect.Update();

			return;
		}

		/**
		 * @brief 初期化関数
		 * @param[in] filePath ファイルパス
		*/
		void CEffectPlayer::Init(const char16_t* filePath)
		{
			// エフェクトの初期化
			m_effect.Init(filePath);

			return;
		}

		/**
		 * @brief エフェクトを再生する
		*/
		void CEffectPlayer::Play()
		{
			// エフェクトの再生
			m_effect.Play();

			return;
		}
	}
}