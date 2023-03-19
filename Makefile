build:
	g++ main.cpp map_reduce.cpp perfect_powers.cpp -o tema1 -lpthread -lm -Wall -Werror

clean:
	rm -f tema1
