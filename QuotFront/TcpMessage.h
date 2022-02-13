#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "asio.hpp"
#include <set>
#include <deque>
#include <map>

using asio::ip::tcp;
using namespace std;

class CMessage
{
public:
	// 根据行情现实情况进行设置
	enum { header_length = 14 };
	enum { max_body_length = 1024 };

	CMessage()
		: body_length_(0)
	{
		memset(data_, 0, sizeof(data_));
	}

	const char* data() const
	{
		return data_;
	}

	char* data()
	{
		return data_;
	}

	std::size_t length() const
	{
		return header_length + body_length_;
	}

	const char* body() const
	{
		return data_ + header_length;
	}

	char* body()
	{
		return data_ + header_length;
	}

	std::size_t body_length() const
	{
		return body_length_;
	}

	void body_length(std::size_t new_length)
	{
		body_length_ = new_length;
		if (body_length_ > max_body_length)
			body_length_ = max_body_length;
	}

	bool decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_, header_length);
		body_length_ = std::atoi(header);
		if (body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		char header[header_length + 1] = "";
		std::sprintf(header, "%4d", static_cast<int>(body_length_));
		std::memcpy(data_, header, header_length);
	}

	
public:
	int  iPackBodyLen;
	int  iPackTotalLen_;
	int  iPosBeforeMayaCode_;	
private:
	char data_[header_length + max_body_length];
	std::size_t body_length_;
	
};
