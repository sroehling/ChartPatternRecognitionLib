
# Pattern Recognition Library Tests

## Running Tests from the Command Line

To run the tests from the command line, the test executable must be run from the test project's root directory
(i.e., the directory/folder with the ".pro" project file)

On Windows, the DLL path for Boost and Qt Libraries also needs to first be set as follows in the terminal:

    set PATH=C:\Qt\5.3\mingw482_32\bin;C:\boost_1_56_0\stage\lib;%PATH%

This will ensure the Boost and Qt libraries will be found by the executable.

## Running the Entire Test Suite

The following will run the entire test suite and generate a summary report at the end.

	$ ../build-TestPatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Debug/TestPatternRecognitionLib --report_level=detailed

## Running a Single Test

The example below runs a single test.

    $ ../build-TestPatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Debug/TestPatternRecognitionLib --log_level=all --run_test=PeriodValTest_YahooCSVValid

## Filtering Test Output for Errors

Filtering for failure output is accomplished by re-directing stderr to stdout, then grepping for failure strings.
The given grep command will do a case-insensitive search for each of the words combined with the -e arguments.

    $ ../build-TestPatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Debug/TestPatternRecognitionLib --report_level=detailed 2>&1 | grep -i -e error -e fail -e abort
