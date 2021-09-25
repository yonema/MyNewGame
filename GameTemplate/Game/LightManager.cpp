#include "stdafx.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace nsMyGame
{
	/**
	 * @brief ���C�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLight
	{
		CLightManager* CLightManager::m_instance = nullptr;


		/**
		 * @brief �R���X�g���N�^
		*/
		CLightManager::CLightManager()
		{
			if (m_instance != nullptr) {
				// �C���X�^���X�����łɍ���Ă���B
				MessageBoxA(nullptr, "���ł�CLightManager�͍���Ă��܂��B", "�G���[", MB_OK);
				std::abort();
			}
			return;
		}

		/**
		 * @brief �f�X�g���N�^
		*/
		CLightManager::~CLightManager()
		{
			//nullptr�����Ă���
			m_instance = nullptr;
			return;
		}


		/**
		 * @brief �A�b�v�f�[�g�֐��B�Q�[�����[�v�ŌĂ�ł��������B
		*/
		void CLightManager::Update()
		{
			// ���_�X�V
			m_lightData.eyePos = g_camera3D->GetPosition();

			return;
		}

		/**
		 * @brief �f�B���N�V�������C�g��ǉ�����
		 * @param[in,out] light �ǉ�����f�B���N�V�������C�g
		*/
		void CLightManager::AddLight(CDirectionalLight* light)
		{
			// �f�B���N�V�������C�g�̐����ő吔�ȏォ�H
			if (m_lightData.directionalLightNum >= nsLightConstData::kMaxDirectionalLightNum)
			{
				// �f�B���N�V�������C�g�̐�����������
				MessageBoxA(nullptr, "�f�B���N�V�������C�g�̐����������܂�", "�G���[", MB_OK);
				std::abort();
				return;
			}

			//Manager�i���g�j�̂��ꂼ��̃f�B���N�V�������C�g�ւ̃|�C���^�Ƀ��C�g��n��
			m_directionalLightPtrs[m_lightData.directionalLightNum] = light;

			// ���C�g�Ƀ��C�g�f�[�^��n��
			light->SetLightDataPtr(m_lightData.directionalLightData[m_lightData.directionalLightNum]);
			// ���C�g�ɐ���i���o�[��n��
			light->SetControlNumber(m_lightData.directionalLightNum);

			// ���C�g�̐��𑝂₷
			m_lightData.directionalLightNum++;

			return;
		}

		/**
		 * @brief �|�C���g���C�g��ǉ�����
		 * @param[in,out] light �ǉ�����|�C���g���C�g
		*/
		void CLightManager::AddLight(CPointLight* light)
		{
			// �|�C���g���C�g�̐����ő吔�ȏォ�H
			if (m_lightData.pointLightNum >= nsLightConstData::kMaxPointLightNum)
			{
				// �|�C���g���C�g�̐�����������
				MessageBoxA(nullptr, "�|�C���g���C�g�̐����������܂�", "�G���[", MB_OK);
				std::abort();
				return;
			}

			//Manager�i���g�j�̂��ꂼ��̃|�C���g���C�g�ւ̃|�C���^�Ƀ��C�g��n��
			m_pointLightPtrs[m_lightData.pointLightNum] = light;

			// ���C�g�Ƀ��C�g�f�[�^��n��
			light->SetLightDataPtr(m_lightData.pointLightData[m_lightData.pointLightNum]);
			// ���C�g�ɐ���i���o�[��n��
			light->SetControlNumber(m_lightData.pointLightNum);

			// ���C�g�̐��𑝂₷
			m_lightData.pointLightNum++;
		}

		/**
		 * @brief �X�|�b�g���C�g��ǉ�����
		 * @param[in,out] light �ǉ�����X�|�b�g���C�g
		*/
		void CLightManager::AddLight(CSpotLight* light)
		{
			// �X�|�b�g���C�g�̐����ő吔�ȏォ�H
			if (m_lightData.spotLightNum >= nsLightConstData::kMaxSpotLightNum)
			{
				// �X�|�b�g���C�g�̐�����������
				MessageBoxA(nullptr, "�X�|�b�g���C�g�̐����������܂�", "�G���[", MB_OK);
				std::abort();
				return;
			}

			//Manager�i���g�j�̂��ꂼ��̃X�|�b�g���C�g�ւ̃|�C���^�Ƀ��C�g��n��
			m_spotLightPtrs[m_lightData.spotLightNum] = light;

			// ���C�g�Ƀ��C�g�f�[�^��n��
			light->SetLightDataPtr(m_lightData.spotLightData[m_lightData.spotLightNum]);
			// ���C�g�ɐ���i���o�[��n��
			light->SetControlNumber(m_lightData.spotLightNum);

			// ���C�g�̐��𑝂₷
			m_lightData.spotLightNum++;
		}

		/**
		 * @brief �f�B���N�V�������C�g��j������
		 * @param[in] light �j������f�B���N�V�������C�g
		*/
		void CLightManager::RemoveLight(const CDirectionalLight* light)
		{
			// �j�����郉�C�g�̐���i���o�[
			const int targetControlNum = light->GetControlNumber();

			// i = �j�����郉�C�g�̐���i���o�[
			// i + 1 = �������郉�C�g�̐���i���o�[
			// �������郉�C�g�̐���i���o�[�����݂̃��C�g�̐��ȉ��������烋�[�v
			for (int i = targetControlNum; i + 1 < m_lightData.directionalLightNum; i++)
			{
				// �j������f�[�^�����Ɏ����Ă����悤�ɕ��ёւ�
				// Manager�i���g�j�̃��C�g�f�[�^�́A�f�B���N�V�������C�g�̃f�[�^����ёւ�
				std::swap<nsLightData::SDirectionalLightData>(
					m_lightData.directionalLightData[i],
					m_lightData.directionalLightData[i + 1]
					);
				// Manager�i���g�j�̂��ꂼ��̃f�B���N�V�������C�g�ւ̃|�C���^����ёւ�
				std::swap<CDirectionalLight*>(m_directionalLightPtrs[i], m_directionalLightPtrs[i + 1]);
			}

			// �j�����郉�C�g�������Ă�����i���o�[���猻�݂̃��C�g�̐�-1�ȉ��������烋�[�v
			// �i��Ԍ��͔j�����郉�C�g�̂���-1�j
			for (int i = targetControlNum; i < m_lightData.directionalLightNum - 1; i++)
			{
				// ���ёւ��ς݂̃f�[�^m_lightData.directionalLightData�ɁA
				// ���ꂼ��̃f�B���N�V�������C�g�̃f�[�^�����킹��
				m_directionalLightPtrs[i]->SetLightDataPtr(m_lightData.directionalLightData[i]);
				// ����i���o�[�����ёւ�
				m_directionalLightPtrs[i]->SetControlNumber(i);
			}

			//�f�B���N�V�������C�g�̐������炷
			m_lightData.directionalLightNum--;

			return;
		}

		/**
		 * @brief �|�C���g���C�g��j������
		 * @param[in] light �j������|�C���g���C�g
		*/
		void CLightManager::RemoveLight(const CPointLight* light)
		{
			// �j�����郉�C�g�̐���i���o�[
			const int targetControlNum = light->GetControlNumber();

			// i = �j�����郉�C�g�̐���i���o�[
			// i + 1 = �������郉�C�g�̐���i���o�[
			// �������郉�C�g�̐���i���o�[�����݂̃��C�g�̐��ȉ��������烋�[�v
			for (int i = targetControlNum; i + 1 < m_lightData.pointLightNum; i++)
			{
				// �j������f�[�^�����Ɏ����Ă����悤�ɕ��ёւ�
				// Manager�i���g�j�̃��C�g�f�[�^�́A�|�C���g���C�g�̃f�[�^����ёւ�
				std::swap<nsLightData::SPointLightData>(
					m_lightData.pointLightData[i],
					m_lightData.pointLightData[i + 1]
					);
				// Manager�i���g�j�̂��ꂼ��̃|�C���g���C�g�ւ̃|�C���^����ёւ�
				std::swap<CPointLight*>(m_pointLightPtrs[i], m_pointLightPtrs[i + 1]);
			}

			// �j�����郉�C�g�������Ă�����i���o�[���猻�݂̃��C�g�̐�-1�ȉ��������烋�[�v
			// �i��Ԍ��͔j�����郉�C�g�̂���-1�j
			for (int i = targetControlNum; i < m_lightData.pointLightNum - 1; i++)
			{
				// ���ёւ��ς݂̃f�[�^m_lightData.pointLightData�ɁA
				// ���ꂼ��̃|�C���g���C�g�̃f�[�^�����킹��
				m_pointLightPtrs[i]->SetLightDataPtr(m_lightData.pointLightData[i]);
				// ����i���o�[�����ёւ�
				m_pointLightPtrs[i]->SetControlNumber(i);
			}

			//�|�C���g���C�g�̐������炷
			m_lightData.pointLightNum--;

			return;
		}

		/**
		 * @brief �X�|�b�g���C�g��j������
		 * @param[in] light �j������X�|�b�g���C�g
		*/
		void CLightManager::RemoveLight(const CSpotLight* light)
		{
			// �j�����郉�C�g�̐���i���o�[
			const int targetControlNum = light->GetControlNumber();

			// i = �j�����郉�C�g�̐���i���o�[
			// i + 1 = �������郉�C�g�̐���i���o�[
			// �������郉�C�g�̐���i���o�[�����݂̃��C�g�̐��ȉ��������烋�[�v
			for (int i = targetControlNum; i + 1 < m_lightData.spotLightNum; i++)
			{
				// �j������f�[�^�����Ɏ����Ă����悤�ɕ��ёւ�
				// Manager�i���g�j�̃��C�g�f�[�^�́A�X�|�b�g���C�g�̃f�[�^����ёւ�
				std::swap<nsLightData::SSpotLightData>(
					m_lightData.spotLightData[i],
					m_lightData.spotLightData[i + 1]
					);
				// Manager�i���g�j�̂��ꂼ��̃X�|�b�g���C�g�ւ̃|�C���^����ёւ�
				std::swap<CSpotLight*>(m_spotLightPtrs[i], m_spotLightPtrs[i + 1]);
			}

			// �j�����郉�C�g�������Ă�����i���o�[���猻�݂̃��C�g�̐�-1�ȉ��������烋�[�v
			// �i��Ԍ��͔j�����郉�C�g�̂���-1�j
			for (int i = targetControlNum; i < m_lightData.spotLightNum - 1; i++)
			{
				// ���ёւ��ς݂̃f�[�^m_lightData.spotLightData�ɁA
				// ���ꂼ��̃X�|�b�g���C�g�̃f�[�^�����킹��
				m_spotLightPtrs[i]->SetLightDataPtr(m_lightData.spotLightData[i]);
				// ����i���o�[�����ёւ�
				m_spotLightPtrs[i]->SetControlNumber(i);
			}

			//�X�|�b�g���C�g�̐������炷
			m_lightData.spotLightNum--;

			return;
		}

	}
}