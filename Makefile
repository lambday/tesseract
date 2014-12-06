SRC			= src/tesseract
INCLUDES	= -Isrc -I/usr/include/eigen3
OPTOPTS		= -O3 -std=c++11
DEBUGOPTS	= -g3 -std=c++11
OPTS		= $(OPTOPTS)
LIBFLAG		= -fPIC
REGRESSION	= $(SRC)/regression
IO			= $(SRC)/io
PREPROCESSOR= $(SRC)/preprocessor
ERRORS		= $(SRC)/errors
ALGORITHM	= $(SRC)/algorithm
EVALUATION	= $(SRC)/evaluation
FEATURES	= $(SRC)/features
REGULARIZER	= $(SRC)/regularizer
COMPUTATION	= $(SRC)/computation
OBJECTS		= $(REGRESSION)/LeastSquares.o \
			  $(IO)/IDX3Reader.o $(IO)/IDX1Reader.o $(IO)/FileReader.o \
			  $(PREPROCESSOR)/DataGenerator.o \
			  $(ERRORS)/SumSquaredError.o $(ERRORS)/PearsonsCorrelation.o $(ERRORS)/SquaredMultipleCorrelation.o \
			  $(EVALUATION)/DataSet.o $(EVALUATION)/Evaluation.o \
			  $(ALGORITHM)/Dummy.o $(ALGORITHM)/ForwardRegression.o $(ALGORITHM)/LocalSearch.o \
			  $(FEATURES)/Features.o \
			  $(REGULARIZER)/DummyRegularizer.o $(REGULARIZER)/SmoothedDifferentialEntropy.o \
			  $(COMPUTATION)/ComputeFunction.o
UNITSRC		= sanity/unit
INTSRC		= sanity/integration
TESTDIR		= tests
TESTS		= $(TESTDIR)/LeastSquares_unittest $(TESTDIR)/IDX3Reader_unittest \
			  $(TESTDIR)/IDX1Reader_unittest $(TESTDIR)/FileReader_unittest \
			  $(TESTDIR)/DataGenerator_unittest $(TESTDIR)/UnitL2Normalizer_unittest \
			  $(TESTDIR)/Error_unittest $(TESTDIR)/ComputeFunction_unittest\
			  $(TESTDIR)/ForwardRegression_unittest $(TESTDIR)/SmoothedDifferentialEntropy_unittest \
			  $(TESTDIR)/LocalSearch_unittest \
			  $(TESTDIR)/Evaluation_integration
LIBS		= -L. -ltsr
MEMCHECK	= valgrind --leak-check=full --track-origins=yes

all: libtsr.so $(TESTS) doc

lib: libtsr.so

libtsr.so: $(OBJECTS)
	g++ $(OPTS) $(LIBFLAG) -shared $(OBJECTS) $(INCLUDES) -o libtsr.so
$(REGRESSION)/LeastSquares.o: $(REGRESSION)/LeastSquares.hpp $(REGRESSION)/LeastSquares.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(REGRESSION)/LeastSquares.cpp $(INCLUDES) -o $(REGRESSION)/LeastSquares.o
$(IO)/IDX3Reader.o: $(IO)/IDX3Reader.hpp $(IO)/IDX3Reader.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(IO)/IDX3Reader.cpp $(INCLUDES) -o $(IO)/IDX3Reader.o
$(IO)/IDX1Reader.o: $(IO)/IDX1Reader.hpp $(IO)/IDX1Reader.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(IO)/IDX1Reader.cpp $(INCLUDES) -o $(IO)/IDX1Reader.o
$(IO)/FileReader.o: $(IO)/FileReader.hpp $(IO)/FileReader.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(IO)/FileReader.cpp $(INCLUDES) -o $(IO)/FileReader.o
$(PREPROCESSOR)/DataGenerator.o: $(PREPROCESSOR)/DataGenerator.hpp $(PREPROCESSOR)/DataGenerator.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(PREPROCESSOR)/DataGenerator.cpp $(INCLUDES) -o $(PREPROCESSOR)/DataGenerator.o
$(ERRORS)/SumSquaredError.o: $(ERRORS)/SumSquaredError.hpp $(ERRORS)/SumSquaredError.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(ERRORS)/SumSquaredError.cpp $(INCLUDES) -o $(ERRORS)/SumSquaredError.o
$(ERRORS)/PearsonsCorrelation.o: $(ERRORS)/PearsonsCorrelation.hpp $(ERRORS)/PearsonsCorrelation.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(ERRORS)/PearsonsCorrelation.cpp $(INCLUDES) -o $(ERRORS)/PearsonsCorrelation.o
$(ERRORS)/SquaredMultipleCorrelation.o: $(ERRORS)/SquaredMultipleCorrelation.hpp $(ERRORS)/SquaredMultipleCorrelation.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(ERRORS)/SquaredMultipleCorrelation.cpp $(INCLUDES) -o $(ERRORS)/SquaredMultipleCorrelation.o
$(ALGORITHM)/Dummy.o: $(ALGORITHM)/Dummy.hpp $(ALGORITHM)/Dummy.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(ALGORITHM)/Dummy.cpp $(INCLUDES) -o $(ALGORITHM)/Dummy.o
$(ALGORITHM)/ForwardRegression.o: $(ALGORITHM)/ForwardRegression.hpp $(ALGORITHM)/ForwardRegression.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(ALGORITHM)/ForwardRegression.cpp $(INCLUDES) -o $(ALGORITHM)/ForwardRegression.o
$(ALGORITHM)/LocalSearch.o: $(ALGORITHM)/LocalSearch.hpp $(ALGORITHM)/LocalSearch.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(ALGORITHM)/LocalSearch.cpp $(INCLUDES) -o $(ALGORITHM)/LocalSearch.o
$(FEATURES)/Features.o: $(FEATURES)/Features.hpp $(FEATURES)/Features.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(FEATURES)/Features.cpp $(INCLUDES) -o $(FEATURES)/Features.o
$(EVALUATION)/DataSet.o: $(EVALUATION)/DataSet.hpp $(EVALUATION)/DataSet.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(EVALUATION)/DataSet.cpp $(INCLUDES) -o $(EVALUATION)/DataSet.o
$(EVALUATION)/Evaluation.o: $(EVALUATION)/Evaluation.hpp $(EVALUATION)/Evaluation.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(EVALUATION)/Evaluation.cpp $(INCLUDES) -o $(EVALUATION)/Evaluation.o
$(COMPUTATION)/ComputeFunction.o: $(COMPUTATION)/ComputeFunction.hpp $(COMPUTATION)/ComputeFunction.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(COMPUTATION)/ComputeFunction.cpp $(INCLUDES) -o $(COMPUTATION)/ComputeFunction.o
$(REGULARIZER)/DummyRegularizer.o: $(REGULARIZER)/DummyRegularizer.hpp $(REGULARIZER)/DummyRegularizer.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(REGULARIZER)/DummyRegularizer.cpp $(INCLUDES) -o $(REGULARIZER)/DummyRegularizer.o
$(REGULARIZER)/SmoothedDifferentialEntropy.o: $(REGULARIZER)/SmoothedDifferentialEntropy.hpp $(REGULARIZER)/SmoothedDifferentialEntropy.cpp
	g++ $(OPTS) $(LIBFLAG) -c $(REGULARIZER)/SmoothedDifferentialEntropy.cpp $(INCLUDES) -o $(REGULARIZER)/SmoothedDifferentialEntropy.o

check: libtsr.so $(TESTS)
	$(TESTDIR)/LeastSquares_unittest
	$(TESTDIR)/IDX3Reader_unittest
	$(TESTDIR)/IDX1Reader_unittest
	$(TESTDIR)/FileReader_unittest
	$(TESTDIR)/DataGenerator_unittest
	$(TESTDIR)/UnitL2Normalizer_unittest
	$(TESTDIR)/Error_unittest
	$(TESTDIR)/ComputeFunction_unittest
	$(TESTDIR)/ForwardRegression_unittest
	$(TESTDIR)/LocalSearch_unittest
	$(TESTDIR)/SmoothedDifferentialEntropy_unittest
	$(TESTDIR)/Evaluation_integration

memcheck: libtsr.so $(TESTS)
	$(MEMCHECK) $(TESTDIR)/LeastSquares_unittest
	$(MEMCHECK) $(TESTDIR)/IDX3Reader_unittest
	$(MEMCHECK) $(TESTDIR)/IDX1Reader_unittest
	$(MEMCHECK) $(TESTDIR)/FileReader_unittest
	$(MEMCHECK) $(TESTDIR)/DataGenerator_unittest
	$(MEMCHECK) $(TESTDIR)/UnitL2Normalizer_unittest
	$(MEMCHECK) $(TESTDIR)/Error_unittest
	$(MEMCHECK) $(TESTDIR)/ComputeFunction_unittest
	$(MEMCHECK) $(TESTDIR)/ForwardRegression_unittest
	$(MEMCHECK) $(TESTDIR)/LocalSearch_unittest
	$(MEMCHECK) $(TESTDIR)/SmoothedDifferentialEntropy_unittest
	$(MEMCHECK) $(TESTDIR)/Evaluation_integration

$(TESTDIR)/LeastSquares_unittest: $(UNITSRC)/LeastSquares_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/LeastSquares_unittest.cpp -o $(TESTDIR)/LeastSquares_unittest
$(TESTDIR)/IDX3Reader_unittest: $(UNITSRC)/IDX3Reader_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/IDX3Reader_unittest.cpp -o $(TESTDIR)/IDX3Reader_unittest
$(TESTDIR)/IDX1Reader_unittest: $(UNITSRC)/IDX1Reader_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/IDX1Reader_unittest.cpp -o $(TESTDIR)/IDX1Reader_unittest
$(TESTDIR)/FileReader_unittest: $(UNITSRC)/FileReader_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/FileReader_unittest.cpp -o $(TESTDIR)/FileReader_unittest
$(TESTDIR)/DataGenerator_unittest: $(UNITSRC)/DataGenerator_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/DataGenerator_unittest.cpp -o $(TESTDIR)/DataGenerator_unittest
$(TESTDIR)/UnitL2Normalizer_unittest: $(UNITSRC)/UnitL2Normalizer_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/UnitL2Normalizer_unittest.cpp -o $(TESTDIR)/UnitL2Normalizer_unittest
$(TESTDIR)/Error_unittest: $(UNITSRC)/Error_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/Error_unittest.cpp -o $(TESTDIR)/Error_unittest
$(TESTDIR)/ComputeFunction_unittest: $(UNITSRC)/ComputeFunction_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/ComputeFunction_unittest.cpp -o $(TESTDIR)/ComputeFunction_unittest
$(TESTDIR)/ForwardRegression_unittest: $(UNITSRC)/ForwardRegression_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/ForwardRegression_unittest.cpp -o $(TESTDIR)/ForwardRegression_unittest
$(TESTDIR)/LocalSearch_unittest: $(UNITSRC)/LocalSearch_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/LocalSearch_unittest.cpp -o $(TESTDIR)/LocalSearch_unittest
$(TESTDIR)/SmoothedDifferentialEntropy_unittest: $(UNITSRC)/SmoothedDifferentialEntropy_unittest.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(UNITSRC)/SmoothedDifferentialEntropy_unittest.cpp -o $(TESTDIR)/SmoothedDifferentialEntropy_unittest
$(TESTDIR)/Evaluation_integration: $(INTSRC)/Evaluation_integration.cpp libtsr.so
	g++ $(OPTS) $(INCLUDES) $(LIBS) $(INTSRC)/Evaluation_integration.cpp -o $(TESTDIR)/Evaluation_integration

doc: libtsr.so doc/Doxyfile
	doxygen doc/Doxyfile
clean:
	rm $(OBJECTS) libtsr.so
	rm -rf doc/html
	rm -rf doc/latex
	rm $(TESTDIR)/*
