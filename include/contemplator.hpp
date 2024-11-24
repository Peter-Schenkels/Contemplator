#pragma once

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>

#include "regex_tokenizer.hpp"


struct template_key_values
{
	std::unordered_map<std::string, std::string> argument_instances;
	std::unordered_map<std::string, std::vector<std::string>> list_arguments_instances;

	std::vector<template_key_values> matrix_arguments_instances{};
};

struct contemplator
{
	static std::vector<std::string> generate(const std::vector<template_key_values>& data_base, const std::stringstream& template_string_stream);
	static std::vector<std::string> generate(const std::vector<template_key_values>& data_base, const std::filesystem::path& input_template);
};



