#include "language_patterns.hpp"

#include <iostream>


parser_context token_patterns::eat_context(const token_iter& tokens_begin, const token_iter& tokens_end, node_type context_type)
{
	ast_node context = {};
	context.type = context_type;
	context.children = {};

	auto it_tokens = tokens_begin;
	while (true)
	{
		auto token = *it_tokens;

		// Are we matching a specific
		{
			matched_token_pattern matched_pattern = eat_specific(it_tokens, tokens_end);

			// Eat tokens we just matched
			it_tokens += matched_pattern.pattern.size();

			if (!matched_pattern.pattern.empty())
			{
				ast_node matched_pattern_node = {};
				matched_pattern_node.value = token_pattern_to_value(matched_pattern);
				matched_pattern_node.type = matched_pattern.matched;

				context.children.push_back(matched_pattern_node);
				continue;
			}
		}

		// Do we start a new context?
		parser_context eat_result;
		for (const auto& [context_type, pattern] : context_patterns)
		{
			if (const template_token& token = *it_tokens; token.type == pattern.front())
			{
				eat_result = eat_context(++it_tokens, tokens_end, context_type);
				break;
			}
		}
		if (eat_result.resulting_node.type != node_type::none)
		{
			context.children.push_back(eat_result.resulting_node);
			it_tokens = eat_result.next_token;
			continue;
		}

		// Do we end this context?
		for (const auto& [node_type, token_groups] : context_patterns)
		{
			if (const template_token& token = *it_tokens; token.type == token_groups.back())
			{
				if (!context_patterns.contains(node_type) && context_type != node_type::none)
				{
					std::cout << "Error: unexpected closing token" << std::endl;
					return { {}, tokens_end };
				}

				std::cout << token.matched;
				return { context, ++it_tokens };
			}
		}

		if (it_tokens != tokens_end)
		{
			return { context, tokens_end };
		}

		// If we couldn't match this token to a context or specific we had an invalid specifics pattern
		std::cout << "Error: Unable to match token" << std::endl;

		return {};
	}
}

matched_token_pattern token_patterns::eat_specific(const token_iter& tokens_begin, const token_iter& tokens_end)
{
	// Eat tokens
	auto                               it_tokens = tokens_begin;
	std::vector<matched_token_pattern> matched = {};

	template_token& token = *it_tokens;
	auto            pattern = match_token_pattern(it_tokens, tokens_end);
	return pattern;
}

matched_token_pattern token_patterns::match_token_pattern(
	const std::vector<template_token>::iterator& tokens_begin,
	const std::vector<template_token>::iterator& tokens_end)
{
	std::unordered_map<node_type, token_pattern> patterns_to_match = specifics_patterns;
	std::vector<template_token>::iterator        it                = tokens_begin;

	size_t                      index          = 0;
	std::vector<template_token> matched_tokens = {};

	while (!patterns_to_match.empty() && it != tokens_end)
	{
		template_token& token = *it;

		std::unordered_map<node_type, token_pattern> survivors;

		for (auto& pattern : patterns_to_match)
		{
			if (index >= pattern.second.size())
				continue;

			if (pattern.second[index] != token.type)
				continue;

			survivors.insert(pattern);
			matched_tokens.push_back(token);
		}

		patterns_to_match = {};
		patterns_to_match = survivors;

		if (patterns_to_match.size() == 1)
		{
			for (int parse_index = matched_tokens.size(); parse_index < patterns_to_match.begin()->second.size(); parse_index++)
			{
				template_token& token = *std::next(it);
				matched_tokens.push_back(token);
			}

			return { patterns_to_match.begin()->first, matched_tokens };
		}

		index++;
		it = std::next(it);
	}

	// error;
	return {};
}

std::string token_patterns::token_pattern_to_value(matched_token_pattern& pattern)
{
	switch (pattern.matched)
	{
		case node_type::context: return "";
		case node_type::matrix_context: return "";
		case node_type::argument: return pattern.pattern.at(1).matched;
		case node_type::argument_list:  return pattern.pattern.at(1).matched;
		case node_type::string: return pattern.pattern.at(0).matched;
		default: ;
	}

	return "ERROR";
}
