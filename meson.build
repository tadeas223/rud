project('rud', 'cpp', default_options: ['cpp_std=c++20'])

add_project_arguments([
  '-O3',
  '-march=native'
],language: 'cpp')

sources = []
incl_dirs = include_directories('include', 'src')

cpu_family = host_machine.cpu_family()
cpu = host_machine.cpu()
system = host_machine.system()

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

subdir('benchmark')

executable('main', 'src/main.cpp', dependencies: rud_dep)
