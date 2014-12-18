#include "../greatest.h"
#include "../../pstring.h"
#include <stdlib.h>
#include <string.h>

TEST test_pact_string_assign_simple() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERT_STR_EQm("pact_string_view returned different data\n", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_assign_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	char* check = "This ";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERT_EQm("pact_string_length returned incorrect value\n", 5, pact_string_length(pstring));
	ASSERT_STR_EQm("pact_string_view returned different data\n", check, pact_string_view(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_assign_length_simple() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 23) == 0);
	ASSERT_STR_EQm("pact_string_view returned different data\n", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_assign_length_section() {
	char* str = "This is some test data.";
	char* check = "This is so";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 10) == 0);
	ASSERT_STR_EQm("pact_string_view returned different data\n", check, pact_string_view(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_assign_length_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 29) == 0);
	ASSERT_EQm("pact_string_length returned incorrect value\n", 29, pact_string_length(pstring));
	ASSERTm("pact_string_view returned different data\n", memcmp(pact_string_view(pstring), str, 29) == 0);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_length() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERT_EQm("pact_string_length returned incorrect value\n", 23, pact_string_length(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_length_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERT_EQm("pact_string_length returned incorrect value\n", 40, pact_string_length(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_clear_length_zero() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_clear returned non-zero\n", pact_string_clear(pstring) == 0);
	ASSERTm("pact_string_length (following pact_string_clear) was not 0\n", pact_string_length(pstring) == 0);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_short() {
	char* str = "This is some test data.";
	char* str2 = malloc(256);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve returned non-zero\n", pact_string_retrieve(pstring, str2, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve does not match input\n", str, str2);
	pact_string_destroy(pstring);
	free(str2);
	PASS();
}

TEST test_pact_string_retrieve_long() {
	char* str = "0_1_2_3_4_5_6_7_8_9_10_11_12_13_14_15_16_17_18_19_20_21_22_23_24_25_26_27_28_29_30_31_32_33_34_35_36_37_38_39_40_41_42_43_44_45_46_47_48_49_50_51_52_53_54_55_56_57_58_59_60_61_62_63_64_65_66_67_68_69_70_71_72_73_74_75_76_77_78_79_80_81_82_83_84_85_86_87_88_89_90_91_92_93_94_95_96_97_98_99";
	char* check = "0_1_2_3_4_5_6_7_8_9_10_11_12_13_14_15_16_17_18_19_20_21_22_23_24_25_26_27_28_29_30_31_32_33_34_35_36_37_38_39_40_41_42_43_44_45_46_47_48_49_50_51_52_53_54_55_56_57_58_59_60_61_62_63_64_65_66_67_68_69_70_71_72_73_74_75_76_77_78_79_80_81_82_83_84_85_86_87_8";
	char* str2 = malloc(256);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve returned non-zero\n", pact_string_retrieve(pstring, str2, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve does not match input\n", check, str2);
	pact_string_destroy(pstring);
	free(str2);
	PASS();
}

TEST test_pact_string_retrieve_substr_start() {
	char* str = "This is some test data.";
	char* check = "This is so";
	char* str2 = malloc(256);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero\n", pact_string_retrieve_substr(pstring, str2, 0, 10) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input\n", check, str2);
	pact_string_destroy(pstring);
	free(str2);
	PASS();
}

TEST test_pact_string_retrieve_substr_start_rest() {
	char* str = "This is some test data.";
	char* str2 = malloc(256);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero\n", pact_string_retrieve_substr(pstring, str2, 0, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input\n", str, str2);
	pact_string_destroy(pstring);
	free(str2);
	PASS();
}

TEST test_pact_string_retrieve_substr_mid() {
	char* str = "This is some test data.";
	char* check = "is some te";
	char* str2 = malloc(256);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero\n", pact_string_retrieve_substr(pstring, str2, 5, 10) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input\n", check, str2);
	pact_string_destroy(pstring);
	free(str2);
	PASS();
}

TEST test_pact_string_retrieve_substr_mid_rest() {
	char* str = "This is some test data.";
	char* check = "is some test data.";
	char* str2 = malloc(256);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero\n", pact_string_retrieve_substr(pstring, str2, 5, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input\n", check, str2);
	pact_string_destroy(pstring);
	free(str2);
	PASS();
}

TEST test_pact_string_retrieve_substr_end() {
	char* str = "This is some test data.";
	char* check = "test data.";
	char* str2 = malloc(256);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero\n", pact_string_retrieve_substr(pstring, str2, 13, 10) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input\n", check, str2);
	pact_string_destroy(pstring);
	free(str2);
	PASS();
}

TEST test_pact_string_retrieve_substr_end_rest() {
	char* str = "This is some test data.";
	char* check = "test data.";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero\n", pact_string_retrieve_substr(pstring, str2, 13, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input\n", check, str2);
	pact_string_destroy(pstring);
	free(str2);
	PASS();
}

TEST test_pact_string_compare_to_higher() {
	char* str = "B";
	char* str2 = "C";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, str2) == 0);
	ASSERT_EQm("pact_string_compare returned wrong value\n", -1, pact_string_compare(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_compare_to_lower() {
	char* str = "B";
	char* str2 = "A";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, str2) == 0);
	ASSERT_EQm("pact_string_compare returned wrong value\n", 1, pact_string_compare(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_compare_to_equal() {
	char* str = "B";
	char* str2 = "B";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, str2) == 0);
	ASSERT_EQm("pact_string_compare returned wrong value\n", 0, pact_string_compare(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_compare_to_longer() {
	char* str = "B";
	char* str2 = "BB";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, str2) == 0);
	ASSERT_EQm("pact_string_compare returned wrong value\n", -1, pact_string_compare(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_compare_to_shorter() {
	char* str = "BB";
	char* str2 = "B";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, str2) == 0);
	ASSERT_EQm("pact_string_compare returned wrong value\n", 1, pact_string_compare(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_compare_both_empty() {
	char* str = "";
	char* str2 = "";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, str2) == 0);
	ASSERT_EQm("pact_string_compare returned wrong value\n", 0, pact_string_compare(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_compare_one_empty() {
	char* str = "B";
	char* str2 = "";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, str2) == 0);
	ASSERT_EQm("pact_string_compare returned wrong value\n", 1, pact_string_compare(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_compare_equal_nulls() {
	char* str = "B\0";
	char* str2 = "B\0";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 2) == 0);
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring2, str2, 2) == 0);
	ASSERT_EQm("pact_string_compare returned wrong value\n", 0, pact_string_compare(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_compare_different_nulls() {
	char* str = "B\0C";
	char* str2 = "B\0B";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 3) == 0);
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring2, str2, 3) == 0);
	ASSERT_EQm("pact_string_compare returned wrong value\n", 1, pact_string_compare(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_compare_cstr_to_higher() {
	char* str = "B";
	char* str2 = "C";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERT_EQm("pact_string_compare_cstr returned wrong value\n", -1, pact_string_compare_cstr(pstring, str2));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_compare_cstr_to_lower() {
	char* str = "B";
	char* str2 = "A";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERT_EQm("pact_string_compare_cstr returned wrong value\n", 1, pact_string_compare_cstr(pstring, str2));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_compare_cstr_to_equal() {
	char* str = "B";
	char* str2 = "B";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERT_EQm("pact_string_compare_cstr returned wrong value\n", 0, pact_string_compare_cstr(pstring, str2));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_compare_cstr_both_empty() {
	char* str = "";
	char* str2 = "";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERT_EQm("pact_string_compare_cstr returned wrong value\n", 0, pact_string_compare_cstr(pstring, str2));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_compare_cstr_one_empty() {
	char* str = "B";
	char* str2 = "";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERT_EQm("pact_string_compare_cstr returned wrong value\n", 1, pact_string_compare_cstr(pstring, str2));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_clone_simple() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_clone returned non-zero\n", pact_string_clone(pstring, pstring2) == 0);
	ASSERT_EQm("after pact_string_clone, length mismatch\n", pact_string_length(pstring), pact_string_length(pstring2));
	ASSERT_EQm("after pact_string_clone, length on dst incorrect\n", 23, pact_string_length(pstring2));
	ASSERT_EQm("after pact_string_clone, length on src incorrect\n", 23, pact_string_length(pstring));
	ASSERT_STR_EQm("after pact_string_clone, dst has incorrect value\n", str, pact_string_view(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, src has incorrect value\n", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_clone_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERTm("pact_string_clone returned non-zero\n", pact_string_clone(pstring, pstring2) == 0);
	ASSERT_EQm("after pact_string_clone, length mismatch\n", pact_string_length(pstring), pact_string_length(pstring2));
	ASSERT_EQm("after pact_string_clone, length on dst incorrect\n", 40, pact_string_length(pstring2));
	ASSERT_EQm("after pact_string_clone, length on src incorrect\n", 40, pact_string_length(pstring));
	ASSERTm("after pact_string_clone, dst has incorrect value\n", memcmp(str, pact_string_view(pstring2), 29) == 0);
	ASSERTm("after pact_string_clone, src has incorrect value\n", memcmp(str, pact_string_view(pstring), 29) == 0);
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_clone_substr_start() {
	char* str = "This is some test data.";
	char* check = "This is so";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_clone returned non-zero\n", pact_string_clone_substr(pstring, pstring2, 0, 10) == 0);
	ASSERT_EQm("after pact_string_clone, length on dst incorrect\n", 10, pact_string_length(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, dst has incorrect value\n", check, pact_string_view(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, src has incorrect value\n", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_clone_substr_start_rest() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_clone returned non-zero\n", pact_string_clone_substr(pstring, pstring2, 0, 255) == 0);
	ASSERT_EQm("after pact_string_clone, length on dst incorrect\n", 23, pact_string_length(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, dst has incorrect value\n", str, pact_string_view(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, src has incorrect value\n", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_clone_substr_mid() {
	char* str = "This is some test data.";
	char* check = "is some te";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_clone returned non-zero\n", pact_string_clone_substr(pstring, pstring2, 5, 10) == 0);
	ASSERT_EQm("after pact_string_clone, length on dst incorrect\n", 10, pact_string_length(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, dst has incorrect value\n", check, pact_string_view(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, src has incorrect value\n", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_clone_substr_mid_rest() {
	char* str = "This is some test data.";
	char* check = "is some test data.";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_clone returned non-zero\n", pact_string_clone_substr(pstring, pstring2, 5, 255) == 0);
	ASSERT_EQm("after pact_string_clone, length on dst incorrect\n", 18, pact_string_length(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, dst has incorrect value\n", check, pact_string_view(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, src has incorrect value\n", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_clone_substr_end() {
	char* str = "This is some test data.";
	char* check = "test data.";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_clone returned non-zero\n", pact_string_clone_substr(pstring, pstring2, 13, 10) == 0);
	ASSERT_EQm("after pact_string_clone, length on dst incorrect\n", 10, pact_string_length(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, dst has incorrect value\n", check, pact_string_view(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, src has incorrect value\n", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_clone_substr_end_rest() {
	char* str = "This is some test data.";
	char* check = "test data.";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_clone returned non-zero\n", pact_string_clone_substr(pstring, pstring2, 13, 255) == 0);
	ASSERT_EQm("after pact_string_clone, length on dst incorrect\n", 10, pact_string_length(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, dst has incorrect value\n", check, pact_string_view(pstring2));
	ASSERT_STR_EQm("after pact_string_clone, src has incorrect value\n", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_start() {
	char* str = "This is some test data.";
	char* search = "This";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, search) == 0);
	ASSERT_EQm("pact_string_find returned incorrect match position\n", 0, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_mid() {
	char* str = "This is some test data.";
	char* search = "some";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, search) == 0);
	ASSERT_EQm("pact_string_find returned incorrect match position\n", 8, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_end() {
	char* str = "This is some test data.";
	char* search = "ta.";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, search) == 0);
	ASSERT_EQm("pact_string_find returned incorrect match position\n", 20, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_noresult() {
	char* str = "This is some test data.";
	char* search = "bgak";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, search) == 0);
	ASSERT_EQm("pact_string_find returned a match incorrectly\n", -1, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_start_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	char* search = "This";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, search) == 0);
	ASSERT_EQm("pact_string_find returned incorrect match position\n", 0, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_start_both_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	char* search = "This \0";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring2, search, 6) == 0);
	ASSERT_EQm("pact_string_find returned incorrect match position\n", 0, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_mid_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	char* search = "some";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, search) == 0);
	ASSERT_EQm("pact_string_find returned incorrect match position\n", 10, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_mid_both_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	char* search = "\0some \0";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring2, search, 7) == 0);
	ASSERT_EQm("pact_string_find returned incorrect match position\n", 9, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_end_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	char* search = "nulls.";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, search) == 0);
	ASSERT_EQm("pact_string_find returned incorrect match position\n", 34, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_end_both_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	char* search = "\0nulls.";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERTm("pact_string_assign_length returned non-zero\n", pact_string_assign_length(pstring2, search, 7) == 0);
	ASSERT_EQm("pact_string_find returned incorrect match position\n", 33, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_noresult_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	char* search = "bgak";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign(pstring2, search) == 0);
	ASSERT_EQm("pact_string_find returned a match incorrectly\n", -1, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}

TEST test_pact_string_find_noresult_both_nulls() {
	char* str = "This \0is \0some \0test \0data \0with \0nulls.";
	char* search = "\0bgak";
	pact_String* pstring = pact_string_create();
	pact_String* pstring2 = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign_length(pstring, str, 40) == 0);
	ASSERTm("pact_string_assign returned non-zero\n", pact_string_assign_length(pstring2, search, 5) == 0);
	ASSERT_EQm("pact_string_find returned a match incorrectly\n", -1, pact_string_find(pstring, pstring2));
	pact_string_destroy(pstring);
	pact_string_destroy(pstring2);
	PASS();
}


SUITE(test_suite_pact_string) {
	RUN_TEST(test_pact_string_assign_simple);
	RUN_TEST(test_pact_string_assign_nulls);
	RUN_TEST(test_pact_string_assign_length_simple);
	RUN_TEST(test_pact_string_assign_length_section);
	RUN_TEST(test_pact_string_assign_length_nulls);
	RUN_TEST(test_pact_string_length);
	RUN_TEST(test_pact_string_length_nulls);
	RUN_TEST(test_pact_string_clear_length_zero);
	RUN_TEST(test_pact_string_retrieve_short);
	RUN_TEST(test_pact_string_retrieve_long);
	RUN_TEST(test_pact_string_retrieve_substr_start);
	RUN_TEST(test_pact_string_retrieve_substr_start_rest);
	RUN_TEST(test_pact_string_retrieve_substr_mid);
	RUN_TEST(test_pact_string_retrieve_substr_mid_rest);
	RUN_TEST(test_pact_string_retrieve_substr_end);
	RUN_TEST(test_pact_string_retrieve_substr_end_rest);
	RUN_TEST(test_pact_string_compare_to_higher);
	RUN_TEST(test_pact_string_compare_to_lower);
	RUN_TEST(test_pact_string_compare_to_equal);
	RUN_TEST(test_pact_string_compare_to_longer);
	RUN_TEST(test_pact_string_compare_to_shorter);
	RUN_TEST(test_pact_string_compare_both_empty);
	RUN_TEST(test_pact_string_compare_one_empty);
	RUN_TEST(test_pact_string_compare_equal_nulls);
	RUN_TEST(test_pact_string_compare_cstr_to_higher);
	RUN_TEST(test_pact_string_compare_cstr_to_lower);
	RUN_TEST(test_pact_string_compare_cstr_to_equal);
	RUN_TEST(test_pact_string_compare_cstr_both_empty);
	RUN_TEST(test_pact_string_compare_cstr_one_empty);
	RUN_TEST(test_pact_string_clone_simple);
	RUN_TEST(test_pact_string_clone_nulls);
	RUN_TEST(test_pact_string_clone_substr_start);
	RUN_TEST(test_pact_string_clone_substr_start_rest);
	RUN_TEST(test_pact_string_clone_substr_mid);
	RUN_TEST(test_pact_string_clone_substr_mid_rest);
	RUN_TEST(test_pact_string_clone_substr_end);
	RUN_TEST(test_pact_string_clone_substr_end_rest);
	RUN_TEST(test_pact_string_find_start);
	RUN_TEST(test_pact_string_find_mid);
	RUN_TEST(test_pact_string_find_end);
	RUN_TEST(test_pact_string_find_noresult);
	RUN_TEST(test_pact_string_find_start_nulls);
	RUN_TEST(test_pact_string_find_start_both_nulls);
	RUN_TEST(test_pact_string_find_mid_nulls);
	RUN_TEST(test_pact_string_find_mid_both_nulls);
	RUN_TEST(test_pact_string_find_end_nulls);
	RUN_TEST(test_pact_string_find_end_both_nulls);
	RUN_TEST(test_pact_string_find_noresult_nulls);
	RUN_TEST(test_pact_string_find_noresult_both_nulls);
}
