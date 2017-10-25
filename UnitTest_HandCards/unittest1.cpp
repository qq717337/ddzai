#include "stdafx.h"
#include "CppUnitTest.h"
#include "../DDZ_AI/CardSet.h"
#include "../DDZ_AI/HandCardsFlag.h"
#include "../DDZ_AI/HandCards.h"
#include "../DDZ_AI/CardStyle.h"
#include <string>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_HandCards
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestCardValueToIndex)
		{
			uint8_t index = HandCardsFlag::CardValueToIndex(0x14);
			Assert::AreEqual(index, uint8_t(1));
		}
		TEST_METHOD(TestFlagString)
		{
			HandCards newHandCards({ 0x13,0x23,0x33,0x25,0x1,0x2 }, false);
			bool hasCard = newHandCards.HasCard(0x13);
			std::string s = newHandCards.FlagString();
			Assert::AreEqual(hasCard, true);
			uint8_t small = newHandCards.GetFlag(0, CardIndex_SmallJoker);
			Assert::AreEqual(small, uint8_t(1));
			Assert::AreEqual(newHandCards.GetFlag(0, CardIndex_LargeJoker), uint8_t(1));
			Assert::AreEqual(newHandCards.GetFlag(0, CardIndex_3), uint8_t(1));
			Assert::AreEqual(newHandCards.GetFlag(0, CardIndex_4), uint8_t(0));
		}
		TEST_METHOD(TestUpdateFlag)
		{
			HandCards newHandCards({ 0x13,0x23,0x16,0x27,0x1f,0x2 }, false);
			// newHandCards.RemoveIndex(CardIndex_3,3);

			std::string s = newHandCards.FlagString();
			//newHandCards.RemoveChain(CardIndex_3,CardIndex_7, 1);
			s = newHandCards.FlagString();
			s = newHandCards.CardSetString();
			newHandCards.UpdateByFlag();
			Assert::AreEqual(newHandCards.Size(), size_t(6));
		}
		TEST_METHOD(TestCardStyle)
		{
			uint8_t a[] = { 0x13,0x23,0x33,0x14,0x24,0x34 };
			std::vector<uint8_t> x(a, a + 6);
			CardStyle s = CardStyle::GetCardsStyleByCardsValue(x);
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
			CardStyle s0 = CardStyle::GetCardsStyleByCardsValue(x);
			Assert::AreEqual(s0.GetCardsCount(), 6);

			CardStyle s1(ECardStyle::Triple_Chain_One, uint8_t(4), uint8_t(6));
			Assert::AreEqual(s1.GetCardsCount(), 12);


			CardStyle s2(ECardStyle::Triple_Chain_Two, uint8_t(4), uint8_t(5));
			Assert::AreEqual(s2.GetCardsCount(), 10);
		}
	};
}