#include "stdafx.h"
#include "CppUnitTest.h"
#include "../DDZ_AI/CardSet.h"
#include "../DDZ_AI/HandCardsFlag.h"
#include "../DDZ_AI/HandCards.h"
#include "../DDZ_AI/OptimizedCard.h"
#include "../DDZ_AI/CardStyle.h"
#include"../DDZ_AI/DecorateDealStrategy.h"
#include"../DDZ_AI/SplitType.h"
#include"../DDZ_AI/Recorder.h"

#include <string>
#include <time.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_HandCards
{
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			srand((unsigned)time(NULL));
			Logger::WriteMessage("Test initialized.\n"); //用于输出信息  
		}
		TEST_METHOD_CLEANUP(TearDown)
		{
			Logger::WriteMessage("Test completed.\n");
		}
		TEST_METHOD(TestCardValueToIndex)
		{
			uint8_t index = HandCardsFlag::CardValueToIndex(0x14);
			Assert::AreEqual(index, uint8_t(1));
		}
		TEST_METHOD(TestFlagString)
		{
			HandCards newHandCards(std::vector<uint8_t>{ 0x13, 0x23, 0x33, 0x25, 0x1, 0x2 }, false);
			Assert::AreEqual(newHandCards.Count(CardIndex_3), 3);
			bool hasCard = newHandCards.HasCard(0x13);
			Assert::AreEqual(hasCard, true);
			uint8_t small = newHandCards.GetFlag(CardIndex_SmallJoker, 0);
			Assert::AreEqual(small, uint8_t(1));
			Assert::AreEqual(newHandCards.GetFlag(CardIndex_LargeJoker, 0), uint8_t(1));
			Assert::AreEqual(newHandCards.GetFlag(CardIndex_3, 0), uint8_t(1));
			std::string s = newHandCards.FlagString();
			Assert::AreEqual(newHandCards.GetFlag(CardIndex_4, 0), uint8_t(0));
		}
		TEST_METHOD(TestUpdateFlag)
		{
			HandCards newHandCards(std::vector<uint8_t>{ 0x13, 0x23, 0x14, 0x15, 0x16, 0x27, 0x1f, 0x2 }, false);

			std::string s = newHandCards.FlagString();
			newHandCards.RemoveChain(CardIndex_3, CardIndex_7, 1);
			s = newHandCards.FlagString();
			s = newHandCards.ToString();
			newHandCards.UpdateByFlag();
			Assert::AreEqual(newHandCards.Size(), size_t(3));
		}
		TEST_METHOD(TestCardStyle)
		{
			uint8_t a[] = { 0x13,0x23,0x33,0x14,0x24,0x34 };
			std::vector<uint8_t> x(a, a + 6);
			CardStyle s = CardStyle::GetCardStyleByCardsValue(x);
			Assert::AreEqual(s.StartValue, uint8_t(0));
			Assert::AreEqual(s.EndValue, uint8_t(1));
			Assert::AreEqual(s.Style, int(ECardStyle::Triple_Chain_Zero));

			CardStyle sb(ECardStyle::Triple_Chain_Zero, uint8_t(4), uint8_t(5));
			int compareRet = sb.Compare(s);
			Assert::AreEqual(compareRet, 1);


			CardStyle se(ECardStyle::Triple_Chain_One, uint8_t(4), uint8_t(5));
			compareRet = se.Compare(s);
			Assert::AreEqual(compareRet, 0);
		}

		TEST_METHOD(TestCardStyleCardCount)
		{
			uint8_t a[] = { 0x13,0x23,0x33,0x14,0x24,0x34 };
			std::vector<uint8_t> x(a, a + 6);
			CardStyle s0 = CardStyle::GetCardStyleByCardsValue(x);
			Assert::AreEqual(s0.GetCardsCount(), 6);

			CardStyle s1(ECardStyle::Triple_Chain_One, uint8_t(4), uint8_t(6));
			Assert::AreEqual(s1.GetCardsCount(), 12);


			CardStyle s2(ECardStyle::Triple_Chain_Two, uint8_t(4), uint8_t(5));
			Assert::AreEqual(s2.GetCardsCount(), 10);
		}

		TEST_METHOD(TestDealCard)
		{
			CardSet set;
			set.DealCard(0, 0x29);
			set.DealIndex(1, CardIndex_3, 3);
			//set.DealIndex(1, CardIndex_SmallJoker, 1);
			set.DealIndex(1, CardIndex_LargeJoker, 1);
			set.DealChain(2, CardIndex_5, CardIndex_J, 1);
			auto p0 = set.PlayerCardSet[0];
			auto p1 = set.PlayerCardSet[1];
			auto p2 = set.PlayerCardSet[2];
			set.Update();

			auto desk = set.ToString();

			Assert::AreEqual(set.LeftCount(2), size_t(10));

			auto& extraCards = set.RandomFillLeft();
			for (int i = 0; i < 3; ++i) {
				int leftCount = set.LeftCount(i);
				Assert::AreEqual(leftCount, 0);
			}
			Assert::AreEqual(extraCards.size(), size_t(3));

			auto str = set.ToString();

			Logger::WriteMessage(str.c_str()); //用于输出信息 
		}
		TEST_METHOD(TestAvailableChain)
		{
			CardSet set;
			set.DealCard(0, 0x29);
			set.DealIndex(1, CardIndex_8, 4);
			set.DealIndex(1, CardIndex_4, 3);
			//set.DealIndex(1, CardIndex_SmallJoker, 1);
			set.DealIndex(1, CardIndex_LargeJoker, 1);
			//set.DealChain(2, CardIndex_5, CardIndex_J, 1);
			auto avChain = set.DeskCardSet.AvailableSingleChain();
			auto deskStr = set.ToString();
			auto r = std::vector<uint8_t>{ 0,6,7 };//33456,910JQK,10JQKA
			Assert::AreEqual(avChain == r, true);
		}
		TEST_METHOD(TestHandCardsIsolate)
		{
			HandCards newHandCards(std::vector<uint8_t>{ 0x13, 0x23, 0x34, 0x35, 0x16, 0x27, 0x1f, 0x2 }, false);

			auto avChain3 = newHandCards.IsolateCards();
		}
		TEST_METHOD(TestOptimizedCard)
		{
			OptimizedCard *opt = new OptimizedCard();
			auto multiBoom = new MultiBoomDealStrategy(opt, 5);
			opt->Optimized(multiBoom, 3);
			std::string s = opt->ToString2();
			Logger::WriteMessage(s.c_str());
			const std::string name = "D:\\UnityProject\\DDZCardImage\\Card.exe " + s;
			system(name.c_str());
			delete multiBoom;
			//delete opt;
		}
		TEST_METHOD(TestSplitType)
		{
			SplitType splitType;
			splitType.AddBoom(CardIndex_Q);
			CardStyle styleRef;
			if (splitType.GetLastShotCardStyles(&styleRef)) {
				auto s = styleRef.ToString();
			}

			splitType.AddDoubleChain(CardIndex_4, CardIndex_7);

			auto s = splitType.GetDoubleChainStyle().ToString();
			SplitMemnto* men = new SplitMemnto(splitType);
			Recorder<SplitType>::R.Push(men);
			auto menCopy = new SplitType();
			Recorder<SplitType>::R.Pop(menCopy);
		}

		TEST_METHOD(TestRecorder)
		{
			HandCards newHandCards(std::vector<uint8_t>{ 0x13, 0x23, 0x34, 0x35, 0x16, 0x27, 0x1f, 0x2 }, false);

			auto avChain3 = newHandCards.IsolateCards();
			HandCardsMemnto* men = new HandCardsMemnto(newHandCards);
			Recorder<HandCards>::R.Push(men);
			auto menCopy = new HandCards();
			Recorder<HandCards>::R.Pop(menCopy);

			men=new HandCardsMemnto(newHandCards);
			Recorder<HandCards>::R.Set("liu",men);
			auto menGet = new HandCards();

			bool isGet=Recorder<HandCards>::R.Get("liu", menGet);
			Recorder<HandCards>::R.Remove("liu");
		}
	};
}