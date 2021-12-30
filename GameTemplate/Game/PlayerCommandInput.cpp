#include "stdafx.h"
#include "PlayerCommandInput.h"
#include "Player.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		using namespace nsPlayerConstData::nsCommandInputConstData;

		/**
		 * @brief コンストラクタ
		*/
		CPlayerCommandInput::CPlayerCommandInput()
		{
			// 乱数を初期化する
			InitRand();

			// InitCommandを呼ぶまでは、終了しているようにする
			m_isEndCommandInput = true;

			return;
		}

		/**
		 * @brief コマンドを初期化する
		 * @param[in] comNum コマンドの数
		 * @param[in] comType コマンドタイプ
		*/
		void CPlayerCommandInput::InitCommand(
			const int comNum,
			const nsPlayer::nsPlayerConstData::nsCommandInputConstData::EnCommandType comType
		)
		{
			// コマンドの配列をリセット
			m_commandButtonTypeArray.clear();
			// コマンドの進行度をリセット
			m_commandProgress = 0;
			// コマンド入力が終了していないようにする
			m_isEndCommandInput = false;

			// コマンドの配列を、コマンドの数だけリサーブ。
			m_commandButtonTypeArray.reserve(comNum);

			// コマンドタイプの候補の配列
			std::vector<EnCommandType> comTypeCandidateArray;
			// コマンドの数だけリサーブ
			comTypeCandidateArray.reserve(comNum);
			// 指定されたコマンドタイプは絶対に入れる
			comTypeCandidateArray.emplace_back(comType);
			// 残りのコマンドの候補をランダムで決める
			for (int i = 0; i < comNum - 1; i++)
			{
				// 指定されたタイプ以下のコマンドタイプから、ランダムで選ぶ。
				const int randSelectType = (*m_randSelectType)(*m_mt) % (comType + 1);
				comTypeCandidateArray.emplace_back(static_cast<EnCommandType>(randSelectType));
			}

			for (int i = 0; i < comNum; i++)
			{
				// 乱数を候補の配列のサイズで割って、配列のランダムな要素番号を得る。
				const int randSelectType = (*m_randSelectType)(*m_mt) % comTypeCandidateArray.size();
				// 候補の中からタイプを取ってきて、そのタイプの乱数を出す。
				const int randByType = (*m_randByType[comTypeCandidateArray[randSelectType]])(*m_mt);
				
				// 一度使用した候補は、配列から消去する。
				comTypeCandidateArray.erase(comTypeCandidateArray.begin() + randSelectType);

				// コマンドの配列に登録
				m_commandButtonTypeArray.emplace_back(static_cast<EnQTEButtonType>(randByType));
			}

			return;
		}

		/**
		 * @brief コマンド入力を実行する
		*/
		void CPlayerCommandInput::Execute()
		{
			if (m_commandProgress >= m_commandButtonTypeArray.size() || m_isEndCommandInput == true)
			{
				// コマンド進行度が、コマンドの配列以上、まはた、
				// コマンド入力が終了していたら、何もしない。早期リターン。
				return;
			}

			if (m_commandButtonTypeArray[m_commandProgress] == m_playerRef->GetInputData().inputCommand)
			{
				// コマンドの配列と、コマンド入力情報が一致していたら、コマンド進行度を進める。
				m_commandProgress++;
			}

			if (m_commandProgress >= m_commandButtonTypeArray.size())
			{
				// コマンド進行度が、コマンドの配列以上になったら、コマンド入力を終了する。
				m_isEndCommandInput = true;
			}
			

			return;
		}

		/**
		 * @brief 乱数を初期化
		*/
		void CPlayerCommandInput::InitRand()
		{
			// 非決定的な乱数生成器でシード生成機を生成
			std::random_device rnd;
			// メルセンヌツイスターの32ビット版、引数は初期シード
			m_mt = std::make_unique<std::mt19937>(rnd());
			// [a, b] 範囲の一様乱数
			m_randByType[enCT_type1] = std::make_unique<std::uniform_int_distribution<>>(
				EnQTEButtonType::enQTE_A,
				EnQTEButtonType::enQTE_X
				);
			m_randByType[enCT_type2] = std::make_unique<std::uniform_int_distribution<>>(
				EnQTEButtonType::enQTE_L_Up,
				EnQTEButtonType::enQTE_L_Right
				);
			m_randByType[enCT_type3] = std::make_unique<std::uniform_int_distribution<>>(
				EnQTEButtonType::enQTE_L1,
				EnQTEButtonType::enQTE_R2
				);

			m_randSelectType = std::make_unique<std::uniform_int_distribution<>>(0, 99);

			//const int randTargetNum = (*m_rand)(*m_mt);

			return;
		}
	}
}