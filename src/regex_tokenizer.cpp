#include <iostream>

#include "regex_tokenizer.hpp"


void regex_tokenizer::add_token_pattern(const std::string& token_pattern, token_group group)
{
	if (current_group_id > groups.size())
	{
		std::cout << "Tokenizer Error: Added more patterns than available group tokens";
		return;
	}

	if (group >= token_group::END)
	{
		std::cout << "Tokenizer Error: Group is not a valid token";
		return;
	}

	if (!pattern.empty())
	{
		pattern += "|";
	}

	pattern += "(" + token_pattern + ")";
	groups[current_group_id] = group;
	current_group_id++;
}

token_group regex_tokenizer::find_token(const std::smatch& matched) const
{
	for (unsigned int group_index = 0; group_index < token_group::END; group_index++)
	{
		if (current_group_id <= group_index)
		{
			return token_group::END;
		}

		if (matched[group_index + 1].matched)
		{
			return groups[group_index];
		}
	}

	return token_group::END;
}

std::vector<template_token> regex_tokenizer::match(std::string input) const
{
	std::regex tokenizer_pattern = std::regex(pattern);

	auto regex_it_begin = std::sregex_iterator(input.begin(), input.end(), tokenizer_pattern);
	auto regex_it_end   = std::sregex_iterator();

	std::vector<template_token> found_nodes;
	int                         nr_of_nodes = 0;
	std::smatch                 out_match;

	for (std::sregex_iterator i = regex_it_begin; i != regex_it_end; ++i)
	{
		out_match = *i;

		found_nodes.emplace_back(find_token(out_match), out_match.prefix(), out_match[0]);
		nr_of_nodes++;
	}

	if (nr_of_nodes > 0)
	{
		found_nodes.emplace_back(token_group::END, out_match.suffix(), "");
	}

	std::vector<template_token> processed_nodes = {};
	processed_nodes.reserve(found_nodes.size() * 2);

	for (const auto& found_node : found_nodes)
	{
		// Seperate the node
		template_token literal_node = { token_group::literal, "", found_node.prefix };

		processed_nodes.push_back(literal_node);
		processed_nodes.push_back(found_node);
	}

	return processed_nodes;
}
