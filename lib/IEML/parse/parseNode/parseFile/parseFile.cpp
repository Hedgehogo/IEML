#include "parseFile.hpp"
#include "../../../node/Node/Node.hpp"
#include "../../../readFile/readFile.hpp"
#include "../../../preprocess/preprocess.hpp"
#include "../../../node/NodeData/file/FileNodeData.hpp"
#include "../../parse.hpp"
#include "../../match/match.hpp"
#include "../../emptyLines/emptyLines.hpp"
#include "../../exceptions/FailedParseException.hpp"
#include "../parseNotScalar/parseMap/parseMap.hpp"

namespace ieml {
	static constexpr auto reFile = ctll::fixed_string{R"(< .*?\n)" };
	
	bool hasRefMapAndMove(std::string::const_iterator& pos, std::string::const_iterator end, Mark &mark, size_t indent) {
		auto currentIndentFind{ctre::starts_with<reTabs>(pos, end)};
		if(currentIndentFind.size() == indent) {
			pos = currentIndentFind.end();
			mark.symbol += indent;
			return true;
		}
		return false;
	}
	
	void fillOutRefMap(std::string::const_iterator &pos, std::string::const_iterator end, const fs::path &filePath, RefKeeper &refKeeper, Mark &mark, size_t indent) {
		std::unique_ptr<INodeData> refMap{parseMap(pos, end, filePath, refKeeper, mark, indent)};
		for(auto& [key, value] : refMap->getMap()) {
			refKeeper.add(key, value);
		}
	}
	
	INodeData *parseFile(std::string::const_iterator &pos, std::string::const_iterator end, const fs::path &filePath, RefKeeper &refKeeper, Mark &mark, size_t indent) {
		if(auto find{matchAndEnter<reFile>(mark, pos, end)}; find) {
			Mark loadedMark{0, 0};
			RefKeeper loadedRefKeeper{refKeeper};
			fs::path loadedFilePath{fs::u8path(find.begin() + 2, find.end() - 1)};
			if(hasRefMapAndMove(pos, end, mark, indent + 1)) fillOutRefMap(pos, end, filePath, loadedRefKeeper, mark, indent + 1);
			INodeData *loadedData{parse(preprocess(readFile<char>(loadedFilePath)), loadedMark, loadedRefKeeper, loadedFilePath)};
			return new FileNodeData(std::unique_ptr<INodeData>{loadedData}, loadedFilePath);
		}
		throw FailedParseException{filePath, mark};
	}
}