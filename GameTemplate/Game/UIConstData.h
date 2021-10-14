#pragma once

namespace nsMyGame
{
	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		/**
		 * @brief ゲームメインUIクラスの定数データ
		*/
		namespace nsGameMainUIConstData
		{
			//!< タイマーのフォントのテキストの先頭
			constexpr const wchar_t* const kTimerFontTextHead = L"Time:";
			//!< タイマーのフォントのテキストの末尾
			constexpr const wchar_t* const kTimerFontTextEnd = L"%02d:%02d";
			//!< タイマーのフォントの座標
			static const Vector2 kTimerFontPosition = { -600.0f,300.0f };
			//!< タイマーのフォントのカラー
			static const Vector4 kTimerFontColor = { 1.0f,1.0f,1.0f,1.0f };
			//!< タイマーのフォントのシャドウオフセット
			constexpr float kTimerFontShadowOffset = 2.0f;
			//!< タイマーのフォントのシャドウカラー
			static const Vector4 kTimerFontShadowColor = { 0.0f,0.0f,0.0f,1.0f };

			//!< ゴールのスプライトファイルパス
			constexpr const char* const kGoalSpriteFilePath = "Assets/Image/sample.dds";
			constexpr int kGoalSpriteWidth = 256;	//!< ゴールのスプライトの幅
			constexpr int kGoalSpriteHeight = 256;	//!< ゴールのスプライトの高さ
			//!< ゴールのスプライトの座標
			static const Vector3 kGoalSpritePosition = { 0.0f,200.0f,0.0f };

		}
	}
}