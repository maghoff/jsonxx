#!/bin/env python
# -*- coding: utf-8 -*-

APPNAME='jsonxx'
VERSION='0.0.0'
blddir='build'
srcdir="."

def set_options(opt):
    opt.tool_options('compiler_cc')
    opt.tool_options('compiler_cxx')


class gcc_configurator:
    @staticmethod
    def sane_default(env):
        pass

    @staticmethod
    def debug_mode(env):
        env.append_unique('CXXFLAGS', '-ggdb')
        env.append_unique('CXXDEFINES', 'DEBUG')
        env.append_unique('CCFLAGS', '-ggdb')
        env.append_unique('CCDEFINES', 'DEBUG')

    @staticmethod
    def release_mode(env):
        env.append_unique('CXXDEFINES', 'NDEBUG')
        env.append_unique('CCDEFINES', 'NDEBUG')
        env.append_unique('LINKFLAGS', '-s')

    @staticmethod
    def optimize(env):
        env.append_unique('CXXFLAGS', '-O3')
        env.append_unique('CCFLAGS', '-O3')

    @staticmethod
    def many_warnings(env):
        env.append_unique('CXXFLAGS', '-Wall')
        env.append_unique('CCFLAGS', '-Wall')
        env.append_unique('LINKFLAGS', '-Wall')

    @staticmethod
    def warnings_as_errors(env):
        env.append_unique('CXXFLAGS', '-Werror')
        env.append_unique('CCFLAGS', '-Werror')
        env.append_unique('LINKFLAGS', '-Werror')

    @staticmethod
    def link_time_code_generation(env):
        # This is available in GCC 4.5, and is called LTO.

        # WHOPR, which is also available in GCC 4.5, is a more scalable
        # alternative, but it optimizes less

        pass


class msvc_configurator:
    @staticmethod
    def sane_default(env):
        env.append_unique('CXXFLAGS', '/GR') # Enable RTTI
        env.append_unique('CXXFLAGS', '/GS') # Buffer Security Check

    @staticmethod
    def debug_mode(env):
        env.append_unique('CXXFLAGS', '/MDd')
        env.append_unique('CXXFLAGS', '/Od')
        env.append_unique('CXXFLAGS', '/Zi')
        env.append_unique('CXXFLAGS', '/RTC1')
        env.append_unique('LINKFLAGS', '/DEBUG')
        env.append_unique('CXXDEFINES', 'DEBUG')
        env.append_unique('CCDEFINES', 'DEBUG')

    @staticmethod
    def release_mode(env):
        env.append_unique('CXXFLAGS', '/MD')
        env.append_unique('CXXDEFINES', 'NDEBUG')
        env.append_unique('CCDEFINES', 'NDEBUG')

    @staticmethod
    def optimize(env):
        env.append_unique('CXXFLAGS', '/O2')
        env.append_unique('CCFLAGS', '/O2')

    @staticmethod
    def many_warnings(env):
        pass

    @staticmethod
    def warnings_as_errors(env):
        env.append_unique('CXXFLAGS', '/WX')
        env.append_unique('CCFLAGS', '/WX')
        env.append_unique('LINKFLAGS', '/WX')

    @staticmethod
    def link_time_code_generation(env):
        env.append_unique('CXXFLAGS', '/GL')
        env.append_unique('CCFLAGS', '/GL')
        env.append_unique('LINKFLAGS', '/LTCG')


def configure(conf):
    # libjson is a necessary dependency:
    import subprocess, sys
    subprocess.check_call([sys.executable, 'get_libjson.py'], cwd='import')

    conf.env['MSVC_VERSIONS'] = ['msvc 9.0']
    conf.env['MSVC_TARGETS'] = ['x86']

    conf.check_tool('compiler_cc')
    conf.check_tool('compiler_cxx')

    compiler_configurators = {
        'gcc': gcc_configurator,
        'msvc': msvc_configurator,
    }

    cc = compiler_configurators[conf.env['CXX_NAME']]

    cc.sane_default(conf.env)

    cc.many_warnings(conf.env)
    cc.warnings_as_errors(conf.env)

    rel_env = conf.env.copy()
    rel_env.set_variant('release')
    conf.set_env_name('default', rel_env)
    cc.release_mode(rel_env)
    cc.optimize(rel_env)
    cc.link_time_code_generation(rel_env)

    deb_env = conf.env.copy()
    deb_env.set_variant('debug')
    conf.set_env_name('debug', deb_env)
    cc.debug_mode(deb_env)


def build(bld):
    bld.add_subdirs('src')
    bld.add_subdirs('tests')

def check(context):
    import UnitTest
    ut = UnitTest.unit_test()
    ut.run()
    ut.print_results()
