CC=gcc LD=gcc GL_LIB_PATH=$(pwd)/../../gl OUT_DIR=$(pwd) make vector map_list
CC=gcc LD=gcc GL_LIB_PATH=$(pwd)/../../gl OUT_DIR=$(pwd) make -C tests run_tests
./run_tests


# If you want to check on one particular test case use following command:
# ./run_tests --run_test=BoolsSetGet
