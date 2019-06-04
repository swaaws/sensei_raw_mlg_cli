all : arch
arch : main.cpp
	g++ main.cpp -lusb-1.0 -o mlg
