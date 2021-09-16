#pragma once
#include "ModelRenderConstData.h"

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
			/**
			 * @brief モデル描画クラス
			*/
			class CModelRender : public IGameObject
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CModelRender() = default;

				/**
				 * @brief デストラクタ
				*/
				~CModelRender() = default;

			public:		// オーバーライドしたメンバ関数
				/**
				 * @brief スタート関数
				 * @return アップデートを行うか？
				*/
				bool Start() override final { return true; };

				/**
				 * @brief 破棄されたときの処理
				*/
				void OnDestroy() override final;

				/**
				 * @brief 常に呼ばれるアップデート関数
				*/
				void AlwaysUpdate() override final;

				/**
				 * @brief 描画処理関数
				 * @param[in] rc レンダーコンテキスト
				*/
				void Render(RenderContext& rc) override final;

			public:		// メンバ関数

				/**
				 * @brief 初期化関数
				 * @param[in] filePath モデルのファイルパス
				 * @param[in] modelUpAxis モデルのUP軸
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClip アニメーションクリップの数
				*/
				void Init(
					const char* filePath,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					AnimationClip* animationClips = nullptr,
					int numAnimationClip = 0
					);

				/**
				 * @brief 座標を設定する
				 * @param[in] position 座標
				*/
				void SetPosition(const Vector3& pos)
				{
					m_position = pos;
				}

				/**
				 * @brief 座標を得る
				 * @return[in] 座標
				*/
				const Vector3& GetPosition() const
				{
					return m_position;
				}

				/**
				 * @brief 回転を設定する
				 * @param[in] rot 回転を得る
				*/
				void SetRotatioin(const Quaternion& rot)
				{
					m_rotation = rot;
				}

				/**
				 * @brief 回転を得る
				 * @return 回転
				*/
				const Quaternion& GetRotation() const
				{
					return m_rotation;
				}

				/**
				 * @brief 拡大率を設定する
				 * @param[in] scale 拡大率
				*/
				void SetScale(const Vector3 scale)
				{
					m_scale = scale;
				}

				/**
				 * @brief 拡大率を得る
				 * @return 拡大率
				*/
				const Vector3& GetScale() const
				{
					return m_scale;
				}

				void PlayAnimation(
					int animNo,
					float interpolateTime = nsModelConstData::kAnimationInterpolateTime
				)
				{
					m_animationPtr->Play(animNo, interpolateTime);
					return;
				}


			private:	// privateなメンバ関数

				/**
				 * @brief 初期化処理のメインコア
				 * @param[in] modelInitData モデルの初期化データ
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClips アニメーションクリップの数
				*/
				void InitMainCore(
					ModelInitData& modelInitData,
					AnimationClip* animationClips,
					const int numAnimationClips
				);

				/**
				 * @brief スケルトンの初期化
				*/
				void InitSkelton();

				/**
				 * @brief アニメーションの初期化
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClips アニメーションクリップの数
				*/
				void InitAnimation(AnimationClip* animationClips, const int numAnimationClips);

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
				void SetCommonModelInitData(
					ModelInitData* modelInitData,
					const char* tkmFilePath,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					const char* fxFilePath = nsModelConstData::kDefaultFxFilePath,
					const char* vsEntryPointFunc = nsModelConstData::kVsEntryPointFunc,
					const char* vsSkinEntryPointFunc = nsModelConstData::kVsSkinEntryPointFunc,
					const char* psEntryPointFunc = nsModelConstData::kPsEntryPointFunc
				);

			private:	// データメンバ
				Model m_model;								//!< モデルクラス
				const char* m_tkmFilePath = nullptr;		//!< tkmファイルのファイルパス
				std::unique_ptr<Skeleton> m_skeletonPtr;	//!< スケルトンクラス
				std::unique_ptr<Animation> m_animationPtr;	//!< アニメーションクラス

				Vector3 m_position = Vector3::Zero;				//!< 座標
				Quaternion m_rotation = Quaternion::Identity;	//!< 回転
				Vector3 m_scale = Vector3::One;					//!< 拡大率

				bool m_isInited = false;					//!< 初期化済みか？


			};

		}
	}
}