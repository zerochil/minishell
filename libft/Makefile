CC = cc

CFLAGS = -Wall -Wextra -Werror

BASE_SRCS = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_strlower.c ft_tolower.c ft_toupper.c ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c ft_strjoinx.c ft_if_else.c ft_strcmp.c ft_strcat.c ends_with.c ft_isnumber.c ft_strspn.c ft_isspace.c ft_numberlen.c ft_strarr.c 

MEMORY_MANAGEMNT_SRCS = resource_operations.c file_operations.c error_operations.c manager_operations.c free_operations.c malloc_operations.c node_operations.c node_operations_predicates.c manager_scope_operations.c 

CONTAINERS_SRCS = array/array_access.c array/array_helper_functions.c array/array_functions.c array/array_manipulation.c array/array_sort.c array/array_iterator.c array/array_search.c array/array_bulk.c\
				  string/string_functions.c string/string_manipulation.c string/string_segment_manipulation.c string/string_access.c string/string_peek.c\
				  containers_functions.c

GET_NEXT_LINE_SRCS = get_next_line.c 

OBJECTS_DIR = .objects/

SRCS = $(addprefix base/, $(BASE_SRCS)) \
	   $(addprefix memory_management/, $(MEMORY_MANAGEMNT_SRCS)) \
	   $(addprefix containers/, $(CONTAINERS_SRCS)) \
	   $(addprefix get_next_line/, $(GET_NEXT_LINE_SRCS)) \

OBJS = $(addprefix $(OBJECTS_DIR), $(SRCS:.c=.o))

HEADERS = libft.h \
		  memory_management/memory_management.h \
		  get_next_line/get_next_line.h \
		  containers/containers.h \
		  	containers/array/array.h \
		  	containers/string/string.h \
		  base/base.h

NAME = libft.a

all: $(NAME)

$(NAME): $(OBJS)
	ar crs $(NAME) $(OBJS)

$(OBJECTS_DIR)%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
