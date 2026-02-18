gtest_dep = dependency('gtest', required: true)
gtest_main_dep = dependency('gtest_main')

test_sources = []

subdir('ds')
subdir('base')

test_exe = executable(
  'rud_test',
  test_sources,
  dependencies: [rud_dep, gtest_dep, gtest_main_dep]
)

test('rud_test', test_exe)
