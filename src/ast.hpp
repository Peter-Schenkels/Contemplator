#pragma once

#include <vector>
#include <string>


enum class node_type
{
	context,
	string,
	argument,
	argument_list,
	matrix_context,
	none,
};

inline const char* to_string(node_type e)
{
	switch (e)
	{
		case node_type::context: return "context";
		case node_type::string: return "string";
		case node_type::argument: return "argument";
		case node_type::argument_list: return "argument_list";
		case node_type::matrix_context: return "matrix_context";
		case node_type::none: return "none";
		default: return "unknown";
	}
}

struct ast_node
{
	node_type type = node_type::none;
	std::string value;

	std::vector<ast_node> children = {};
};


