all: clean compile run

compile: main.cpp
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ -o driver.out main.cpp


run: 
	@echo "-------------------------------------------"
	@echo "Running..."
	./driver.out
	@echo "Completed."
	
clean:
	@echo "-------------------------------------------"
	@echo "Removing compiled files..."
	@rm -f *.out
