#pragma once
#include <string>

#include "token_group.hpp"


struct template_token
{
	token_group type;

	std::string prefix{};
	std::string matched{};
};

