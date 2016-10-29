tests:
	g++ test/Runner.cpp \
		test/QueryTest.cpp    src/Query.cpp \
		test/CacheTest.cpp    src/Cache.cpp \
		test/ResponseTest.cpp src/Response.cpp \
		mocks/Udp.cpp mocks/IPAddress.cpp \
		-I ./src \
		-I ./mocks \
		-o test/runner
	test/runner
	rm test/runner
