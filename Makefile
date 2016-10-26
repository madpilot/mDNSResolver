tests:
	g++ test/QueryTest.cpp src/Query.cpp mocks/Udp.cpp -o test/runner
	test/runner
	rm test/runner