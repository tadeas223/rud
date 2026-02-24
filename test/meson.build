gtest_dep = dependency('gtest', required: true)
gtest_main_dep = dependency('gtest_main', required: true)

test_sources = []
test_incl_dirs = include_directories('.')

subdir('base')

test_exe = executable(
  'rud_test',
  test_sources,
  dependencies: [rud_dep, gtest_dep, gtest_main_dep],
  include_directories: test_incl_dirs
)

test('rud_test', test_exe, args: ['--rng-seed=12345'])
