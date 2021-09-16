#include "stdafx.h"
#include "ModelRender.h"

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief モデル関連のネームスペース
		*/
		namespace nsModel
		{
			// モデルの定数データを使用可能にする
			using namespace nsModelConstData;

			/**
			 * @brief 破棄されたときの処理
			*/
			void CModelRender::OnDestroy()
			{
				return;
			}

			/**
			 * @brief 常に呼ばれるアップデート関数
			*/
			void CModelRender::AlwaysUpdate()
			{
				// 初期化済みか？
				if (m_isInited != true)
					return;	// 初期化していない、早期リターン

				// スケルトンが初期化されているか？
				if (m_skeletonPtr)
					// スケルトンが初期化されていたら、スケルトンを更新。
					m_skeletonPtr->Update(m_model.GetWorldMatrix());
				// アニメーションが初期化されているか？
				if (m_animationPtr)	// アニメーションが初期化されていたら、アニメーションを進める。
					m_animationPtr->Progress(1.0f / 60.0f);

				// モデルの座標更新
				m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

				return;
			}

			/**
			 * @brief 描画処理関数
			 * @param rc レンダーコンテキスト
			*/
			void CModelRender::Render(RenderContext& rc)
			{
				// 初期化済みか？
				if (m_isInited != true)
					return;	// 初期化していない、早期リターン

				// モデルを描画
				m_model.Draw(rc);

				return;
			}


			/**
			 * @brief 初期化関数
			 * @param filePath モデルのファイルパス
			 * @param modelUpAxis モデルのUP軸
			 * @param animationClips アニメーションクリップ
			 * @param numAnimationClip アニメーションクリップの数
			*/
			void CModelRender::Init(
				const char* filePath,
				const EnModelUpAxis modelUpAxis,
				AnimationClip* animationClips,
				int numAnimationClip
			)
			{
				// モデルの初期化データ
				ModelInitData modelInitData;

				// モデルの初期化データの共通部分の設定
				SetCommonModelInitData(&modelInitData, filePath);

				// 初期化処理のメインコア
				InitMainCore(modelInitData, animationClips, numAnimationClip);

				return;
			}

			/**
			 * @brief 初期化処理のメインコア
			 * @param modelInitData モデルの初期化データ
			 * @param animationClips アニメーションクリップ
			 * @param numAnimationClips アニメーションクリップの数
			*/
			void CModelRender::InitMainCore(
				ModelInitData& modelInitData,
				AnimationClip* animationClips,
				const int numAnimationClips
			)
			{
				// tkmファイルパスを保持する
				m_tkmFilePath = modelInitData.m_tkmFilePath;

				// スケルトンの初期化
				InitSkelton();
				// スケルトンが存在するか？
				if (m_skeletonPtr)
					// 存在する。スケルトンの設定
					modelInitData.m_skeleton = m_skeletonPtr.get();

				// モデルの初期化
				m_model.Init(modelInitData);

				// アニメーションの初期化
				InitAnimation(animationClips, numAnimationClips);

				// 初期化完了
				m_isInited = true;

				return;
			}


			/**
			 * @brief スケルトンの初期化
			*/
			void CModelRender::InitSkelton()
			{
				// tkmファイルをtksファイルに変換する
				std::string skeletonFilePath = m_tkmFilePath;
				// ファイルパスの文字列から、拡張子の場所を探す
				int pos = (int)skeletonFilePath.find(kTkmFileExtensinon);
				// tkmのファイルパスからtksのファイルパスに入れ替える
				skeletonFilePath.replace(pos, kFileExtensionLength, kTksFileExtensinon);

				// スケルトンのリソースの確保
				m_skeletonPtr.reset(new Skeleton);
				// スケルトンのデータを読み込み。
				bool isInited = m_skeletonPtr->Init(skeletonFilePath.c_str());

				// 初期化に失敗したか？
				if (isInited != true)
				{
					// 失敗したらスケルトンのリソースを開放する
					m_skeletonPtr.reset();
				}

				return;
			}

			/**
			 * @brief アニメーションの初期化
			 * @param animationClips アニメーションクリップ
			 * @param numAnimationClips アニメーションクリップの数
			*/
			void CModelRender::InitAnimation(AnimationClip* animationClips, const int numAnimationClips)
			{
				// アニメーションクリップが登録されているか
				if (animationClips == nullptr)
					return;	// されていない場合は何もしない

				// アニメーションのリソースの確保
				m_animationPtr.reset(new Animation);
				// アニメーションを初期化。
				m_animationPtr->Init(
					*m_skeletonPtr,		// アニメーションを流し込むスケルトン。
					animationClips,		// アニメーションクリップ。
					numAnimationClips	// アニメーションの数。
				);

				return;
			}

			/**
			 * @brief モデルの初期化データの共通部分の設定
			 * @param[out] modelInitData モデルの初期化データ
			 * @param[in] tkmFilePath モデルのtkmファイルパス
			 * @param[in] fxFilePath シェーダーのfxファイルパス
			 * @param[in] modelUpAxis モデルのUP軸
			 * @param[in] vsEntryPointFunc 頂点シェーダーのエントリーポイント
			 * @param[in] vsSkinEntryPointFunc スキンありの頂点シェーダーのエントリーポイント
			 * @param[in] psEntryPointFunc ピクセルシェーダーのエントリーポイント
			*/
			void CModelRender::SetCommonModelInitData(
				ModelInitData* modelInitData,
				const char* tkmFilePath,
				const char* fxFilePath,
				const EnModelUpAxis modelUpAxis,
				const char* vsEntryPointFunc,
				const char* vsSkinEntryPointFunc,
				const char* psEntryPointFunc
			)
			{
				// モデルのファイルパスの設定
				modelInitData->m_tkmFilePath = tkmFilePath;

				// モデルのUP軸の設定
				modelInitData->m_modelUpAxis = modelUpAxis;

				// シェーダーのファイルパスの設定
				modelInitData->m_fxFilePath = fxFilePath;
				// 頂点シェーダーのエントリーポイントの設定
				modelInitData->m_vsEntryPointFunc = vsEntryPointFunc;
				// スキンありの頂点シェーダーのエントリーポイントの設定
				modelInitData->m_vsSkinEntryPointFunc = vsSkinEntryPointFunc;
				// ピクセルシェーダーのエントリーポイントの設定
				modelInitData->m_psEntryPointFunc = psEntryPointFunc;

				return;

			}


		}
	}
}