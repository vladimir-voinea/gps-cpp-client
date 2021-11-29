#pragma once

#include <string>
#include <json_dto/pub.hpp>

struct book_t
{
	book_t() = default;

	book_t(
		std::string author,
		std::string title )
		:	author{ std::move( author ) }
		,	title{ std::move( title ) }
	{}

	std::string author;
	std::string title;
};

namespace json_dto
{
	template < typename JSON_IO >
	void
	json_io( JSON_IO & io, book_t& book )
	{
		io
			& json_dto::mandatory( "author", book.author )
			& json_dto::mandatory( "title", book.title );
	}
}
