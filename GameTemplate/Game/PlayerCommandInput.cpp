#include "stdafx.h"
#include "PlayerCommandInput.h"
#include "Player.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		using namespace nsPlayerConstData::nsCommandInputConstData;

		/**
		 * @brief �R���X�g���N�^
		*/
		CPlayerCommandInput::CPlayerCommandInput()
		{
			// ����������������
			InitRand();

			// InitCommand���ĂԂ܂ł́A�I�����Ă���悤�ɂ���
			m_isEndCommandInput = true;

			return;
		}

		/**
		 * @brief �R�}���h������������
		 * @param[in] comNum �R�}���h�̐�
		 * @param[in] comType �R�}���h�^�C�v
		*/
		void CPlayerCommandInput::InitCommand(
			const int comNum,
			const nsPlayer::nsPlayerConstData::nsCommandInputConstData::EnCommandType comType
		)
		{
			// �R�}���h�̔z������Z�b�g
			m_commandButtonTypeArray.clear();
			// �R�}���h�̐i�s�x�����Z�b�g
			m_commandProgress = 0;
			// �R�}���h���͂��I�����Ă��Ȃ��悤�ɂ���
			m_isEndCommandInput = false;

			// �R�}���h�̔z����A�R�}���h�̐��������T�[�u�B
			m_commandButtonTypeArray.reserve(comNum);

			// �R�}���h�^�C�v�̌��̔z��
			std::vector<EnCommandType> comTypeCandidateArray;
			// �R�}���h�̐��������T�[�u
			comTypeCandidateArray.reserve(comNum);
			// �w�肳�ꂽ�R�}���h�^�C�v�͐�΂ɓ����
			comTypeCandidateArray.emplace_back(comType);
			// �c��̃R�}���h�̌��������_���Ō��߂�
			for (int i = 0; i < comNum - 1; i++)
			{
				// �w�肳�ꂽ�^�C�v�ȉ��̃R�}���h�^�C�v����A�����_���őI�ԁB
				const int randSelectType = (*m_randSelectType)(*m_mt) % (comType + 1);
				comTypeCandidateArray.emplace_back(static_cast<EnCommandType>(randSelectType));
			}

			for (int i = 0; i < comNum; i++)
			{
				// ���������̔z��̃T�C�Y�Ŋ����āA�z��̃����_���ȗv�f�ԍ��𓾂�B
				const int randSelectType = (*m_randSelectType)(*m_mt) % comTypeCandidateArray.size();
				// ���̒�����^�C�v������Ă��āA���̃^�C�v�̗������o���B
				const int randByType = (*m_randByType[comTypeCandidateArray[randSelectType]])(*m_mt);
				
				// ��x�g�p�������́A�z�񂩂��������B
				comTypeCandidateArray.erase(comTypeCandidateArray.begin() + randSelectType);

				// �R�}���h�̔z��ɓo�^
				m_commandButtonTypeArray.emplace_back(static_cast<EnQTEButtonType>(randByType));
			}

			return;
		}

		/**
		 * @brief �R�}���h���͂����s����
		*/
		void CPlayerCommandInput::Execute()
		{
			if (m_commandProgress >= m_commandButtonTypeArray.size() || m_isEndCommandInput == true)
			{
				// �R�}���h�i�s�x���A�R�}���h�̔z��ȏ�A�܂͂��A
				// �R�}���h���͂��I�����Ă�����A�������Ȃ��B�������^�[���B
				return;
			}

			if (m_commandButtonTypeArray[m_commandProgress] == m_playerRef->GetInputData().inputCommand)
			{
				// �R�}���h�̔z��ƁA�R�}���h���͏�񂪈�v���Ă�����A�R�}���h�i�s�x��i�߂�B
				m_commandProgress++;
			}

			if (m_commandProgress >= m_commandButtonTypeArray.size())
			{
				// �R�}���h�i�s�x���A�R�}���h�̔z��ȏ�ɂȂ�����A�R�}���h���͂��I������B
				m_isEndCommandInput = true;
			}
			

			return;
		}

		/**
		 * @brief ������������
		*/
		void CPlayerCommandInput::InitRand()
		{
			// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
			std::random_device rnd;
			// �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
			m_mt = std::make_unique<std::mt19937>(rnd());
			// [a, b] �͈͂̈�l����
			m_randByType[enCT_type1] = std::make_unique<std::uniform_int_distribution<>>(
				EnQTEButtonType::enQTE_A,
				EnQTEButtonType::enQTE_X
				);
			m_randByType[enCT_type2] = std::make_unique<std::uniform_int_distribution<>>(
				EnQTEButtonType::enQTE_L_Up,
				EnQTEButtonType::enQTE_L_Right
				);
			m_randByType[enCT_type3] = std::make_unique<std::uniform_int_distribution<>>(
				EnQTEButtonType::enQTE_L1,
				EnQTEButtonType::enQTE_R2
				);

			m_randSelectType = std::make_unique<std::uniform_int_distribution<>>(0, 99);

			//const int randTargetNum = (*m_rand)(*m_mt);

			return;
		}
	}
}