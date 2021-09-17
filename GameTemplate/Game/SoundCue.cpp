#include "stdafx.h"
#include "SoundCue.h"

namespace nsMyGame
{
	/**
	 * @brief サウンド
	*/
	namespace nsSound
	{
		float CSoundCue::m_SEVolume = 1.0f;
		float CSoundCue::m_BGMVolume = 1.0f;

		/**
		 * @brief デストラクタ
		*/
		CSoundCue::~CSoundCue()
		{
			//CSoundCueオブジェクトをDeleteGOした際、再生中だった場合、停止させる。
			if (m_loopSoundSource) {
				DeleteGO(m_loopSoundSource);
			}

			return;
		}


		/**
		 * @brief 初期化関数
		 * @param filePath wavファイルパス
		 * @param soundType サウンドタイプ
		*/
		void CSoundCue::Init(const wchar_t* filePath, const EnSoundType soundType)
		{
			//引数で渡された const wchat_t* を、
			//データメンバの wchar の配列に入れる
			swprintf_s(m_filePath, filePath);

			//一度、作って破棄することで、
			//Flyweightパターンを使用しているWaveFileBankに
			//登録させる。
			CSoundSource* ss = NewGO<CSoundSource>(nsCommonData::enPriorityFirst);
			ss->Init(m_filePath);
			DeleteGO(ss);

			return;
		}

		/**
		 * @brief サウンド再生
		 * @param[in] isLoop ループ再生か？
		*/
		void CSoundCue::Play(const bool isLoop)
		{
			// 再生中にする
			m_isPlaying = true;

			// ループ再生かワンショット再生かで処理を振り分ける
			if (isLoop)
			{
				// ループ再生
				PlayLoop();
			}
			else
			{
				// ワンショット再生
				PlayOneShot();
			}

			return;
		}

		/**
		 * @brief ループ再生の処理
		*/
		void CSoundCue::PlayLoop()
		{
			// ループ再生用のサウンドソースが
			// すでに作れれていたら
			if (m_loopSoundSource)
			{
				// 一時停止したものを再開する
				m_loopSoundSource->Play(true);

			}
			else
			{
				// ループ再生用のサウンドソースが
				// まだ未使用の場合は
				// 新しく作って再生する。
				m_loopSoundSource = NewGO<CSoundSource>(nsCommonData::enPriorityFirst);
				m_loopSoundSource->Init(m_filePath);
				m_loopSoundSource->Play(true);
				float vol = m_volume * GetTypeVolume();
				SetVolume(vol);
			}

			return;
		}

		/**
		 * @brief ワンショット再生の処理
		*/
		void CSoundCue::PlayOneShot()
		{
			// 毎回、サウンドソースを作って再生させる
			// Flyweightパターンを使用しているから
			// 重くならないぜ！
			CSoundSource* ss = NewGO<CSoundSource>(nsCommonData::enPriorityFirst);
			ss->Init(m_filePath);
			float vol = m_volume * GetTypeVolume();
			ss->SetVolume(vol);	// OSの場合はここでボリュームを設定する
			ss->Play(false);

			return;
		}

		/**
		 * @brief ボリュームを設定。1.0fが通常のボリューム。
		 * @attention 負の値を入れると、その絶対値が反映される
		 * @param volume ボリューム
		*/
		void CSoundCue::SetVolume(const float volume)
		{
			// ボリュームを保持
			m_volume = volume;

			// ループ再生用のサウンドソースが使われていたら
			if (!m_loopSoundSource)
				return;

			// ループ再生用のサウンドソースのボリュームを設定する
			float vol = m_volume * GetTypeVolume();
			m_loopSoundSource->SetVolume(vol);

			return;
		}

	}
}