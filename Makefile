tests:
	g++ test/QueryTest.cpp src/Query.cpp -o test/runner
	test/runner
	rm test/runner