#pragma once
#include <array>
#include <regex>
#include <vector>

#include "template_token.hpp"
#include "token_group.hpp"


struct regex_tokenizer
{
	int current_group_id = 0;
	std::array<token_group, static_cast<unsigned long long>(token_group::END)> groups;
	std::string pattern;

	void add_token_pattern(const std::string& token_pattern, token_group group);

	[[nodiscard]] token_group find_token(const std::smatch& matched) const;

	[[nodiscard]] std::vector<template_token> match(std::string input) const;
};
