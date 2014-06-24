
# Pattern Recognition Library Tests

## Working Directory for Test Execution

By default, the working directory for running the tests from Eclipse is:

    $workspace_loc/project_name

So, tests (such as the ones for checking the quote reading code) read files reference the data starting with this working directory. This setting is configured in Eclipse's launch configuration for the project.

## Running Tests from the Command Line

To run the tests from the command line, the test executable must be run from this same working directory used by Eclipse; e.g.:

    % ./Debug/PatternRecognitionLibTest