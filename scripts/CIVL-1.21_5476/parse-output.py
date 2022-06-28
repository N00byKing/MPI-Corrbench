import re


# TODO documentation at different location

# input:
# input_dir: the directory where the tool was run and output was captured
# is_error_expected: bool if an error is expected
# error_specification: information read from error-classification.json

# return values:
# error_found count off errors found
# -1 means could not tell (e.g. output is missing or not helpful)
# correct_error_found wether or not exactely the expected error was found
# -1 means could not tell (e.g. output is missing or not helpful or error specification missing) 0 means false, 1 true

#  is_error_expected=True the output correct_error_found is ignored
def parse_output(input_dir, is_error_expected, error_specification):
    error_found = -1
    correct_error_found = -1

    fname = input_dir + "/output.txt"

    try:
        with open(fname, mode='r') as file:
            data = file.read().replace('\n', ' ').lower()

            # Stringchecks from the MPI Bugs Initiative (https://gitlab.com/MpiBugsInitiative/MpiBugsInitiative/)
            if re.search("cannot be invoked without MPI_Init\(\) being called before", data):
                error_found = 1
            if re.search('DEADLOCK', data):
                error_found = 1
            if re.search('MBI_MSG_RACE', data):
                error_found = 1
            if re.search('reaches an MPI collective routine .*? while at least one of others are collectively reaching MPI_', data):
                error_found = 1
            if re.search('which has an inconsistent datatype specification with at least one of others', data):
                error_found = 1
            if re.search('of MPI routines is not consistent with the specified MPI_Datatype', data):
                error_found = 1
            if re.search('which has a different root with at least one of others', data):
                error_found = 1
            if re.search('has a different MPI_Op', data):
                error_found = 1
            if re.search('MPI message leak', data):
                error_found = 1
            if re.search('MEMORY_LEAK', data):
                error_found = 1
            if re.search('The standard properties hold for all executions', data):
                error_found = 0
            if re.search('kind: UNDEFINED_VALUE, certainty: MAYBE', data) or re.search('kind: UNDEFINED_VALUE, certainty: PROVEABLE', data):
                error_found = 1
            if re.search('kind: DEREFERENCE, certainty: MAYBE', data) or re.search('kind: DEREFERENCE, certainty: PROVEABLE', data):
                error_found = 1
            if re.search('kind: MPI_ERROR, certainty: MAYBE', data) or re.search('kind: MPI_ERROR, certainty: PROVEABLE', data):
                error_found = 1

            if re.search('Compilation of .*? raised an error \(retcode: ', data):
                error_found = -1
            if re.search('This feature is not yet implemented', data):
                error_found = -1
            if re.search('doesn.t have a definition', data):
                error_found = -1
            if re.search('Undeclared identifier', data):
                error_found = -1
            if re.search('A CIVL internal error has occurred', data):
                error_found = -1

    except FileNotFoundError:
        print("Error: FileNotFoundError for file %s (ignoring case)" % (fname))

    return error_found, correct_error_found
