//included into ../Parser.hpp
#include "../../helpers/blankLines/blankLines.hpp"
#include "../../helpers/match/match.hpp"

namespace ieml {
	template<typename Char_, typename FileInclude_>
	Option<BasicNodeData<Char_>> BasicParser<Char_, FileInclude_>::parseNotScalar(Size indent) {
		if(skipBlankLinesLn<Char_>(pos_, end(), mark_)) {
			if(matchIndent<Char_>(pos_, end(), mark_, indent)) {
				if(auto list{parseList(indent)}) {
					return BasicNodeData<Char_>{list.value()};
				}
				if(auto map{parseMap(indent)}) {
					return BasicNodeData<Char_>{map.value()};
				}
			}
			exceptWithCheckSpace(FailedParseException::Reason::ExpectedNotScalar);
		}
		return {};
	}
}