//included into ../Parser.hpp

namespace ieml {
	template<typename Char_, typename FileInclude_>
	BasicNodeData<Char_> BasicParser<Char_, FileInclude_>::parseNode(Size indent) {
		if(skipBlankLinesLn<Char_>(pos_, end(), mark_)) {
			if(!matchIndent<Char_>(pos_, end(), mark_, indent)) {
				except(FailedParseException::Reason::FailedDetermineType);
			}
		}
		if(auto tag{parseTag(indent)})
			return BasicNodeData<Char_>{*tag};
		if(auto takeAnchor{parseTakeAnchor(indent)})
			return BasicNodeData<Char_>{*takeAnchor};
		if(auto getAnchor{parseGetAnchor(indent)})
			return BasicNodeData<Char_>{*getAnchor};
		if(auto file{parseFile(indent)})
			return BasicNodeData<Char_>{*file};
		if(auto list{parseList(indent)})
			return BasicNodeData<Char_>{*list};
		if(auto map{parseMap(indent)})
			return BasicNodeData<Char_>{*map};
		return parseScalar(indent);
	}
}