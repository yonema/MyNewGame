#pragma once
#include "ModelRenderConstData.h"
#include "Render.h"

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
			private:	// エイリアス宣言
				using ModelPtr = std::unique_ptr<Model>;		//!< モデルクラスのユニークポインタ
				using SkeletonPtr = std::unique_ptr<Skeleton>;	//!< スケルトンクラスのユニークポインタ
				using AnimPtr = std::unique_ptr<Animation>;		//!< アニメーションクラスにユニークポインタ

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
				 * @brief 描画オブジェクト登録の入口
				*/
				void AddRenderEntrance() override final;

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
				 * @brief 半透明描画用の初期化関数
				 * @param[in] filePath モデルのファイルパス
				 * @param[in] modelUpAxis モデルのUP軸
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClip アニメーションクリップの数
				*/
				void IniTranslucent(
					const char* filePath,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					AnimationClip* animationClips = nullptr,
					int numAnimationClip = 0
				);

				/**
				 * @brief フォワードレンダリング用の初期化関数。特殊なシェーディングを行いたいとき用。
				 * @param[in] modelInitData モデルの初期化データ
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClip アニメーションクリップの数
				*/
				void InitForwardRendering(
					ModelInitData& modelInitData,
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

				/**
				 * @brief アニメーションを再生する
				 * @param[in] animNo アニメーションクリップの番号
				 * @param[in] interpolateTime 補完時間（単位：秒）
				*/
				void PlayAnimation(
					const int animNo,
					const float interpolateTime = nsModelConstData::kAnimationInterpolateTime
				)
				{
					m_animationPtr->Play(animNo, interpolateTime);
					return;
				}

				/**
				 * @brief 自己発光カラーを設定
				 * @param[in] emmisonColor 自己発光カラー
				*/
				void SetEmmisonColor(const Vector4& emmisonColor)
				{
					m_model->SetEmmisonColor(emmisonColor);
				}

				/**
				 * @brief 乗算カラーを設定
				 * @param[in] mulColor 乗算カラー
				*/
				void SetMulColor(const Vector4& mulColor)
				{
					m_model->SetMulColor(mulColor);
				}

				/**
				 * @brief モデルのアルファ値を設定
				 * @param[in] alphaValue アルファ値
				*/
				void SetAlphaValue(const float alphaValue)
				{
					m_model->SetAlphaValue(alphaValue);
				}


			private:	// privateなメンバ関数

				/**
				 * @brief 初期化処理のメインコア
				 * @param[in] modelInitData モデルの初期化データ
				 * @param[in] animationClips アニメーションクリップ
				 * @param[in] numAnimationClips アニメーションクリップの数
				 * @param[in] isDefferdRender ディファードレンダリングで描画するか？
				*/
				void InitMainCore(
					ModelInitData& modelInitData,
					AnimationClip* animationClips,
					const int numAnimationClips,
					const bool isDefferdRender = true
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

				/**
				 * @brief デフォルトの定数バッファをセット
				 * @param modelInitData モデルの初期化データ
				*/
				void SetDefaultConstantBuffer(ModelInitData* modelInitData);

				/**
				 * @brief レンダラーを初期化する
				 * @param[in] isDefferdRender ディファードレンダリングで描画するか？
				*/
				void InitRender(const bool isDefferdRender);

				/**
				 * @brief モデルを描画する
				 * @param[in] rc レンダリングコンテキスト
				*/
				void ModelRender(RenderContext& rc);

			private:	// データメンバ
				ModelPtr m_model;							//!< モデルクラス
				CRender m_render;							//!< レンダラークラス
				SkeletonPtr m_skeletonPtr;					//!< スケルトンクラス
				AnimPtr m_animationPtr;						//!< アニメーションクラス
				const char* m_tkmFilePath = nullptr;		//!< tkmファイルのファイルパス

				Vector3 m_position = Vector3::Zero;				//!< 座標
				Quaternion m_rotation = Quaternion::Identity;	//!< 回転
				Vector3 m_scale = Vector3::One;					//!< 拡大率

				bool m_isInited = false;					//!< 初期化済みか？


			};

		}
	}
}