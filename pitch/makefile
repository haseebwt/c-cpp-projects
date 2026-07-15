EXEC=pitch
CLIB= -lportaudio -lrt -lasound -ljack -pthread

$(EXEC): pitch.cpp
	g++ -Wextra -Wall -o $@ $^ $(CLIB)
clean:
	rm -rf $(EXEC)
.PHONY: clean 
