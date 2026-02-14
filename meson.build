project('rud', 'cpp', default_options: ['cpp_std=c++20'])

sources = []
incl_dirs = include_directories('include', 'src')

subdir('src')

executable('main', sources, include_directories: incl_dirs)
