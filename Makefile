SRC			= src/tesseract
INCLUDES	= -Isrc -I/usr/include/eigen3
OPTS		= -O3 -std=c++11 -fPIC
REGRESSION	= $(SRC)/regression
IO			= $(SRC)/io
PREPROCESSOR= $(SRC)/preprocessor
ERRORS		= $(SRC)/errors
ALGORITHM	= $(SRC)/algorithm
EVALUATION	= $(SRC)/evaluation
FEATURES	= $(SRC)/features
OBJECTS		= $(REGRESSION)/LeastSquares.o \
			  $(IO)/IDX3Reader.o $(IO)/IDX1Reader.o $(IO)/FileReader.o \
			  $(PREPROCESSOR)/DataGenerator.o \
			  $(ERRORS)/SumSquaredError.o $(ERRORS)/PearsonsCorrelation.o $(ERRORS)/SquaredMultipleCorrelation.o \
			  $(EVALUATION)/DataSet.o $(EVALUATION)/Evaluation.o \
			  $(ALGORITHM)/Dummy.o \
			  $(FEATURES)/Features.o
UNITSRC		= sanity/unit
INTSRC		= sanity/integration
UNITTESTS	= $(UNITSRC)/LeastSquares_unittest.cpp $(UNITSRC)/IDX3Reader_unittest.cpp \
			  $(UNITSRC)/IDX1Reader_unittest.cpp $(UNITSRC)/FileReader_unittest.cpp \
			  $(UNITSRC)/DataGenerator_unittest.cpp $(UNITSRC)/UnitL2Normalizer_unittest.cpp \
			  $(UNITSRC)/Error_unittest.cpp \
			  $(INTSRC)/Evaluation_integration.cpp
LIBS		= -L. -ltsr -lshogun
TESTS		= tests

libtsr.so: $(OBJECTS)
	g++ $(OPTS) -shared $(OBJECTS) $(INCLUDES) -o libtsr.so -lshogun
$(REGRESSION)/LeastSquares.o: $(REGRESSION)/LeastSquares.hpp $(REGRESSION)/LeastSquares.cpp
	g++ $(OPTS) -c $(REGRESSION)/LeastSquares.cpp $(INCLUDES) -o $(REGRESSION)/LeastSquares.o
$(IO)/IDX3Reader.o: $(IO)/IDX3Reader.hpp $(IO)/IDX3Reader.cpp
	g++ $(OPTS) -c $(IO)/IDX3Reader.cpp $(INCLUDES) -o $(IO)/IDX3Reader.o
$(IO)/IDX1Reader.o: $(IO)/IDX1Reader.hpp $(IO)/IDX1Reader.cpp
	g++ $(OPTS) -c $(IO)/IDX1Reader.cpp $(INCLUDES) -o $(IO)/IDX1Reader.o
$(IO)/FileReader.o: $(IO)/FileReader.hpp $(IO)/FileReader.cpp
	g++ $(OPTS) -c $(IO)/FileReader.cpp $(INCLUDES) -o $(IO)/FileReader.o
$(PREPROCESSOR)/DataGenerator.o: $(PREPROCESSOR)/DataGenerator.hpp $(PREPROCESSOR)/DataGenerator.cpp
	g++ $(OPTS) -c $(PREPROCESSOR)/DataGenerator.cpp $(INCLUDES) -o $(PREPROCESSOR)/DataGenerator.o
$(ERRORS)/SumSquaredError.o: $(ERRORS)/SumSquaredError.hpp $(ERRORS)/SumSquaredError.cpp
	g++ $(OPTS) -c $(ERRORS)/SumSquaredError.cpp $(INCLUDES) -o $(ERRORS)/SumSquaredError.o
$(ERRORS)/PearsonsCorrelation.o: $(ERRORS)/PearsonsCorrelation.hpp $(ERRORS)/PearsonsCorrelation.cpp
	g++ $(OPTS) -c $(ERRORS)/PearsonsCorrelation.cpp $(INCLUDES) -o $(ERRORS)/PearsonsCorrelation.o
$(ERRORS)/SquaredMultipleCorrelation.o: $(ERRORS)/SquaredMultipleCorrelation.hpp $(ERRORS)/SquaredMultipleCorrelation.cpp
	g++ $(OPTS) -c $(ERRORS)/SquaredMultipleCorrelation.cpp $(INCLUDES) -o $(ERRORS)/SquaredMultipleCorrelation.o
$(ALGORITHM)/Dummy.o: $(ALGORITHM)/Dummy.hpp $(ALGORITHM)/Dummy.cpp
	g++ $(OPTS) -c $(ALGORITHM)/Dummy.cpp $(INCLUDES) -o $(ALGORITHM)/Dummy.o
$(FEATURES)/Features.o: $(FEATURES)/Features.hpp $(FEATURES)/Features.cpp
	g++ $(OPTS) -c $(FEATURES)/Features.cpp $(INCLUDES) -o $(FEATURES)/Features.o
$(EVALUATION)/DataSet.o: $(EVALUATION)/DataSet.hpp $(EVALUATION)/DataSet.cpp
	g++ $(OPTS) -c $(EVALUATION)/DataSet.cpp $(INCLUDES) -o $(EVALUATION)/DataSet.o
$(EVALUATION)/Evaluation.o: $(EVALUATION)/Evaluation.hpp $(EVALUATION)/Evaluation.cpp
	g++ $(OPTS) -c $(EVALUATION)/Evaluation.cpp $(INCLUDES) -o $(EVALUATION)/Evaluation.o
test: libtsr.so $(UNITTESTS)
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/LeastSquares_unittest.cpp -o $(TESTS)/LeastSquares_unittest
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/IDX3Reader_unittest.cpp -o $(TESTS)/IDX3Reader_unittest
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/IDX1Reader_unittest.cpp -o $(TESTS)/IDX1Reader_unittest
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/FileReader_unittest.cpp -o $(TESTS)/FileReader_unittest
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/DataGenerator_unittest.cpp -o $(TESTS)/DataGenerator_unittest
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/UnitL2Normalizer_unittest.cpp -o $(TESTS)/UnitL2Normalizer_unittest
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/Error_unittest.cpp -o $(TESTS)/Error_unittest
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(INTSRC)/Evaluation_integration.cpp -o $(TESTS)/Evaluation_integration
	$(TESTS)/LeastSquares_unittest
	$(TESTS)/IDX3Reader_unittest
	$(TESTS)/IDX1Reader_unittest
	$(TESTS)/FileReader_unittest
	$(TESTS)/DataGenerator_unittest
	$(TESTS)/UnitL2Normalizer_unittest
	$(TESTS)/Error_unittest
	$(TESTS)/Evaluation_integration
	rm $(TESTS)/*
doc: libtsr.so doc/Doxyfile
	doxygen doc/Doxyfile
clean:
	rm $(OBJECTS) libtsr.so
	rm -rf doc/html
	rm -rf doc/latex
