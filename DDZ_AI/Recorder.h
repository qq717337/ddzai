#ifndef _RECORDER_H_
#define _RECORDER_H_
#include <stack>
#include <map>
#include <string>
#include "SplitType.h"
#include"HandCards.h"

template<class T>
class Memnto
{
private:
public:
	Memnto() {}
	virtual ~Memnto() {}
	virtual void  Restore(T* ref) = 0;
};

template<typename T>
class Recorder
{
private:
	static std::stack<Memnto<T>*> _Stack;
	static std::map<std::string, Memnto<T>*> _Map;
public:
	//static Recorder<T> R;
inline static void Push(Memnto<T>* ref) { _Stack.emplace(ref); }
inline static void Pop(T* ref) {
		//_Stack.top().
		if (ref!=NULL){
		Memnto<T>*topElem= _Stack.top();
		topElem->Restore(ref);
		delete topElem;
		}
		_Stack.pop();
	}
	inline static void Set(std::string key, Memnto<T>* ref) { _Map.insert(std::map<std::string, Memnto<T>*>::value_type(key, ref)); }
	inline static bool Get(std::string key, T* ref,bool remove=false) {
		auto iter= _Map.find(key);
		if (iter != _Map.end()) {
			Memnto<T>*topElem = iter->second;
			topElem->Restore(ref);
			return true;
		}
		else {
			return false;
		}
	}
	inline static bool Remove(std::string key) {
		auto iter = _Map.find(key);
		if (iter != _Map.end()) {
			Memnto<T>*topElem = iter->second;
			delete (topElem);
			_Map.erase(iter);
			return true;
		}
		else {
			return false;
		}
	}
}; 
template <typename T>
std::stack<Memnto<T>*> Recorder<T>::_Stack; //静态数据成员的初始化  

template <typename T>
std::map<std::string, Memnto<T>*> Recorder<T>::_Map; //静态数据成员的初始化  

class SplitMemnto :public Memnto<SplitType>
{
private:
	SplitType _Data;
public:
	SplitMemnto(const SplitMemnto& splitInfo)=delete;
	SplitMemnto operator=(const SplitMemnto& splitInfo) = delete;
	SplitMemnto(const SplitType& splitInfo);
	virtual ~SplitMemnto();
	virtual void Restore(SplitType* ref) override;
};

class HandCardsMemnto :public Memnto<HandCards>
{
private:
	uint8_t _Data[CARD_VALUE_LEN][4];
public:
	HandCardsMemnto(const HandCardsMemnto& splitInfo) = delete;
	HandCardsMemnto operator=(const HandCardsMemnto& splitInfo) = delete;
	HandCardsMemnto(const HandCards& splitInfo);
	virtual ~HandCardsMemnto();
	virtual void Restore(HandCards* ref) override;
};

#endif
