build_tests:
	g++ -g -std=c++0x \
		test/Runner.cpp \
		test/QueryTest.cpp        src/Query.cpp \
		test/CacheTest.cpp        src/Cache.cpp \
		test/ResponseTest.cpp     src/Response.cpp \
		test/AnswerTest.cpp       src/Answer.cpp \
		test/mDNSResolverTest.cpp src/mDNSResolver.cpp \
		mocks/UDP.cpp mocks/IPAddress.cpp mocks/millis.c \
		-I src \
		-I mocks \
		-D TEST \
		-o test/runner

clean_tests:
	rm test/runner

debug_tests:
	make build_tests
	gdb test/runner

tests:
	make build_tests
	test/runner
	make clean_tests

ci:
	make build_tests
	test/runner
