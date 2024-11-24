#pragma once


enum token_group : size_t
{
	context_start,
	context_end,
	argument_start,
	argument_end,
	list_argument_start,
	list_argument_end,
	matrix_context_start,
	matrix_context_end,
	literal,
	END,
};

inline const char* to_string(const token_group e)
{
	switch (e)
	{
		case context_start: return "context_start";
		case context_end: return "context_end";
		case argument_start: return "argument_start";
		case argument_end: return "argument_end";
		case list_argument_start: return "list_argument_start";
		case list_argument_end: return "list_argument_end";
		case matrix_context_start: return "matrix_context_start";
		case matrix_context_end: return "matrix_context_end";
		case literal: return "literal";
		case END: return "END";
		default: return "unknown";
	}
}
