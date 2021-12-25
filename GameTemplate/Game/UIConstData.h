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

			constexpr float kStageWidth = 70000.0f;				//!< ステージの幅
			constexpr float kStageHeight = 70000.0f;			//!< ステージの高さ
			constexpr float kPlayerCenterBuffWidth = 35000.0f;	//!< プレイヤーをセンターに寄せるための幅
			constexpr float kPlayerCenterBuffHeight = 2800.0f;	//!< プレイヤーをセンターに寄せるための高さ

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
			static const Vector2 kMiniMapSpritePosition = { 500.0f,-250.0f};

			//!< プレイヤーのアイコンのスプライトのファイルパス
			constexpr const char* const kPlayerIconSpriteFilePath = "Assets/Image/miniMap/playerArrow.DDS";
			constexpr int kPlayerIconSpriteWidth = static_cast<int>(64 * 0.5f);	//! プレイヤーアイコンの幅
			constexpr int kPlayerIconSpriteHeight = static_cast<int>(64 * 0.5f);	//! プレイヤーアイコンの高さ
			// プレイヤーのアイコンのスプライトの座標
			static const Vector2 kPlayerIconSpritePosition = kMiniMapSpritePosition;

			//!<  車のアイコンのスプライトのファイルパス
			constexpr const char* const kCarIconSpriteFilePath = "Assets/Image/miniMap/carIcon.DDS";
			constexpr int kCarIconSpriteWidth = static_cast<int>(512 * 0.1f);	//! 車アイコンの幅
			constexpr int kCarIconSpriteHeight = static_cast<int>(512 * 0.1f);	//! 車アイコンの高さ

			//!<  車のミニアイコンのスプライトのファイルパス
			constexpr const char* const kCarMiniIconSpriteFilePath = "Assets/Image/miniMap/carMiniIcon.DDS";
			constexpr int kCarMiniIconSpriteWidth = static_cast<int>(64 * 0.5f);	//! 車ミニアイコンの幅
			constexpr int kCarMiniIconSpriteHeight = static_cast<int>(64 * 0.5f);	//! 車ミニアイコンの高さ

			//!<  車のミニアイコンの画面はみだし用のスプライトのファイルパス
			constexpr const char* const kCarMiniIconOutSpriteFilePath = "Assets/Image/miniMap/outOfView.DDS";
			constexpr int kCarMiniIconOutSpriteWidth = static_cast<int>(64 * 0.25f);	//! 車ミニアイコンの幅
			constexpr int kCarMiniIconOutSpriteHeight = static_cast<int>(64 * 0.25f);	//! 車ミニアイコンの高さ
			constexpr float kCarMiniIconOutBuff = 10.0f;	//!< 普通の車アイコンとずらす量

			// ミニマップの4頂点
			static const Vector2 kMiniMapVert[4] =
			{
				// 左上
				{
					kPlayerIconSpritePosition.x - kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y + kMiniMapDrawHeight * 0.5f
				},
				// 右上
				{
					kPlayerIconSpritePosition.x + kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y + kMiniMapDrawHeight * 0.5f
				},
				// 左下
				{
					kPlayerIconSpritePosition.x - kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y - kMiniMapDrawHeight * 0.5f
				},
				// 右下
				{
					kPlayerIconSpritePosition.x + kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y - kMiniMapDrawHeight * 0.5f
				},

			};

			// ミニマップの辺のベクトル
			static Vector2 kMiniMapEdge[4]
			{
				// 左上から右上。始点vert0。
				{
					kMiniMapVert[1].x - kMiniMapVert[0].x,
					kMiniMapVert[1].y - kMiniMapVert[0].y
				},
				// 右上から右下。始点vert1。
				{
					kMiniMapVert[3].x - kMiniMapVert[1].x,
					kMiniMapVert[3].y - kMiniMapVert[1].y
				},
				// 左下から左上。始点vert2。
				{
					kMiniMapVert[0].x - kMiniMapVert[2].x,
					kMiniMapVert[0].y - kMiniMapVert[2].y
				},
				// 右下から左下。始点vert3。
				{
					kMiniMapVert[2].x - kMiniMapVert[3].x,
					kMiniMapVert[2].y - kMiniMapVert[3].y
				},
			};
			
		}
	}
}