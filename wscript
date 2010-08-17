#!/bin/env python
# -*- coding: utf-8 -*-

APPNAME='jsonxx'
VERSION='0.0.0'
blddir='build'
srcdir="."

def set_options(opt):
    opt.tool_options('compiler_cxx')


class gcc_configurator:
    @staticmethod
    def debug_mode(env):
        env.append_unique('CXXFLAGS', '-ggdb')
        env.append_unique('CXXDEFINES', 'DEBUG')

    @staticmethod
    def release_mode(env):
        env.append_unique('CXXDEFINES', 'NDEBUG')
        env.append_unique('LINKFLAGS', '-s')

    @staticmethod
    def optimize(env):
        env.append_unique('CXXFLAGS', '-O3')

    @staticmethod
    def many_warnings(env):
        env.append_unique('CXXFLAGS', '-Wall')
        env.append_unique('LINKFLAGS', '-Wall')

    @staticmethod
    def warnings_as_errors(env):
        env.append_unique('CXXFLAGS', '-Werror')
        env.append_unique('LINKFLAGS', '-Werror')


class msvc_configurator:
    @staticmethod
    def debug_mode(env):
        env.append_unique('CXXDEFINES', 'DEBUG')

    @staticmethod
    def release_mode(env):
        env.append_unique('CXXDEFINES', 'NDEBUG')

    @staticmethod
    def optimize(env):
        pass

    @staticmethod
    def many_warnings(env):
        pass

    @staticmethod
    def warnings_as_errors(env):
        pass


def configure(conf):
    conf.check_tool('compiler_cxx')

    compiler_configurators = {
        'gcc': gcc_configurator,
        'msvc': msvc_configurator,
    }

    cc = compiler_configurators[conf.env['CXX_NAME']]

    cc.many_warnings(conf.env)
    cc.warnings_as_errors(conf.env)

    rel_env = conf.env.copy()
    rel_env.set_variant('release')
    conf.set_env_name('default', rel_env)
    cc.release_mode(rel_env)
    cc.optimize(rel_env)

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
