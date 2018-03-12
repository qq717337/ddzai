#include "stdafx.h"
#include "GameTable.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"
#include "rapidjson/filereadstream.h"
#include<thread>

GameTable::GameTable(const CardSet& cardSet) :debug_step(0)
{
	LordPlayStrategy* lordStrategy = new LordPlayStrategy(cardSet.PlayerCardSet[0]->Data(), this);
	Farmer1PlayerStrategy*farmer1Strategy = new Farmer1PlayerStrategy(cardSet.PlayerCardSet[1]->Data(), this);
	Farmer2PlayerStrategy*farmer2Strategy = new Farmer2PlayerStrategy(cardSet.PlayerCardSet[2]->Data(), this);

	lordStrategy->Init(); farmer1Strategy->Init(); farmer2Strategy->Init();
	m_playerStrategy[0].reset(lordStrategy);
	m_playerStrategy[1].reset(farmer1Strategy);
	m_playerStrategy[2].reset(farmer2Strategy);
}


GameTable::~GameTable()
{
}

CardStyle GameTable::Play(EIdentity::EIdentity_ identity)
{
	m_curIdentity = identity;
	m_lastIdentity = identity;
	auto playStyle = m_playerStrategy[identity]->Play();
	return playStyle;
}

CardStyle GameTable::Take(EIdentity::EIdentity_ identity, EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	m_curIdentity = identity;
	m_lastIdentity = lastIdentity;
	m_lastCardStyle = lastStyle;
	//如果是王炸的话，则在此处就要不起
	if (m_lastCardStyle == CardStyle::JokerBoom) {
		return CardStyle::Invalid;
	}
	auto playStyle = m_playerStrategy[identity]->Take(lastIdentity, lastStyle);
	return playStyle;
}

size_t GameTable::CardCount(EIdentity::EIdentity_ identity)const
{
	auto handCards = m_playerStrategy[identity]->GetHandCards();
	return handCards->Size();
}

bool GameTable::IsStyleOtherCanNotTake(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)const
{
	if (lastIdentity == EIdentity::Lord) {
		return !GetHandCard(EIdentity::Farmer1)->CanTake(lastStyle) && !GetHandCard(EIdentity::Farmer2)->CanTake(lastStyle);
	}
	else {
		return !GetHandCard(EIdentity::Lord)->CanTake(lastStyle);
	}
	return false;
}

const HandCards* GameTable::GetHandCard(EIdentity::EIdentity_ identity) const
{
	return m_playerStrategy[identity]->GetHandCards();
}
HandCards* GameTable::GetHandCard(EIdentity::EIdentity_ identity)
{
	return const_cast<HandCards*>(m_playerStrategy[identity]->GetHandCards());
}
uint8_t GameTable::BiggestCardValue(EIdentity::EIdentity_ identity, int count)
{
	if (identity == EIdentity::Lord) {
		int max = 0;
		auto f1_cards = GetHandCard(EIdentity::Farmer1);
		for (int i = CardIndex_LargeJoker; i >= CardIndex_3; --i) {
			auto i_count = f1_cards->Count(i);
			if (i_count >= count) {
				max = i;
				break;
			}
		}
		auto f2_cards = GetHandCard(EIdentity::Farmer2);
		for (int i = CardIndex_LargeJoker; i >= CardIndex_3; --i) {
			auto i_count = f2_cards->Count(i);
			if (i_count >= count && i >= max) {
				return i;
			}
		}
		return max;
	}
	else {
		auto lord_cards = GetHandCard(EIdentity::Lord);
		for (int i = CardIndex_LargeJoker; i >= CardIndex_3; --i) {
			auto i_count = lord_cards->Count(i);
			if (i_count >= count) {
				return i;
			}
		}
	}
	return 0;
}
const PlayStrategyBase* GameTable::GetPlayStrategy(EIdentity::EIdentity_ identity) const
{
	return m_playerStrategy[identity].get();
}

const bool GameTable::ShouldAskLord(const SplitType & splitInfo, const HandCards & cards)
{
	if (splitInfo.GetBoom().size() >= 2) {
		return true;
	}
	if (splitInfo.MinStepCount() <= 6) {
		if (cards.Count(CardIndex_SmallJoker) * 4 + cards.Count(CardIndex_LargeJoker) * 5 + cards.Count(CardIndex_2) * 3 + cards.Count(CardIndex_A) * 2 > 10)
			return true;
	}
	return false;
}

void GameTable::SaveTable()
{
	using namespace rapidjson;
	Document document;
	document.SetObject();
	Document::AllocatorType& allocator = document.GetAllocator();
	Value f1(kArrayType);
	for (auto & v : m_playerStrategy[1]->GetHandCards()->ToCardValues())
		f1.PushBack(v, allocator);
	document.AddMember("f1", f1, allocator);

	Value f2(kArrayType);
	for (auto & v : m_playerStrategy[2]->GetHandCards()->ToCardValues())
		f2.PushBack(v, allocator);
	document.AddMember("f2", f2, allocator);

	Value l(kArrayType);
	for (auto & v : m_playerStrategy[0]->GetHandCards()->ToCardValues())
		l.PushBack(v, allocator);
	document.AddMember("l", l, allocator);

	document.AddMember("Last_Style", m_lastCardStyle.Style, allocator);
	document.AddMember("Last_Start", m_lastCardStyle.StartValue, allocator);
	document.AddMember("Last_End", m_lastCardStyle.EndValue, allocator);

	Value extra(kArrayType);
	for (auto & v : m_lastCardStyle.Extra)
		extra.PushBack(v, allocator);
	document.AddMember("Last_Extra", extra, allocator);

	document.AddMember("Last_Identity", m_lastIdentity, allocator);
	document.AddMember("Cur_Identity", m_curIdentity, allocator);

	StringBuffer buffer;
	buffer.Clear();
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	//writer.SetIndent(' ', 2);  
	document.Accept(writer);

	std::function<void(std::string, std::string)> saveHandler = [](std::string str, std::string fName)
	{
		std::string filepath = "D:\\CommondCode\\DDZ_AI\\Log\\" + fName;
		FILE *fp = fopen(filepath.c_str(), "wb");
		//fputs(str.c_str(), fp);  
		fwrite(str.c_str(), 1, str.size(), fp);
		fclose(fp);
	};
	std::thread th_save(saveHandler, std::string(buffer.GetString(), buffer.GetSize()), "2.json");
	th_save.detach();
}

void GameTable::LoadTable(const std::string & f)
{
	using namespace rapidjson;
	using namespace std;
	std::string path = "D:\\CommondCode\\DDZ_AI\\Log\\2.json";
#ifdef _WIN32
	const char rmode[3] = "rb";
#else
	const char rmode[2] = "r";
#endif
	FILE* fp = fopen(path.c_str(), rmode); // 非 Windows 平台使用 "r"
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	Document d;
	d.ParseStream(is);

	if (d.HasParseError())
	{
		printf("GetParseError %d\n", d.GetParseError());
	}
	m_lastCardStyle.Style = d["Last_Style"].GetInt();
	m_lastCardStyle.StartValue = d["Last_Start"].GetInt();
	m_lastCardStyle.EndValue = d["Last_End"].GetInt();

	const Value& l = d["l"];
	std::vector<uint8_t> cards;
	for (Value::ConstValueIterator itr = l.Begin(); itr != l.End(); ++itr)
		cards.push_back(itr->GetInt());
	m_playerStrategy[0]->GetHandCards()->Reset(cards);

	const Value& f1 = d["f1"];
	cards.clear();
	for (Value::ConstValueIterator itr = f1.Begin(); itr != f1.End(); ++itr)
		cards.push_back(itr->GetInt());
	m_playerStrategy[1]->GetHandCards()->Reset(cards);

	const Value& f2 = d["f2"];
	cards.clear();
	for (Value::ConstValueIterator itr = f2.Begin(); itr != f2.End(); ++itr)
		cards.push_back(itr->GetInt());
	m_playerStrategy[2]->GetHandCards()->Reset(cards);
}
