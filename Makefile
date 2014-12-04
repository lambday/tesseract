SRC			= src/tesseract
INCLUDES	= -Isrc -I/usr/include/eigen3
OPTS		= -O3 -std=c++11 -fPIC
REGRESSION	= $(SRC)/regression
OBJECTS		= $(REGRESSION)/LeastSquares.o
UNITSRC		= sanity/unit
LIBS		= -L. -ltsr -lshogun
TESTS		= tests

libtsr.so: $(OBJECTS)
	g++ $(OPTS) -shared $(OBJECTS) $(INCLUDES) -o libtsr.so -lshogun
$(REGRESSION)/LeastSquares.o: $(REGRESSION)/LeastSquares.hpp
	g++ $(OPTS) -c $(REGRESSION)/LeastSquares.cpp $(INCLUDES) -o $(REGRESSION)/LeastSquares.o
test: libtsr.so $(UNITSRC)/LeastSquares_unittest.cpp
	# compile all the tests
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/LeastSquares_unittest.cpp -o $(TESTS)/LeastSquares_unittest
	# run all the tests
	$(TESTS)/LeastSquares_unittest
	# delete all the tests
	rm $(TESTS)/*
doc: libtsr.so doc/Doxyfile
	doxygen doc/Doxyfile
clean:
	rm $(REGRESSION)/*.o libtsr.so
	rm -rf doc/html
	rm -rf doc/latex
