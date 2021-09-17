#pragma once
#include "SoundSource.h"

namespace nsMyGame
{
	/**
	 * @brief サウンド
	*/
	namespace nsSound
	{

		/**
		 * @brief サウンドキュークラス
		 * サウンド再生クラス、CSoundSourceを使ってもよい。
		*/
		class CSoundCue : public IGameObject
		{
		public:		//列挙型

			/**
			 * @brief サウンドタイプ
			*/
			enum EnSoundType
			{
				enSE,	// SE
				enBGM	// BGM
			};
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CSoundCue() = default;
			/**
			 * @brief デストラクタ
			*/
			~CSoundCue();
		public:		// メンバ関数

			/**
			 * @brief 初期化関数
			 * @param filePath wavファイルパス
			 * @param soundType サウンドタイプ
			*/
			void Init(const wchar_t* filePath, const EnSoundType soundType);

			/**
			 * @brief サウンド再生
			 * @param[in] isLoop ループ再生か？
			*/
			void Play(const bool isLoop);

			/**
			 * @brief ボリュームを設定。1.0fが通常のボリューム。
			 * @attention 負の値を入れると、その絶対値が反映される
			 * @param volume ボリューム
			*/
			void SetVolume(const float volume);

			/**
			 * @brief ボリュームを取得
			 * @return ボリューム
			*/
			float GetVolume() const
			{
				return m_volume;
			}

			/**
			 * @brief サウンドの再生を停止する。
			 * @details この関数で停止すると、次に再生した時最初からの再生になる。
			 * @attention 今の仕様だと、ループ再生は停止できるがワンショット再生は停止できない。
			*/
			void Stop()
			{
				//ループ再生用のサウンドソースが使われていたら
				if (!m_loopSoundSource)
					return;
				//サウンドを停止する
				m_loopSoundSource->Stop();
				m_isPlaying = false;
				return;
			}

			/**
			 * @brief サウンドの再生を一時停止する
			 * @derails この関数で一時停止すると、次に再生した時途中からの再生になる。
			 * @attention 今の仕様だと、ループ再生は一時停止できるがワンショット再生は一時停止できない。
			*/
			void Pause()
			{
				//ループ再生用のサウンドソースが使われていたら
				if (!m_loopSoundSource)
					return;
				//サウンドを一時停止する
				m_loopSoundSource->Pause();
				m_isPlaying = false;
				return;
			}

			/**
			 * @brief 今再生中か？
			 * @attention 今の仕様だと、ループ再生の再生中は検知できるがワンショット再生の再生中は検知できない。
			*/
			bool IsPlaying() const
			{
				return  m_isPlaying;
			}

			/**
			 * @brief サウンドのタイプを設定
			 * @details CSoundCue::enSEかCSoundCue::enBGMでタイプを設定する。
			 * @param soundType サウンドタイプ
			*/
			void SetSoundType(const EnSoundType soundType)
			{
				m_soundType = soundType;
			}

		public:		//staticなメンバ関数

			/**
			 * @brief SEのボリュームを設定する。1.0fが通常。
			 * @param SEVolume SEのボリューム
			*/
			static void SetSEVolume(const float SEVolume)
			{
				m_SEVolume = SEVolume;
			}

			/**
			 * @brief BGMのボリュームを設定する。1.0fが通常
			 * @param BGMVolume 
			*/
			static void SetBGMVolume(const float BGMVolume)
			{
				m_BGMVolume = BGMVolume;
			}
			

		private:	//privateなメンバ関数

			/**
			 * @brief ループ再生の処理
			*/
			void PlayLoop();

			/**
			 * @brief ワンショット再生の処理
			*/
			void PlayOneShot();

			/**
			 * @brief タイプ別のボリュームを取得
			 * @return タイプ別のボリューム
			*/
			float GetTypeVolume()
			{
				float volume = 1.0f;

				switch (m_soundType)
				{
				case enSE:
					volume = m_SEVolume;
					break;
				case enBGM:
					volume = m_BGMVolume;
					break;
				}

				return volume;
			}


		private:	//データメンバ
			CSoundSource* m_loopSoundSource = nullptr;	//!< ループ再生用のサウンドソース
			wchar_t m_filePath[256];					//!< wavファイルパス
			float m_volume = 1.0f;						//!< ボリューム
			bool m_isPlaying = false;					//!< 再生中か？
			EnSoundType m_soundType = enSE;				//!< サウンドタイプ

		private:	//staticなデータメンバ
			static float m_SEVolume;					//!< SEのボリューム
			static float m_BGMVolume;					//!< BGMのボリューム

		};
	}
}

