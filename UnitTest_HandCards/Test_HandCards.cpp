#include "stdafx.h"
#include "CppUnitTest.h"

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
			HandCards newHandCards(std::vector<uint8_t>{ 0x13, 0x23, 0x33, 0x25, 0x1, 0x2 }, true);
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
			CardVector a({ 0x28,0x16,0x18,0x36,0x1e,0x38,0x19,0x29,0x49,0x2e });

			CardStyle s = CardStyle::FromCardsValue(a);
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
			CardStyle s0 = CardStyle::FromCardsValue(x);
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
			set.DealChain(1, CardIndex_3, CardIndex_J, 1);
			set.DealChain(1, CardIndex_10, CardIndex_A, 2);
			//set.DealChain(2, CardIndex_5, CardIndex_J, 1);
			set.Update();
			auto p1 = set.PlayerCardSet[1];
			auto avChain = p1->AvailableSingleChainRange();
			auto deskStr = set.ToString();
			auto r = std::vector<CardRange>{ CardRange(CardIndex_3,CardIndex_A) };//33456,910JQK,10JQKA
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
			auto superior = new SuperiorDealStrategy(opt, 2);
			opt->Optimized(superior, 1);
			std::string s = opt->ToString2();
			Logger::WriteMessage(s.c_str());
			OpenCVEntry *cv = new OpenCVEntry(L"D:\\CommondCode\\DDZ_AI\\DDZ_AI\\CardsImage");
			//cv->ShowPlay(opt, 2,1, { 0x01,0x02 }, {0x23,0x33,0x43});
			cv->ShowCard(opt, { OpenCVEntry::TextInfo(CvPoint(550,35),CvScalar(255,255,255),1.0,"Optimized Card") });
			cv->Wait(30000);
			//const std::string name = "D:\\UnityProject\\DDZCardImage\\Card.exe " + s;
			//system(name.c_str());
		}

		TEST_METHOD(TestSmoothCard)
		{
			SmoothCard *opt = new SmoothCard();
			opt->Optimized(nullptr, 3, 3);
			std::string s = opt->ToString2();
			Logger::WriteMessage(s.c_str());
			const std::string name = "D:\\UnityProject\\DDZCardImage\\Card.exe " + s;
			system(name.c_str());
		}

		TEST_METHOD(TestSplitType)
		{
			SplitType splitType;
			splitType.AddBoom(CardIndex_Q);
			CardStyle styleRef;
			if (splitType.GetLastShotCardStyle(&styleRef)) {
				auto s = styleRef.ToString();
			}

			splitType.AddDoubleChain(CardIndex_4, CardIndex_7);

			auto s = splitType.GetDoubleChainStyle().ToString();
			SplitMemnto* men = new SplitMemnto(splitType);
			Recorder<SplitType>::Push(men);
			auto menCopy = new SplitType();
			Recorder<SplitType>::Pop(menCopy);
		}

		TEST_METHOD(TestRecorder)
		{
			HandCards newHandCards(std::vector<uint8_t>{ 0x13, 0x23, 0x34, 0x35, 0x16, 0x27, 0x1f, 0x2 }, false);

			auto avChain3 = newHandCards.IsolateCards();
			HandCardsMemnto* men = new HandCardsMemnto(newHandCards);
			Recorder<HandCards>::Push(men);
			auto menCopy = new HandCards();
			Recorder<HandCards>::Pop(menCopy);

			men = new HandCardsMemnto(newHandCards);//前面已经Pop之后删除了new 的Memnto对象，所以这里要重新New
			Recorder<HandCards>::Set("liu", men);
			auto menGet = new HandCards();
			bool isGet = Recorder<HandCards>::Get("liu", menGet);
			Recorder<HandCards>::Remove("liu");
		}

		TEST_METHOD(TestTips)
		{
			HandCards v(CardVector{ 0x13,0x24,0x34,0x15,0x25,0x35,0x36,0x17,0x18,0x1f,0x2f });
			auto tips = v.FindAvailableTake(CardStyle::DoubleStyle(CardIndex_3), true);
			auto tips2 = v.MinValueStyle();
			Assert::AreEqual(tips.empty(), false);
		}
	};
}