
#ifndef ICFGEN_PROGRAMEXCEPTION_H
#define ICFGEN_PROGRAMEXCEPTION_H

#include <stdexcept>

class ProgramException : public std::runtime_error
{		
	std::string message;

	template <class T, class... Args>
	static void process(std::ostream& stream, T t, Args... args)
	{		
		stream << t;
		process(stream, args...);
	}

	static void process(std::ostream& stream) {}

	template <class T, class... Args>
	static std::string join(T t, Args... args)
	{
		std::ostringstream oss;
		process(oss, t, args...);
		return oss.str();
	}
	
public:

	template <class T, class... Args>
	ProgramException(T t, Args... args) : std::runtime_error(join(t, args...))
	{
	
	}
};

#endif