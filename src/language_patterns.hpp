#pragma once

#include <unordered_map>
#include "ast.hpp"
#include "template_token.hpp"
#include "token_group.hpp"


typedef std::vector<template_token>::iterator token_iter;

struct matched_token_pattern
{
	node_type matched = node_type::none;
	std::vector<template_token> pattern = {};
};

struct parser_context
{
	ast_node resulting_node;
	token_iter next_token;
};

struct token_patterns
{
	typedef std::vector<token_group> token_pattern;

	static inline std::unordered_map<node_type, token_pattern> specifics_patterns
	{
		{node_type::argument, { argument_start, literal, argument_end } },
		{node_type::argument_list, { list_argument_start, literal, list_argument_end } },
		{node_type::string, { literal } },
	};

	static inline std::unordered_map<node_type, token_pattern> context_patterns
	{
		{node_type::context, { context_start, context_end} },
		{node_type::matrix_context, { matrix_context_start, matrix_context_end } },
	};


	static matched_token_pattern match_token_pattern(const std::vector<template_token>::iterator& tokens_begin, const std::vector<template_token>::iterator& tokens_end);

	static std::string token_pattern_to_value(matched_token_pattern& pattern);

	static parser_context eat_context(const token_iter& tokens_begin, const token_iter& tokens_end, node_type context_type);

	static matched_token_pattern eat_specific(const token_iter& tokens_begin, const token_iter& tokens_end);
};


