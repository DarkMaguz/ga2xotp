project('ga2xotp', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

protoc = find_program('protoc')
gen = generator(protoc, \
  output    : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
  arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/src', '--cpp_out=@BUILD_DIR@', '@INPUT@'])
generated = gen.process('src/OtpMigration.proto')

deps = [dependency('protobuf', version : '>=3.21.12'),
        dependency('liburiparser', version : '>=0.9.8'),
        dependency('zbar', version : '>=0.23.93'),
        dependency('Magick++', version : '>=7.1.1'),
        dependency('openssl', version : '>=3.5.5'),
        dependency('libcrypto++', version : '>=8.9'),
        dependency('nlohmann_json', version : '>=3.11.3')]

executable('ga2xotp',
           'src/main.cpp',
           generated,
           'src/utils.cpp',
           dependencies: deps)
