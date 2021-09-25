/*!
 *@brief	�Q�[���I�u�W�F�N�g�̊��N���X�B
 */

#pragma once

#include <list>
#include <string>
#include "../../GameTemplate/Game/Noncopyable.h"
class RenderContext;

/*!
*@brief	�Q�[���I�u�W�F�N�g�B
* �R�s�[�֎~�p��
*/
class IGameObject : private nsMyGame::nsUtil::Noncopyable
{
public:
	/*!
		*@brief	�f�X�g���N�^
		*/
	virtual ~IGameObject()
	{
	}
public:

	
	/*!
	*@brief	Update�̒��O�ŌĂ΂��J�n�����B
	*@details
	* 
	* �{�֐���true��Ԃ��ƃQ�[���I�u�W�F�N�g�̏��������������Ɣ��f�����</br>
	* Update�֐����Ă΂�o���܂��Btrue��Ԃ��Ĉȍ~��Start�֐��͌Ă΂�Ȃ��Ȃ�܂��B</br>
	* �Q�[���I�u�W�F�N�g�̏������ɕ����t���[��������ꍇ�Ȃǂ�false��Ԃ��āA�������X�e�b�v�Ȃǂ��g����</br>
	* �K�؂ɏ��������s���Ă��������B
	*/
	virtual bool Start() { return true; }
	/*!
		*@brief	�X�V
		*/
	virtual void Update() {}
	///*!
	// *@brief	�`��
	//*/
	//virtual void Render(RenderContext& renderContext)
	//{
	//	(void)renderContext;
	//}
	
public:
	/*!
	*@brief Start�֐������������H
	*/
	bool IsStart() const
	{
		return m_isStart;
	}
	/*!
	*@brief �A�N�e�B�u���ǂ����𔻒�B
	*/
	bool IsActive() const
	{
		return m_isActive;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���B
	/// </summary>
	void Activate()
	{
		m_isActive = true;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���B
	/// </summary>
	void Deactivate()
	{
		m_isActive = false;
	}
	
	/// <summary>
	/// ���S������B
	/// </summary>
	void Dead()
	{
		m_isDead = true;
	}
	/// <summary>
	/// ���S���Ă���H
	/// </summary>
	/// <returns>true���Ԃ��Ă����玀�S���Ă���</returns>
	bool IsDead() const
	{
		return m_isDead;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̖��O��ݒ�B
	/// </summary>
	/// <param name="name">���O</param>
	void SetName(const char* name)
	{
		if (name != nullptr) {
			m_name = name;
		}
	}
	/// <summary>
	/// �����œn���ꂽ���O���A���̃Q�[���I�u�W�F�N�g�̖��O�ƃ}�b�`���邩����B
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool IsMatchName(const char* name) const
	{
		if (strcmp(m_name.c_str(), name) == 0) {
			return true;
		}
		return false;
	}
public:

	//void RenderWrapper(RenderContext& renderContext)
	//{
	//	if (m_isActive && m_isStart && !m_isDead ) {
	//		Render(renderContext);
	//	}
	//}
	
	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead ) {
			Update();
		}
	}
	void StartWrapper()
	{
		if (m_isActive && !m_isStart && !m_isDead ) {
			if (Start()) {
				//���������������B
				m_isStart = true;
			}
		}
	}

		
	friend class CGameObjectManager;
protected:
	std::string m_name;								//�Q�[���I�u�W�F�N�g�̖��O
	bool m_isStart = false;							//Start�̊J�n�t���O�B
	bool m_isDead = false;							//���S�t���O�B
	bool m_isRegistDeadList = false;				//���S���X�g�ɐς܂�Ă���B
	bool m_isNewFromGameObjectManager;	//GameObjectManager��new���ꂽ�B
	bool m_isRegist = false;							//GameObjectManager�ɓo�^����Ă���H
	bool m_isActive = true;							//Active�t���O�B


//�ǉ�
public:		//�I�[�o�[���C�h���Ăق��������o�֐�

	/**
	 * @brief �|�[�Y���̂݌Ă΂��A�b�v�f�[�g�֐�
	*/
	virtual void UpdateOnlyPaused() {};

	/**
	 * @brief �|�[�Y���ł��|�[�Y���łȂ��Ă��֌W�Ȃ���ɌĂ΂��A�b�v�f�[�g�֐�
	*/
	virtual void AlwaysUpdate() {};

	/**
	 * @brief ��������鎞�ɌĂ΂�鏈��
	 * @details CGameManager::DeleteGameObject���Ă񂾂Ƃ��Ɏ��s�����B
	 * �f�X�g���N�^���O�Ɏ��s�����
	*/
	virtual void OnDestroy() {};

	/**
	 * @brief �`��I�u�W�F�N�g��o�^��������ƂȂ�֐��B
	 * ���̊֐����I�[�o�[���C�h���āA���̒��ŁA
	 * nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject()��
	 * �`��I�u�W�F�N�g�Ƃ��ēo�^����B
	*/
	virtual void AddRenderEntrance() {};

public:		//�����o�֐�

	/**
	 * @brief �|�[�Y���A�b�v�f�[�g�̃��b�p�[
	*/
	void UpdateOnlyPausedWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			UpdateOnlyPaused();
		}
	}

	/**
	 * @brief ��ɌĂ΂��A�b�v�f�[�g�̃��b�p�[
	*/
	void AlwaysUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			AlwaysUpdate();
		}
	}

	/**
	 * @brief �������ꂽ���ɌĂ΂��֐��̃��b�p�[
	*/
	void OnDestroyWrapper()
	{
		// �f�X�g���C�C�x���g�����s����B
		m_destroyEvent();

		OnDestroy();
	}

	/**
	 * @brief �`��I�u�W�F�N�g�o�^�̓���
	*/
	void AddRenderEntranceWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			AddRenderEntrance();
		}
	}

	/**
	 * @brief �f�X�g���C�C�x���g��ݒ�B���g���j�������Ƃ��ɌĂ΂��C�x���g�B
	 * @param[in] destroyEvent �f�X�g���C�C�x���g�B
	*/
	void SetDestroyEvent(const std::function<void(void)> destroyEvent)
	{
		m_destroyEvent = destroyEvent;
	}

private:	// �f�[�^�����o
	//!< �f�X�g���C�C�x���g�B���g���j�������Ƃ��ɌĂ΂��C�x���g�B
	std::function<void(void)> m_destroyEvent = []() {return; };
	
};
