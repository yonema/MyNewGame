#pragma once
/*!
 * @brief	Noncopyポリシー
 */

#ifndef _TKNONCOPYABLE_H_
#define _TKNONCOPYABLE_H_



namespace nsNinjaAttract
{
	/**
	 * @brief Util関連のネームスペース
	*/
	namespace nsUtil
	{

		/**
		 * @brief コピー禁止
		*/
		struct Noncopyable {
			Noncopyable() = default;
			Noncopyable(const Noncopyable&) = delete;
			Noncopyable& operator=(const Noncopyable&) = delete;
		};
	}
}

#endif // _TKNONCOPYABLE_H_