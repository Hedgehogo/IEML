#pragma once
#include <string>
#include <map>
#include <variant>
#include "../../fileSystem/fileSystem.hpp"
#include "../NodeType/NodeType.hpp"

namespace ieml {
	using llint = long long;
	
	class Node;
	
	using Tag = std::string;
	using FilePath = ieml::fs::path;
	
	/// @brief Node data storing null
	struct NullNodeData {};
	
	/// @brief Node data storing tag and other data
	struct TagNodeData;
	
	/// @brief Node data storing the file path and other data
	struct FileNodeData;
	
	/// @brief Node data storing scalar
	using ScalarNodeData = std::string;
	
	/// @brief Node data storing a list of other nodes
	using ListNodeData = std::vector<Node>;
	
	/// @brief Node data storing a map of named other nodes
	using MapNodeData = std::map<std::string, Node>;
	
	/// @brief Variant for storing different node data
	using NodeData = std::variant<NullNodeData, ScalarNodeData, ListNodeData, MapNodeData, TagNodeData, FileNodeData>;
	using PNodeData = NodeData*;
	
	struct TagNodeData {
		PNodeData data;
		Tag tag;
		
		TagNodeData(PNodeData data, Tag tag);
		
		TagNodeData(const TagNodeData& other);
		
		~TagNodeData();
	};
	
	struct FileNodeData {
		PNodeData data;
		FilePath filePath;
		
		FileNodeData(PNodeData data, FilePath filePath);
		
		FileNodeData(const FileNodeData& other);
		
		~FileNodeData();
	};
}
