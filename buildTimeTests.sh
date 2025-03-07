if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <test_file.cpp>"
    exit 1
fi

# Construct the path for the test file (assumed to be in the 'test' directory)
TEST_FILE="test/$1"

# Verify the test file exists in the test directory
if [ ! -f "$TEST_FILE" ]; then
    echo "Error: Test file '$TEST_FILE' not found in the test directory."
    exit 1
fi

# Extract the base name (without the .cpp extension) for naming the executable
BASE_NAME=$(basename "$TEST_FILE" .cpp)

# Create the bin directory if it doesn't already exist
BIN_DIR="bin"
mkdir -p "$BIN_DIR"

# Build the command with your desired compiler flags
BUILD_CMD="g++ -std=c++11 -fopenmp -Wall -Wextra -Werror -pedantic -O3 $TEST_FILE -o $BIN_DIR/$BASE_NAME"

echo "Compiling $TEST_FILE..."
echo "$BUILD_CMD"
$BUILD_CMD
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Run the binary and time its execution
echo "Running $BIN_DIR/$BASE_NAME..."
time "$BIN_DIR/$BASE_NAME"