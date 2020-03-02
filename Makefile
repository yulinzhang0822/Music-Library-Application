CXXFLAGS=-ggdb -std=c++11 -Wpedantic -Wall -Wextra -Werror -Wzero-as-null-pointer-constant
MusicLibrary: MusicLibrary.cpp
	g++ $(CXXFLAGS) MusicLibrary.cpp -o MusicLibrary.out



clean:
	echo ----------removing executable program MusicLibrary.out----------
	/bin/rm MusicLibrary.out
