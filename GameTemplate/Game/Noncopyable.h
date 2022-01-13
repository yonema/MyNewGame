#pragma once
/*!
 * @brief	Noncopy�|���V�[
 */

#ifndef _TKNONCOPYABLE_H_
#define _TKNONCOPYABLE_H_



namespace nsNinjaAttract
{
	/**
	 * @brief Util�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUtil
	{

		/**
		 * @brief �R�s�[�֎~
		*/
		struct Noncopyable {
			Noncopyable() = default;
			Noncopyable(const Noncopyable&) = delete;
			Noncopyable& operator=(const Noncopyable&) = delete;
		};
	}
}

#endif // _TKNONCOPYABLE_H_