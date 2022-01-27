#pragma once

namespace nsNinjaAttract
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

		/**
		 * @brief ミニマップの定数データ
		*/
		namespace nsMiniMapConstData
		{
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
			static const Vector2 kMiniMapSpritePosition = { 500.0f,-250.0f };

			// ミニマップの枠用のスプライトのファイルパス
			constexpr const char* const kMiniMapFrameSpriteFilePath = "Assets/Image/miniMap/map_waku.DDS";
			constexpr int kMiniMapFrameSpriteWidth = static_cast<int>(400 * 0.62f);	//!< ミニマップの枠の幅
			constexpr int kMiniMapFrameSpriteHeight = static_cast<int>(300 * 0.62f);	//!< ミニマップの枠の高さ
			//!< ミニマップの枠用のスプライトの座標
			static const Vector2 kMiniMapFrameSpritePosition =
			{ kMiniMapSpritePosition.x + 10.0f, kMiniMapSpritePosition.y + 8.0f };

			//!< プレイヤーのアイコンのスプライトのファイルパス
			constexpr const char* const kPlayerIconSpriteFilePath = "Assets/Image/miniMap/playerArrow.DDS";
			constexpr int kPlayerIconSpriteWidth = static_cast<int>(64 * 0.5f);	//! プレイヤーアイコンの幅
			constexpr int kPlayerIconSpriteHeight = static_cast<int>(64 * 0.5f);	//! プレイヤーアイコンの高さ
			// プレイヤーのアイコンのスプライトの座標
			static const Vector2 kPlayerIconSpritePosition = kMiniMapSpritePosition;

			//!<  車のアイコンのスプライトのファイルパス
			constexpr const char* const kCarIconSpriteFilePath = "Assets/Image/miniMap/carIcon.DDS";
			constexpr int kCarIconSpriteWidth = static_cast<int>(512 * 0.08f);	//! 車アイコンの幅
			constexpr int kCarIconSpriteHeight = static_cast<int>(512 * 0.08f);	//! 車アイコンの高さ
			constexpr float kCarIconSpriteWorldPosYBuff = 100.0f;	// 車のワールド座標のY座標をずらす量
			constexpr float kDisplayCarIconTime = 5.0f;	//!< 車のアイコンを表示しておく時間
			//!< 車アイコンのデフォルトカラー
			static const Vector3 kCarIconDefaultColor = { 1.0f,0.0f,0.0f };
			//!< 車アイコンの範囲外カラー、近いバージョン
			static const Vector3 kCarIconOutColorNear = { 1.0f,0.3f,0.0f };
			//!< 車アイコンの範囲外カラー、遠いバージョン
			static const Vector3 kCarIconOutColorFar = { 1.0f,1.0f,0.0f };
			constexpr float kCarIconOutMaxRage = 300.0f;	//!< 車アイコンの範囲外の最大距離


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
			static Vector2 kMiniMapEdge[4] =
			{
				// 左上から右上。始点vert0。
				{ kMiniMapVert[1].x - kMiniMapVert[0].x, kMiniMapVert[1].y - kMiniMapVert[0].y },
				// 右上から右下。始点vert1。
				{ kMiniMapVert[3].x - kMiniMapVert[1].x, kMiniMapVert[3].y - kMiniMapVert[1].y },
				// 左下から左上。始点vert2。
				{ kMiniMapVert[0].x - kMiniMapVert[2].x, kMiniMapVert[0].y - kMiniMapVert[2].y },
				// 右下から左下。始点vert3。
				{ kMiniMapVert[2].x - kMiniMapVert[3].x, kMiniMapVert[2].y - kMiniMapVert[3].y },
			};

		}

		/**
		 * @brief エネミーを捕まえる処理の定数データ
		*/
		namespace nsEnemyCatchUIConstData
		{

			enum EnCatchEnemySpriteType
			{
				enCanCatchEnemyMain,
				enCanCatchEnemyFrame1,
				enCanCatchEnemyFrame2,
				enCanCatchEnemySpriteNum
			};
			//!< 敵を捕まえることができる合図のスプライトのファイルパス
			constexpr const char* const kCanCatchEnemySpriteFilePath[enCanCatchEnemySpriteNum] = 
			{
				"Assets/Image/qte/y.DDS",
				"Assets/Image/qte/canCatch_frame1.DDS",
				"Assets/Image/qte/canCatch_frame2.DDS"
			};

			//!< 敵を捕まえることができる合図のスプライトの幅
			constexpr int kCanCatchEnemySpriteWidht[enCanCatchEnemySpriteNum] = 
			{
				static_cast<int>(256 * 0.2f),
				static_cast<int>(512 * 0.2f),
				static_cast<int>(512 * 0.2f)
			};
			//!< 敵を捕まえることができる合図のスプライトの高さ
			constexpr int kCanCatchEnemySpriteHeight[enCanCatchEnemySpriteNum] =
			{
				static_cast<int>(256 * 0.2f),
				static_cast<int>(512 * 0.2f),
				static_cast<int>(512 * 0.2f)
			};


			constexpr float kCanCatchTime1 = 1.5f;		//!< 敵を捕まえることができる合図のスプライトのタイム1
			constexpr float kCanCatchTime2 = 0.5f;		//!< 敵を捕まえることができる合図のスプライトのタイム2
			constexpr float kCanCatchMaxScale = 1.5f;	//!< 敵を捕まえることができる合図のスプライトの最大拡大率


			//!< ターゲットを示すスプライトのファイルパス
			constexpr const char* const kTargetSpriteFilePath = "Assets/Image/qte/target.DDS";
			//!< ターゲットを示すスプライトの幅
			constexpr int kTargetSpriteWidth = static_cast<int>(300 * 0.3f);
			//!< ターゲットを示すスプライトの高さ
			constexpr int kTargetSpriteHeight = static_cast<int>(400 * 0.3f);

			constexpr float kTargetSpriteMaxScale = 1.0f;	//!< ターゲットを示すスプライトの最大拡大率
			constexpr float kTargetSpriteMinScale = 0.3f;	//!< ターゲットを示すスプライトの最小拡大率
			constexpr float kTargetSpritePowPower = 3.0f;	//!< ターゲットを示すスプライトの拡大率の変化の指数


			//!< ターゲットとの距離を示すフォントレンダラーの座標のバッファ
			static const Vector2 kTargetLengthFontRenderPosBuff = { -30.0f, 10.0f };
		}

		/**
		 * @brief ミッションのUIの定数データ
		*/
		namespace nsMissionUIConstData
		{
			/**
			 * @brief ミッションの種類
			*/
			enum EnMissionType
			{
				enMT_carSilver,
				enMT_carRed,
				enMT_carBlue,
				enMT_clearTime,
				enMT_noCommandMiss,
				enMissionTypeNum
			};

			/**
			 * @brief ミッションのリザルトの種類
			*/
			enum EnMissionResultType
			{
				enMRT_clear,
				enMRT_great,
				enMRT_perfect,
				enMissionResultTypeNum
			};

			//!< ミッションのレベル2Dのファイルパス
			constexpr const char* const kMissionLevel2DFilePath = "Assets/level2DData/mission.casl";
			constexpr const char* const kMissionLevelObjName = "mission";	//!< ミッションのレベルオブジェクトネーム
			//!< チェックマークのレベルオブジェクトネーム
			constexpr const char* const kCheckMarkLevelObjNames[enMissionTypeNum] =
			{
				"mission_checkMark_carSilver",
				"mission_checkMark_carRed",
				"mission_checkMark_carBlue",
				"mission_checkMark_clearTime",
				"mission_checkMark_noCommandMiss"
			};

			//!< ミッションのスプライトのファイルパス
			constexpr const char* const kMissionSpriteFilePath = "Assets/Image/mission/mission.DDS";
			//!< チェックマークのスプライトのファイルパス
			constexpr const char* const kCheckMarkSpriteFilePath = "Assets/Image/mission/mission_checkMark.DDS";

			//!< ミッションリザルトの枠のスプライトのファイルパス
			constexpr const char* const kMissionResultFrameFilePath = "Assets/Image/mission/mission_result_frame.DDS";
			//!< ミッショリザルトの枠のスプライトの幅
			constexpr int kMissionResultFrameSpriteWidth = static_cast<int>(512.0f * 1.0f);
			//!< ミッショリザルトの枠のスプライトの高さ
			constexpr int kMissionResultFrameSpriteHeight = static_cast<int>(512.0f * 1.0f);
			//!< ミッションリザルトのテキストのレベルオブジェクトネーム
			constexpr const char* const kMissionResultTextSpriteFilepath[enMissionResultTypeNum] =
			{
				"Assets/Image/mission/mission_result_clear.DDS",
				"Assets/Image/mission/mission_result_great.DDS",
				"Assets/Image/mission/mission_result_perfect.DDS"
			};
			//!< ミッショリザルトのテキストのスプライトの幅
			constexpr int kMissionResultTextSpriteWidth = static_cast<int>(512.0f * 1.0f);
			//!< ミッショリザルトのテキストのスプライトの高さ
			constexpr int kMissionResultTextSpriteHeight = static_cast<int>(512.0f * 1.0f);
		}
	}
}