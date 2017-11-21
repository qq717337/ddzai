#include "stdafx.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_Split
{
	TEST_CLASS(UnitTest4)
	{
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			srand((unsigned)time(NULL));
			Logger::WriteMessage("Test initialized.\n"); //���������Ϣ  
		}
		TEST_METHOD_CLEANUP(TearDown)
		{
			Logger::WriteMessage("Test completed.\n");
		}
		TEST_METHOD(TestSplitStrategyBase)
		{
			for (int i = 0; i < 100; i++) {//Լ��0.8ms
			//CardSet set;
			//auto& extraCards = set.RandomFillLeft();
			//HandCards *cards = set.PlayerCardSet[0];
				//HandCards *cards = new HandCards(std::vector<uint8_t>{ 0x34, 0x44, 0x15, 0x45, 0x26, 0x46, 0x47, 0x18, 0x38, 0x49, 0x1a, 0x1b, 0x2c, 0x2d, 0x4d, 0x3d, 0x1d }, true);
				//auto s = cards->ToString();
				//Logger::WriteMessage(s.c_str());
				SplitStrategyBase strategy(std::make_shared<HandCards>(std::vector<uint8_t>{ 0x34, 0x44, 0x15, 0x45, 0x26, 0x46, 0x47, 0x18, 0x38, 0x49, 0x1a, 0x1b, 0x2c, 0x2d, 0x4d, 0x3d, 0x1d }, true));
				strategy.SplitIsolate();
				strategy.SplitBoom();
				strategy.SplitDoubleChain();
				strategy.SplitTripleChain();
				strategy.SplitSingleChain();
				strategy.SplitTriple();
				strategy.SplitIsolate();
				//int minStep = strategy._Split_Ptr()->MinStepCount();
				//auto t = strategy._Split_Ptr()->MinValueCardStyle().ToString();
			}
		}
		TEST_METHOD(TestSplitTwoSingleChain)
		{
			CardSet set;
			auto& extraCards = set.RandomFillLeft();
			std::shared_ptr< HandCards >cards = std::make_shared<HandCards>(set.PlayerCardSet[0]->Data());
			//HandCards *cards = new HandCards(std::vector<uint8_t>{0x33, 0x44, 0x15, 0x45, 0x26, 0x46,0x37, 0x47, 0x18, 0x38, 0x49, 0x1a, 0x1b, 0x2c, 0x2d, 0x4d, 0x3d, 0x1d }, true);
			auto s = cards->ToString();
			Logger::WriteMessage(s.c_str());
			SplitStrategyBase strategy(cards);

			strategy.SplitIsolate();
			strategy.SplitBoom();
			strategy.SplitIntersectChain();
			strategy.SplitDoubleChain();
			strategy.SplitTripleChain();
			strategy.SplitSingleChain();
			strategy.SplitTriple();
			strategy.SplitIsolate();
			SplitType type = *(strategy._Split_Ptr());
			s = type.ToString();

			Logger::WriteMessage(s.c_str());
			size_t minStep = type.MinStepCount();
			size_t cardCount = type.CardCount();
			auto t = strategy._Split_Ptr()->MinValueCardStyle().ToString();
		}
		TEST_METHOD(TestSplitStrategy)
		{
			CardSet set;
			auto& extraCards = set.RandomFillLeft();
			//HandCards *cards = set.PlayerCardSet[0];
			//auto s = cards->ToString();
			//Logger::WriteMessage(s.c_str());
			MinStepSplitStrategy strategy(std::make_shared<HandCards>(std::vector<uint8_t>{0x33, 0x44, 0x15, 0x45,
				0x26, 0x46, 0x37, 0x47, 0x18, 0x38, 0x49, 0x1a, 0x1b, 0x2c, 0x2d, 0x4d, 0x3d, 0x1d, 0x1, 0x2}, true));
			strategy.Split();
			auto&minStepSplitType = strategy.MinStepSplit();
		}
		TEST_METHOD(TestGameTable)
		{
			CardSet* opt = new CardSet();
			opt->PlayerCardSet[0] = new HandCards(CardVector{ 0x4b,0x2d,0x4d,0x2f });
			opt->PlayerCardSet[1] = new HandCards(CardVector{ 0x24,0x44,0x25,0x26,0x27,0x37,0x48,0x19,0x29,0x39,0x1f });
			opt->PlayerCardSet[2] = new HandCards(CardVector{ 0x13,0x23,0x33,0x43,0x14,0x16,0x46,0x17,0x18,0x28,0x1a,0x3a,0x4a,0x2b,0x3b });

			//auto& extraCards = set.RandomFillLeft();

			GameTable table(*opt);
			CardVector lastPlayCard({ 0x1e,0x2e,0x3e,0x4e });
			//CardStyle lastPlayStyle = CardStyle::DoubleChainStyle(CardIndex_3, CardIndex_5);
			CardStyle lastPlayStyle = CardStyle::FromCardsValue(lastPlayCard);
			EIdentity::EIdentity_ lastPlayIdentity = EIdentity::Lord;
			EIdentity::EIdentity_ curPlayIdentity = EIdentity::Farmer1;
			auto thisStyle = table.Take(curPlayIdentity, lastPlayIdentity, lastPlayStyle);
			auto cards = table.GetHandCard(curPlayIdentity)->GetCardsByStyle(thisStyle);

			OpenCVEntry *cv = new OpenCVEntry(L"D:\\CommondCode\\DDZ_AI\\DDZ_AI\\CardsImage");
			cv->ShowPlay(
				table.GetHandCard(EIdentity::Lord)->ToCardValues(),
				table.GetHandCard(EIdentity::Farmer1)->ToCardValues(),
				table.GetHandCard(EIdentity::Farmer2)->ToCardValues()
				, lastPlayIdentity, curPlayIdentity, lastPlayCard, cards);
			//cv->ShowCard(opt);
			cv->Wait(30000);
		}
		EIdentity::EIdentity_ NextPlayerIdentity(EIdentity::EIdentity_ identity) {
			switch (identity) {
			case EIdentity::Lord:
				return EIdentity::Farmer1;
			case EIdentity::Farmer1:
				return EIdentity::Farmer2;
			case EIdentity::Farmer2:
				return EIdentity::Lord;
			}
			return EIdentity::Lord;;
		}
		TEST_METHOD(TestNewGame)
		{
			OpenCVEntry *cv = new OpenCVEntry(L"D:\\CommondCode\\DDZ_AI\\DDZ_AI\\CardsImage");
			OptimizedCard *opt = new OptimizedCard();
			opt->Optimized(nullptr);
			std::string s = opt->ToString2();
			Logger::WriteMessage(s.c_str());
			opt->PlayerCardSet[0]->AddCards(opt->ExtraCard);
			opt->PlayerCardSet[0]->UpdateByFlag();
			opt->PlayerCardSet[1]->UpdateByFlag();
			opt->PlayerCardSet[2]->UpdateByFlag();
			//CardSet *opt=new CardSet();
			//opt->PlayerCardSet[0] = new HandCards(CardVector{ 0x34,0x15,0x35,0x45,0x36,0x47,0x38,0x49,0x2a,0x1b,0x4b,0x2d,0x4d,0x1e,0x2e,0x3e,0x4e,0x2f,0x1,0x2 });
			//opt->PlayerCardSet[1] = new HandCards(CardVector{ 0x24,0x44,0x25,0x26,0x27,0x37,0x48,0x19,0x29,0x39,0x1c,0x2c,0x3c,0x4c,0x1d,0x3d,0x1f });
			//opt->PlayerCardSet[2] = new HandCards(CardVector{ 0x13,0x23,0x33,0x43,0x14,0x16,0x46,0x17,0x18,0x28,0x1a,0x3a,0x4a,0x2b,0x3b,0x3f,0x4f });
			//
			//cv->ShowCard(opt);
			GameTable table(*opt);
			EIdentity::EIdentity_ lastPlayIdentity = EIdentity::Lord;
			EIdentity::EIdentity_ currentIdentity = EIdentity::Lord;
			CardVector lastPlayCardsValue;
			CardStyle lastPlayStyle = CardStyle::FromCardsValue(lastPlayCardsValue);

			while (true) {
				table.debug_step++;
				if (table.debug_step > 60) {
					Logger::WriteMessage("һ�㲻����������Ĵ�����������ѭ����");
					return;
				}
				auto l_handCards = table.GetHandCard(EIdentity::Lord);
				auto f1_handCards = table.GetHandCard(EIdentity::Farmer1);
				auto f2_handCards = table.GetHandCard(EIdentity::Farmer2);
				auto l_cardsValue = l_handCards->ToCardValues();
				auto f1_cardsValue = f1_handCards->ToCardValues();
				auto f2_cardsValue = f2_handCards->ToCardValues();

				std::ostringstream ostr_stepText;
				ostr_stepText << "step=" << table.debug_step;
				Log::InfoF(ostr_stepText.str());
				Log::InfoF("opt->PlayerCardSet[0] = new HandCards(CardVector" + l_handCards->ToString() + ");");
				Log::InfoF("opt->PlayerCardSet[1] = new HandCards(CardVector" + f1_handCards->ToString() + ");");
				Log::InfoF("opt->PlayerCardSet[2] = new HandCards(CardVector" + f2_handCards->ToString() + ");");

				for (int i = 0; i < 3; i++) {
					Logger::WriteMessage(table.GetHandCard(EIdentity::EIdentity_(i))->ToString().c_str());
					if (table.CardCount(EIdentity::EIdentity_(i)) == 0) {
						Logger::WriteMessage((IdentityNameTable[i] + "���ʤ��").c_str());
						goto win;
					}
				}
				auto currentPlayerHandCards = table.GetHandCard(currentIdentity);
				CardStyle r;
				if (currentIdentity == lastPlayIdentity) {
					r = table.Play(currentIdentity);
					Logger::WriteMessage(("Play:" + IdentityNameTable[lastPlayIdentity] + ":" + IdentityNameTable[currentIdentity]).c_str());
					Logger::WriteMessage(r.ToString().c_str());
				}
				else {
					r = table.Take(currentIdentity, lastPlayIdentity, lastPlayStyle);
					Logger::WriteMessage(("Take:" + IdentityNameTable[lastPlayIdentity] + ":" + IdentityNameTable[currentIdentity]).c_str());
					Logger::WriteMessage(r.ToString().c_str());
				}

				auto outCards = currentPlayerHandCards->GetCardsByStyle(r);
				currentPlayerHandCards->RemoveCard(r);
				currentPlayerHandCards->UpdateByFlag();
				std::ostringstream ostr_path;
				ostr_path << "D:\\CommondCode\\DDZ_AI\\DDZ_AI\\OutputImage\\" << table.debug_step << ".png";

				cv->WritePlay(
					ostr_path.str(),
					l_cardsValue,
					f1_cardsValue,
					f2_cardsValue,
					lastPlayIdentity, currentIdentity, lastPlayCardsValue, outCards,
					{ OpenCVEntry::TextInfo(CvPoint(600,25),CvScalar(255,255,255),1.0,ostr_stepText.str()) });

				if (r.Valid()) { //�������Ҫ������ô���������Ʒ��Ϊ�ҽӵģ�������ҳ��ģ�Ȼ���ٴ����ҳ�
					lastPlayIdentity = currentIdentity;
					lastPlayStyle = r;
					auto lastHandCard = table.GetHandCard(lastPlayIdentity);
					lastPlayCardsValue = outCards;
					//currentPlayerHandCards.SubCardStyle(r)
				}
				table.GetHandCard(EIdentity::Lord)->UpdateByFlag();
				table.GetHandCard(EIdentity::Farmer1)->UpdateByFlag();
				table.GetHandCard(EIdentity::Farmer2)->UpdateByFlag();
				currentIdentity = NextPlayerIdentity(currentIdentity);
			}
			win:
			Log::InfoF("/////////////////////////////////////////////////////////////////////////////////////\n");
		}
	};
}