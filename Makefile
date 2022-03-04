all:
	echo "Cpp related files"
	find . -maxdepth 1 -type f -regextype posix-extended -regex ".*(obj|cpp|out)"
	g++ -Wall -Wextra -Werror src.cpp -lglfw -lX11 -lpthread -lXrandr -lXi -ldl -lstdc++ -lm -g
	# g++ -Wall -Wextra -Werror ./glad/glad.c grid.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lstdc++ -lm
