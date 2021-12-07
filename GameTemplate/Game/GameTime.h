#pragma once
/*!
 *@brief	�Q�[�����ԃN���X�B
 * �V���O���g���B
 */

#include "StopWatch.h"
#include <queue>
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief �^�C�}�[
	*/
	namespace nsTimer
	{

		/*!
		 *@brief	�Q�[�����ԁB
		 */
		class CGameTime : private nsUtil::Noncopyable
		{
		public:
			CGameTime()
			{
				m_font.SetShadowParam(true, 2.0f, Vector4::Black);
			}
			~CGameTime() = default;
		public:  //�����o�֐�

			/// <summary>
			/// �Q�[���^�C���̃C���X�^���X���擾
			/// </summary>
			/// <returns>�Q�[���^�C���̃C���X�^���X</returns>
			static CGameTime& GetInstance()
			{
				static CGameTime t;
				return t;
			}
			/*!
			 *@brief	1�t���[���̌o�ߎ��Ԃ��擾(�P�ʁE�b)�B
			 */
			const float GetFrameDeltaTime() const
			{
				//�Œ�FPS�ɂ���B�ς͗v�����B
				//return 1.0f / 30.0f;
				return m_frameDeltaTime;
			}

			void PushFrameDeltaTime(float deltaTime)
			{
				float a = max(1.0f / 144.0f, deltaTime);
				float b = min(a, 1.0f / 30.0f);
				m_frameDeltaTimeQue.push_back(min(max(1.0f / 144.0f, deltaTime), 1.0f / 30.0f));
				if (m_frameDeltaTimeQue.size() > 30.0f) {
					float totalTime = 0.0f;
					for (auto time : m_frameDeltaTimeQue) {
						totalTime += time;
					}
					//���ϒl���Ƃ�B
					m_frameDeltaTime = min(1.0f / 30.0f, totalTime / m_frameDeltaTimeQue.size());
					m_frameDeltaTimeQue.pop_front();
				}
			}

			/// <summary>
			/// FPS��`�悷��
			/// </summary>
			/// <param name="rc">�����_�����O�R���e�L�X�g</param>
			/// <param name="elapsedTime">�o�ߎ���</param>
			void DrawFPS(RenderContext& rc, float elapsedTime);


		private:
			std::list<float> m_frameDeltaTimeQue;
			float m_frameDeltaTime = 1.0f / 60.0f;		//1�t���[���̌o�ߎ��ԁB
			float m_timeTotal = 0.0f;	//�J�E���g�t���[�����̌o�ߎ��Ԃ̑��a
			float m_fps = 0.0f;			//�\������FPS
			Font m_font;				//�t�H���g

		};

		/// <summary>
		/// �Q�[���^�C���̃C���X�^���X���擾
		/// </summary>
		/// <returns>�Q�[���^�C���̃C���X�^���X</returns>
		static CGameTime& GameTime()
		{
			return CGameTime::GetInstance();
		}

	}
}