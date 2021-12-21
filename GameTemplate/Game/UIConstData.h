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

			//!< ミニマップの背景用スプライトのファイルパス
			constexpr const char* const kMiniMapBackSpriteFilePath = "Assets/Image/miniMap/map_back.DDS";
			//!< ミニマップのスプライトのファイルパス
			constexpr const char* const kMiniMapSpriteFilePath = "Assets/Image/miniMap/map.DDS";
			//!< ミニマップのFXファイルパス
			constexpr const char* const kMiniMapFxFilePath = "Assets/shader/miniMap.fx";
			// 元の画像サイズ * 大きさ倍率
			constexpr int kMiniMapSpriteWidth = static_cast<int>(720 * 0.8f);	//!< ミニマップの幅
			constexpr int kMiniMapSpriteHeight = static_cast<int>(720 * 0.8f);	//!< ミニマップの高さ
			// 元の画像サイズ * アスペクト比 * 大きさ倍率
			constexpr int kMiniMapDrawWidth = static_cast<int>(720 * 0.4f * 0.7f);	//!< ミニマップの幅
			constexpr int kMiniMapDrawHeight = static_cast<int>(720 * 0.3f * 0.7f);	//!< ミニマップの高さ
			//!< ミニマップのスプライトの座標
			static const Vector3 kMiniMapSpritePosition = { 500.0f,-250.0f,0.0f };

			//!< プレイヤーのアイコンのスプライトのファイルパス
			constexpr const char* const kPlayerIconSpriteFilePath = "Assets/Image/miniMap/playerArrow.DDS";
			constexpr int kPlayerIconSpriteWidth = static_cast<int>(64 * 0.5f);	//! プレイヤーアイコンの幅
			constexpr int kPlayerIconSpriteHeight = static_cast<int>(64 * 0.5f);	//! プレイヤーアイコンの高さ
			// プレイヤーのアイコンのスプライトの座標
			static const Vector3 kPlayerIconSpritePosition = kMiniMapSpritePosition;
			
		}
	}
}