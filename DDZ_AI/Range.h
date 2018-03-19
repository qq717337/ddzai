#ifndef _RANGE_H_
#define _RANGE_H_
template<class Type>
struct Range {
public:
	Type Start;
	Type End;
	inline Type Length()const {
        _ASSERT(End >= Start);
		return End - Start + 1;
	}
	Range() {
	}
	Range(Type A, Type B) {
		Start = A;
		End = B;
	}
	bool operator==(const Range& rhs)const {
		return Start == rhs.Start&& End == rhs.End;
	}
};
typedef  Range<uint8_t> CardRange;
#endif
