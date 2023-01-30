#pragma once
#include <ctre/functions.hpp>
#include "../../../../node/Mark/Mark.hpp"
#include "../../../../node/NodeData/NodeData.hpp"
#include "../../../../reference/RefKeeper/RefKeeper.hpp"

namespace ieml {
	static constexpr auto reMapKey = ctll::fixed_string{R"([^\"\n<>]*?: ?)" };
	
	NodeData parseMap(std::string::const_iterator &pos, std::string::const_iterator end, const fs::path &filePath, RefKeeper &refKeeper, Mark &mark, size_t indent);
}
