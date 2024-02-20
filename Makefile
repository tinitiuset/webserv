NAME = webserv

COMPILER =	c++
STANDARD =	-std=c++11
DEBUG =		-g3	# 0, 1, 2, 3 DEBUG INFORMATION LEVELS
FLAGS =		#-Wall -Werror -Wextra

#SRC_FILES = main.cpp ./model/*.cpp ./requests/*.cpp ./utils/*.cpp Multiplexer.cpp
#HEADER_FILES = ./model/*.hpp ./requests/*.hpp ./utils/*.hpp ./cgi/*.hpp ./utils/defaults.hpp

SRC_FILES = main.cpp \
            ./model/Conf.cpp \
            ./model/Location.cpp \
            ./model/Server.cpp \
            ./requests/GetRequest.cpp \
            ./requests/PostRequest.cpp \
            ./requests/Request.cpp \
            ./requests/Resource.cpp \
            ./requests/Response.cpp \
            ./utils/Logger.cpp \
            ./utils/Utils.cpp \
            Multiplexer.cpp

HEADER_FILES =  ./model/*.hpp \
                ./model/Conf.hpp \
                ./model/Location.hpp \
                ./model/Server.hpp \
                ./requests/GetRequest.hpp \
                ./requests/PostRequest.hpp \
                ./requests/Request.hpp \
                ./requests/Resource.hpp \
                ./requests/Response.hpp \
                ./utils/Logger.hpp \
                ./utils/Utils.hpp \
                Multiplexer.hpp \
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