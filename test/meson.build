catch2_dep = dependency('catch2', required: true)
catch2_with_main_dep = dependency('catch2-with-main', required: true)

test_sources = []
test_incl_dirs = include_directories('.')

subdir('base')

test_exe = executable(
  'rud_test',
  test_sources,
  dependencies: [rud_dep, catch2_dep, catch2_with_main_dep],
  include_directories: test_incl_dirs
)

test('rud_test', test_exe, args: ['--rng-seed=12345'])
