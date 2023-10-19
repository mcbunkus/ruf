project(
  'ruf', 
  'cpp',
  version: '0.1', 
  default_options: ['cpp_std=c++17']
)

cmake = import('cmake')

fmt = cmake.subproject('fmt')
cxxopts = cmake.subproject('cxxopts')

deps = [
  fmt.dependency('fmt'),
  cxxopts.dependency('cxxopts')
]

src = files(
  'src/finder.cpp',
  'src/main.cpp',
  'src/prompt.cpp',
  'src/ruf.cpp'
)


executable(meson.project_name(), src, dependencies: deps)
