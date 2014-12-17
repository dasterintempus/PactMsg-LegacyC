#include "../greatest.h"
#include "../../pstring.h"
#include <stdlib.h>
#include <string.h>

TEST test_pact_string_assign_simple() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERT_STR_EQm("pact_string_view returned different data", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_assign_nulls() {
	char* str = "This \x00is \x00some \x00test \x00data \x00with \x00nulls.";
	char* check = "This ";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERT_EQm("pact_string_length returned incorrect value", 5, pact_string_length(pstring));
	ASSERT_STR_EQm("pact_string_view returned different data", check, pact_string_view(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_assign_length_simple() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero", pact_string_assign_length(pstring, str, 23) == 0);
	ASSERT_STR_EQm("pact_string_view returned different data", str, pact_string_view(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_assign_length_section() {
	char* str = "This is some test data.";
	char* check = "This is so";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero", pact_string_assign_length(pstring, str, 10) == 0);
	ASSERT_STR_EQm("pact_string_view returned different data", check, pact_string_view(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_assign_length_nulls() {
	char* str = "This \x00is \x00some \x00test \x00data \x00with \x00nulls.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign_length returned non-zero", pact_string_assign_length(pstring, str, 29) == 0);
	ASSERT_EQm("pact_string_length returned incorrect value", 29, pact_string_length(pstring));
	ASSERTm("pact_string_view returned different data", memcmp(pact_string_view(pstring), str, 29) == 0);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_length() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERT_EQm("pact_string_length returned incorrect value", 23, pact_string_length(pstring));
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_clear_length_zero() {
	char* str = "This is some test data.";
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_clear returned non-zero", pact_string_clear(pstring) == 0);
	ASSERTm("pact_string_length (following pact_string_clear) was not 0", pact_string_length(pstring) == 0);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_short() {
	char* str = "This is some test data.";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve returned non-zero", pact_string_retrieve(pstring, str2, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve does not match input", str, str2);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_long() {
	char* str = "0_1_2_3_4_5_6_7_8_9_10_11_12_13_14_15_16_17_18_19_20_21_22_23_24_25_26_27_28_29_30_31_32_33_34_35_36_37_38_39_40_41_42_43_44_45_46_47_48_49_50_51_52_53_54_55_56_57_58_59_60_61_62_63_64_65_66_67_68_69_70_71_72_73_74_75_76_77_78_79_80_81_82_83_84_85_86_87_88_89_90_91_92_93_94_95_96_97_98_99";
	char* check = "0_1_2_3_4_5_6_7_8_9_10_11_12_13_14_15_16_17_18_19_20_21_22_23_24_25_26_27_28_29_30_31_32_33_34_35_36_37_38_39_40_41_42_43_44_45_46_47_48_49_50_51_52_53_54_55_56_57_58_59_60_61_62_63_64_65_66_67_68_69_70_71_72_73_74_75_76_77_78_79_80_81_82_83_84_85_86_87_8";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve returned non-zero", pact_string_retrieve(pstring, str2, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve does not match input", check, str2);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_substr_start() {
	char* str = "This is some test data.";
	char* check = "This is so";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero", pact_string_retrieve_substr(pstring, str2, 0, 10) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input", check, str2);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_substr_start_rest() {
	char* str = "This is some test data.";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero", pact_string_retrieve_substr(pstring, str2, 0, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input", str, str2);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_substr_mid() {
	char* str = "This is some test data.";
	char* check = "is some te";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero", pact_string_retrieve_substr(pstring, str2, 5, 10) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input", check, str2);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_substr_mid_rest() {
	char* str = "This is some test data.";
	char* check = "is some test data.";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero", pact_string_retrieve_substr(pstring, str2, 5, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input", check, str2);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_substr_end() {
	char* str = "This is some test data.";
	char* check = "test data.";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero", pact_string_retrieve_substr(pstring, str2, 13, 10) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input", check, str2);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_substr_end_rest() {
	char* str = "This is some test data.";
	char* check = "test data.";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero", pact_string_retrieve_substr(pstring, str2, 13, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input", check, str2);
	pact_string_destroy(pstring);
	PASS();
}

TEST test_pact_string_retrieve_substr_tail_rest() {
	char* str = "This is some test data.";
	char* check = "st data.";
	char* str2 = malloc(255);
	pact_String* pstring = pact_string_create();
	ASSERTm("pact_string_assign returned non-zero", pact_string_assign(pstring, str) == 0);
	ASSERTm("pact_string_retrieve_substr returned non-zero", pact_string_retrieve_substr(pstring, str2, 15, 255) == 0);
	ASSERT_STR_EQm("out buffer post pact_string_retrieve_substr does not match correct portion of input", check, str2);
	pact_string_destroy(pstring);
	PASS();
}

SUITE(test_suite_pact_string) {
	RUN_TEST(test_pact_string_assign_simple);
	RUN_TEST(test_pact_string_assign_nulls);
	RUN_TEST(test_pact_string_assign_length_simple);
	RUN_TEST(test_pact_string_assign_length_section);
	RUN_TEST(test_pact_string_assign_length_nulls);
	RUN_TEST(test_pact_string_length);
	RUN_TEST(test_pact_string_clear_length_zero);
	RUN_TEST(test_pact_string_retrieve_short);
	RUN_TEST(test_pact_string_retrieve_long);
	RUN_TEST(test_pact_string_retrieve_substr_start);
	RUN_TEST(test_pact_string_retrieve_substr_start_rest);
	RUN_TEST(test_pact_string_retrieve_substr_mid);
	RUN_TEST(test_pact_string_retrieve_substr_mid_rest);
	RUN_TEST(test_pact_string_retrieve_substr_end);
	RUN_TEST(test_pact_string_retrieve_substr_end_rest);
	RUN_TEST(test_pact_string_retrieve_substr_tail_rest);
}
