NAME = webserv

COMPILER =	c++
STANDARD =	-std=c++98
DEBUG =		-g0	# 0, 1, 2, 3 DEBUG INFORMATION LEVELS
FLAGS =		-Wall -Werror -Wextra

SRC_FILES = main.cpp Server.cpp Multiplexer.cpp Logger.cpp Request.cpp PostRequest.cpp \
			GetRequest.cpp Response.cpp Resource.cpp ConfigFile.cpp EachServer.cpp

all: $(NAME)

$(NAME): $(SRC_FILES)
	@$(COMPILER)  $(FLAGS) $(STANDARD) $(DEBUG) $(SRC_FILES) -o $(NAME)
	@echo "Compiled"

clean:
	@rm -f $(NAME)
	@echo "Cleared executable"

fclean: clean

re: clean all

.PHONY: all clean fclean re