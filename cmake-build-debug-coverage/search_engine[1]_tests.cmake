add_test([=[sample_test_case.sample_test]=]  [==[E:/Programs/Git Projects/search_engine/cmake-build-debug-coverage/search_engine.exe]==] [==[--gtest_filter=sample_test_case.sample_test]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[sample_test_case.sample_test]=]  PROPERTIES WORKING_DIRECTORY [==[E:/Programs/Git Projects/search_engine/cmake-build-debug-coverage]==] SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  search_engine_TESTS sample_test_case.sample_test)
