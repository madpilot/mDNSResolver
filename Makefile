tests:
	g++ test/Runner.cpp test/QueryTest.cpp src/Query.cpp test/CacheTest.cpp src/Cache.cpp src/Response.cpp mocks/Udp.cpp -o test/runner
	test/runner
	rm test/runner
