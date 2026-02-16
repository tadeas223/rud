project('rud', 'cpp', default_options: ['cpp_std=c++20'])

sources = []
incl_dirs = include_directories('include', 'src')

subdir('src')

rud_lib = library('rud',
  sources,
  include_directories: incl_dirs,
  install : true
)

rud_dep = declare_dependency(
  link_with: rud_lib,
  include_directories: incl_dirs
)

meson.override_dependency('rud', rud_dep)

executable('main', 'src/main.cpp', dependencies: rud_dep)
