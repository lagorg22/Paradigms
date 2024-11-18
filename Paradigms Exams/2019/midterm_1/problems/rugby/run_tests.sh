export CFLAGS="$CFLAGS -Wall -std=c99"

# Set up GL lib paths
if [[ -z "${GL_LIB_PATH}" ]]; then
    GL_LIB_PATH="~/dev/src/gl"
fi
export GL_LIB_PATH
export CFLAGS="$CFLAGS -I$GL_LIB_PATH"
echo "GL lib path: $GL_LIB_PATH"
echo "CFLAGS: $CFLAGS"

# Clean and build GL lib
echo "-- Building GL lib"
make -C $GL_LIB_PATH/lunit clean lunit

# Clean
make clean
make -C tests clean

# packed_list
make rugby
make -C tests run_tests
./tests/run_tests

