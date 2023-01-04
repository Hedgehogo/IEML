#pragma once
#include <string>

namespace ieml {
	/// @brief  Enum, storing the type of node
	enum class NodeType {
		Null,
		Scalar,
		List,
		Map,
	};
	
	/// @brief Creates a string from a node type.
	///
	/// @param NodeType Node type.
	///
	/// @return The node type as a string.
	std::string getNodeTypeString(NodeType nodeType);
}