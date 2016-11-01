build_tests:
	g++ -g -std=c++11 \
		test/Runner.cpp \
		test/QueryTest.cpp    src/Query.cpp \
		test/CacheTest.cpp    src/Cache.cpp \
		test/ResponseTest.cpp src/Response.cpp \
		test/AnswerTest.cpp   src/Answer.cpp \
		mocks/UDP.cpp mocks/IPAddress.cpp \
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
