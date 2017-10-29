#pragma once
template<class Type>
struct Range {
public:
	Type Start;
	Type End;
	Type Length() {
		_ASSERTE(End >= Start);
		return End - Start + 1;
	}
	Range() {
	}
	Range(Type A, Type B) {
		Start = A;
		End = B;
	}
};
typedef  Range<uint8_t> CardRange;