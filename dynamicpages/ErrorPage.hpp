# ifndef ERRORPAGE_HPP
# define ERRORPAGE_HPP

# include <string>
# include "../utils/Codes.hpp"
# include "../utils/Utils.hpp"

class ErrorPage {

public:
	static std::string build(int status);
};

# endif
