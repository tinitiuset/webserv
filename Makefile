NAME = webserv

COMPILER =	c++
STANDARD =	-std=c++98
DEBUG =		-g0
FLAGS =		-Wall -Werror -Wextra

SRC_FILES = main.cpp \
			./model/Conf.cpp \
			./model/Location.cpp \
			./model/Server.cpp \
			./requests/Request.cpp \
			./requests/RequestList.cpp \
			./requests/GetRequest.cpp \
			./requests/PostRequest.cpp \
			./requests/DeleteRequest.cpp \
			./requests/Resource.cpp \
			./requests/Response.cpp \
			./cookies/Cookie.cpp \
			./dynamicpages/ErrorPage.cpp \
			./utils/Logger.cpp \
			./utils/Codes.cpp \
			./utils/Utils.cpp \
			./exceptions/RequestException.cpp \
			./Multiplexer.cpp
HEADER_FILES =  ./model/Conf.hpp \
				./model/Location.hpp \
				./model/Server.hpp \
				./requests/Request.hpp \
				./requests/RequestList.hpp \
				./requests/GetRequest.hpp \
				./requests/PostRequest.hpp \
				./requests/DeleteRequest.hpp \
				./requests/Resource.hpp \
				./requests/Response.hpp \
				./cookies/Cookie.hpp \
				./dynamicpages/ErrorPage.hpp \
				./utils/Logger.hpp \
				./utils/Codes.hpp \
				./utils/Utils.hpp \
				./dynamicpages/ErrorPage.hpp \
				./exceptions/RequestException.hpp \
				./Multiplexer.hpp \
				./utils/defaults.hpp

all: $(NAME)

$(NAME): $(SRC_FILES) $(HEADER_FILES)
	@$(COMPILER)  $(FLAGS) $(STANDARD) $(DEBUG) $(SRC_FILES) -o $(NAME)
	@echo "Compiled"

clean:
	@rm -f $(NAME)
	@echo "Cleared executable"

fclean: clean

re: clean all

.PHONY: all clean fclean re