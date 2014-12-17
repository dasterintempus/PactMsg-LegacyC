#include "../greatest.h"

extern SUITE(test_suite_pact_string);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(test_suite_pact_string);
	GREATEST_MAIN_END();
}