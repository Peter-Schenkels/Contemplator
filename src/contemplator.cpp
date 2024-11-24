#include "include/contemplator.hpp"

#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>
#include <string>

#include "language_patterns.hpp"



struct generation_context
{
	size_t argument_list_index = 0;
	size_t argument_list_entries = 0;
	std::string generated;
	bool discard = false;
};

const generation_context discard_context = {
	.argument_list_index = 0,
	.argument_list_entries = 0,
	.generated = "",
	.discard = true
};


static std::vector<template_token> tokenize(const std::stringstream& template_buffer);
static std::string generate_using_tree(const template_key_values& key_data_base, const ast_node& tree);

static generation_context handle_argument(const template_key_values& key_data_base, const ast_node& node);
static generation_context handle_list_argument(const template_key_values& key_data_base, const generation_context& context, const ast_node& node);
static generation_context handle_context(const template_key_values& key_data_base, const ast_node& node);
static generation_context handle_string(const ast_node& node);
static generation_context handle_matrix_context(const template_key_values& key_data_base, const ast_node& node);
static generation_context handle_node(const template_key_values& key_data_base, const generation_context& context, const ast_node& node);



std::vector<template_token> tokenize(const std::stringstream& template_buffer)
{
	const std::string rtest = template_buffer.str();

	regex_tokenizer tokenizer;

	tokenizer.add_token_pattern("\\$\\<", token_group::context_start);
	tokenizer.add_token_pattern("\\>\\$", token_group::context_end);

	tokenizer.add_token_pattern("\\$\\[", token_group::list_argument_start);
	tokenizer.add_token_pattern("\\]\\$", token_group::list_argument_end);

	tokenizer.add_token_pattern("\\$\\(", token_group::argument_start);
	tokenizer.add_token_pattern("\\)\\$", token_group::argument_end);

	tokenizer.add_token_pattern("\\$\\#", token_group::matrix_context_start);
	tokenizer.add_token_pattern("\\#\\$", token_group::matrix_context_end);

	return tokenizer.match(rtest);
}


generation_context handle_argument(
	const template_key_values& key_data_base, 
	const ast_node& node)
{
	const auto it = key_data_base.argument_instances.find(node.value);
	if (it == key_data_base.argument_instances.end()) {
		std::cout << "Could not find entry for argument: " << node.value << std::endl;
		return discard_context;
	}

	generation_context new_context = {
		.argument_list_index = 0,
		.argument_list_entries = 0,
		.generated = it->second,
		.discard = false
	};

	return new_context;
}


generation_context handle_list_argument(
	const template_key_values& key_data_base, 
	const generation_context& context, 
	const ast_node& node)
{
	const auto it = key_data_base.list_arguments_instances.find(node.value);
	if (it == key_data_base.list_arguments_instances.end()) {
		std::cout << "Could not find list for argument: " << node.value << std::endl;
		return discard_context;
	}

	if (it->second.empty())
	{
		return discard_context;
	}

	generation_context new_context = {
		.argument_list_index = 0,
		.argument_list_entries = 0,
		.generated = it->second.at(context.argument_list_index),
		.discard = false
	};

	return new_context;
}


generation_context handle_context(
	const template_key_values& key_data_base, 
	const ast_node& node)
{
	std::vector<size_t> sizes;

	// Collect sizes for argument list nodes
	for (const ast_node& child : node.children)
	{
		if (child.type == node_type::argument_list)
		{
			const auto it = key_data_base.list_arguments_instances.find(child.value);
			sizes.push_back(it != key_data_base.list_arguments_instances.end() ? it->second.size() : 0);
		}
	}

	// Check if all sizes are equal
	bool list_arguments_equal = sizes.empty() || std::ranges::all_of(sizes, [&](const size_t size) { return size == sizes[0]; });

	// Set new_context based on the equality check
	generation_context new_context = {};
	new_context.argument_list_entries = list_arguments_equal && !sizes.empty() ? sizes[0] : 0;

	if (!list_arguments_equal)
	{
		std::cout << "Uneven amount of list arguments" << std::endl;
	}

	do
	{
		for (const ast_node& child_node : node.children)
		{
			const generation_context out = handle_node(key_data_base, new_context, child_node);

			// Nested nodes can be discarded
			if ( out.discard && (child_node.type == node_type::context || child_node.type == node_type::matrix_context))
			{
				continue;
			}
			// Anything else breaks
			else if( out.discard )
			{
				return discard_context;
			}
			else
			{
				new_context.generated += out.generated;
			}
		}

		new_context.argument_list_index++;
	}
	while(new_context.argument_list_index < new_context.argument_list_entries);

	return new_context;
}


generation_context handle_string(const ast_node& node)
{
	return generation_context{ 0, 0, node.value, false};
}


generation_context handle_matrix_context(const template_key_values& key_data_base, const ast_node& node)
{
	generation_context new_context = {};

	for (const template_key_values& matrix_arguments_instance : key_data_base.matrix_arguments_instances)
	{
		// cast current node as context
		generation_context out = handle_context(matrix_arguments_instance, node);

		if (out.discard)
		{
			continue;
		}

		new_context.generated += out.generated;
	}

	return new_context;
}


generation_context handle_node(const template_key_values& key_data_base, const generation_context& context, const ast_node& node)
{
	switch (node.type)
	{
		case node_type::context:        return handle_context(key_data_base, node);
		case node_type::string:         return handle_string(node);
		case node_type::argument:       return handle_argument(key_data_base, node);
		case node_type::argument_list:  return handle_list_argument(key_data_base, context, node);
		case node_type::matrix_context: return handle_matrix_context(key_data_base, node);
		case node_type::none:
			break;
	}

	return discard_context;
}


std::string generate_using_tree(const template_key_values& key_data_base, const ast_node& tree)
{
	std::string content;

	if (tree.type == node_type::none)
	{
		for (const ast_node& child : tree.children)
		{
			const generation_context in = { 0, 0, "", false };
			const generation_context out = handle_node(key_data_base, in, child);

			if (out.discard)
			{
				content += "";
				continue;
			}

			content += out.generated;
		}
	}

	return content;
}


std::vector<std::string> contemplator::generate(const std::vector<template_key_values>& data_base, const std::stringstream& template_string_stream)
{
	std::vector<template_token> tokens = tokenize(template_string_stream);

	token_patterns parser = {};
	auto [resulting_node, next_token] = parser.eat_context(tokens.begin(), tokens.end(), node_type::none);

	std::vector<std::string> generated_strings = {};
	generated_strings.reserve(data_base.size());

	for (const template_key_values& context_data_base : data_base)
	{
		generated_strings.emplace_back(generate_using_tree(context_data_base, resulting_node));
	}

	return generated_strings;
}

std::vector<std::string> contemplator::generate(const std::vector<template_key_values>& data_base, const std::filesystem::path& input_template)
{
	std::ifstream template_stream_buffer(input_template);

	if (!template_stream_buffer.is_open())
	{
		std::cout << "ERROR: couldn't load :" << input_template << std::endl;
		return {};
	}

	std::stringstream template_string_stream;
	template_string_stream << template_stream_buffer.rdbuf();

	return generate(data_base, template_string_stream);
}
