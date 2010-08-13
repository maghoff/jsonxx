APPNAME='jsonxx'
VERSION='0.0.0'
blddir='build'
srcdir="."

def set_options(opt):
    opt.tool_options('compiler_cxx')

def configure(conf):
    conf.check_tool('compiler_cxx')


def build(bld):
    bld.add_subdirs('src')
    bld.add_subdirs('tests')

def check(context):
    import UnitTest
    ut = UnitTest.unit_test()
    ut.run()
    ut.print_results()
