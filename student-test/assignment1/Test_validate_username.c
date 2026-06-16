#include "unity.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../../examples/autotest-validate/autotest-validate.h"
#include "../../assignment-autotest/test/assignment1/username-from-conf-file.h"

/**
 * This function:
 *   1) Calls my_username() to get the hard-coded username.
 *   2) Reads the username from conf/username.txt.
 *   3) Verifies that both strings are identical.
 */
void test_validate_my_username()
{
    char *conf_username = malloc_username_from_conf_file();

    TEST_ASSERT_EQUAL_STRING_MESSAGE(
        my_username(),
        conf_username,
        "Username in conf/username.txt does not match my_username()"
    );

    free(conf_username);
}