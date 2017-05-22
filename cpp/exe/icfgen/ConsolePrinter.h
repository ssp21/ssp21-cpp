
#ifndef ICFGEN_CONSOLEPRINTER_H
#define ICFGEN_CONSOLEPRINTER_H

#include "ssp21/crypto/IMessagePrinter.h"

#include <iostream>
#include <iomanip>

class ConsolePrinter : public ssp21::IMessagePrinter
{	

	size_t indentation_level = 0;

	void write_line_prefix()
	{
		for (int i = 0; i < indentation_level; ++i)
		{
			std::cout << " ";
		}		
	}	
	
public:

	void push_indent()
	{
		this->indentation_level += 2;
	}

	void pop_indent()
	{
		this->indentation_level -= 2;
	}

	virtual void print(const char* message) override
	{
		this->write_line_prefix();
		std::cout << message << std::endl;
	}
	
	virtual void print(const char* name, uint32_t value) override
	{
		this->write_line_prefix();
		std::cout << name << ": " << value << std::endl;
	}
	
	virtual void print(const char* name, const char* value) override
	{
		this->write_line_prefix();
		std::cout << name << ": " << value << std::endl;
	}
	
	virtual void print(const char* name, const ssp21::seq32_t& data) override
	{
		this->write_line_prefix();
		std::cout << name << " (length == " << data.length() << ")" << std::endl;

		this->push_indent();
		ssp21::seq32_t remainder = data;
		while (remainder.is_not_empty())
		{
			const ssp21::seq32_t line = remainder.take(16u);
			remainder = remainder.skip(16);

			this->write_line_prefix();

			for (uint32_t i = 0; i < line.length(); ++i)
			{
				if (i != 0) std::cout << ":";
				std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)line[i];
			}
			std::cout << std::endl;
		}
		this->pop_indent();
	}
	
};

#endif