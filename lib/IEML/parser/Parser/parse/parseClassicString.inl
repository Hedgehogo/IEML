//included into ../Parser.hpp
#include "../../helpers//blankLines/blankLines.hpp"

namespace ieml {
	template<typename Char_>
	struct StringMark {
		BasicStringCIter<Char_> pos;
		Size lastLength{0};
		Size enterCount{0};
		Size realLength{0};
	};
	
	template<typename Char_>
	bool matchIndent(BasicStringCIter<Char_>& pos, BasicStringCIter<Char_> end, Size indent) {
		Size currentIndent{0};
		BasicStringCIter<Char_> currentPos{pos};
		while(currentIndent < indent && currentPos != end && *currentPos == '\t') {
			currentIndent += 1;
			currentPos += 1;
		}
		if(currentIndent == indent) {
			pos = currentPos - 1;
			return true;
		}
		return false;
	}
	
	template<typename Char_>
	Option<StringMark<Char_>> isClassicString(BasicStringCIter<Char_> inputPos, BasicStringCIter<Char_> end, Size indent) {
		if(*inputPos != '\"')
			return {};
		
		StringMark<Char_> mark{inputPos + 1, 1};
		while(true) {
			if(mark.pos == end) {
				return {};
			}
			if(*mark.pos == '\"') {
				break;
			}
			if(*mark.pos == '\\') {
				mark.pos += 1;
				if(mark.pos == end) {
					return {};
				}
				switch (*mark.pos) {
					case '\\':
					case '\"':
					case 't':
					case 'n':
						mark.realLength += 1;
						mark.lastLength += 2;
						break;
					case '\n':
						mark.enterCount += 1;
						mark.lastLength = 0;
						mark.pos += 1;
						if(!matchIndent<Char_>(mark.pos, end, indent)) {
							return {};
						}
						break;
					default:
						mark.realLength += 2;
						mark.lastLength += 2;
						break;
				}
			} else if(*mark.pos == '\n') {
				mark.enterCount += 1;
				mark.realLength += 1;
				mark.lastLength = 0;
				mark.pos += 1;
				if(!matchIndent<Char_>(mark.pos, end, indent)) {
					return {};
				}
			} else {
				mark.lastLength += 1;
				mark.realLength += 1;
			}
			mark.pos += 1;
		}
		mark.pos += 1;
		mark.lastLength += 1;
		
		return mark;
	}
	
	template<typename Char_>
	void handleSymbol(BasicStringCIter<Char_>& input, BasicStringIter<Char_>& output, Size indent) {
		if(*input == '\\') {
			input += 1;
			if(*input != '\n') {
				switch(*input) {
					case  'n': *output = '\n'; break;
					case  't': *output = '\t'; break;
					case '\"': *output = '\"'; break;
					case '\\': *output = '\\'; break;
					default:
						*output = '\\';
						output += 1;
						*output = *input;
				}
				output += 1;
			}
		} else {
			*output = *input;
			output += 1;
		}
		if(*input == '\n') {
			input += 1 + indent;
		} else {
			input += 1;
		}
	}
	
	template<typename Char_>
	BasicString<Char_> handleClassicString(BasicStringCIter<Char_> first, Size realLength, Size indent) {
		BasicString<Char_> result(realLength, '\0');
		for(auto pos = result.begin(); pos < result.end();) {
			handleSymbol<Char_>(first, pos, indent);
		}
		return result;
	}
	
	template<typename Char_, typename FileInclude_>
	Option<BasicStringData<Char_>> BasicParser<Char_, FileInclude_>::parseClassicString(Size indent) {
		if(auto stringMark{isClassicString<Char_>(pos_, end(), indent)}) {
			BasicString<Char_> result{handleClassicString<Char_>(pos_ + 1, stringMark->realLength, indent)};
			pos_ = stringMark->pos;
			mark_.symbol = stringMark->lastLength;
			mark_.line += stringMark->enterCount;
			return BasicStringData<Char_>{result};
		}
		return {};
	}
}