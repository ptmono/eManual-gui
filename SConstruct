import os
os.environ['QT4DIR'] = '/usr/lib64/qt4'

env = Environment()
env.Tool('qt4')
env.EnableQt4Modules(['QtCore', 'QtGui', 'QtTest', 'QtSql', 'QtWebKit'])
env.Append(LIBS=['boost_program_options', 'boost_filesystem',
                 'boost_system'])
env.Append(CPPFLAGS=['-Wall'])

Export('env')


### Base sources
sources_base = [
    'build/main.cpp',
    'build/mainwindow.cpp',
    'build/emanualcmd.cpp',
    'build/models.cpp',
    'build/libs.cpp',
    'build/config.cpp',

    'build/dialogs.cpp'
    ]

### uics
uics = [
    'src/createElementDlg.ui',
    'src/createEventDlg.ui',
    'src/createGroupDlg.ui',
    'src/inputDlg.ui'
    ]

# Generate code and header files from ui files
for ui in uics:
    env.Uic4(ui)

### For qjson
dir_qjson = 'tools/qjson/src/'
sources_qjson = [
    'json_parser.cc',
    'json_scanner.cpp',
    'parser.cpp',
    'parserrunnable.cpp',
    'qobjecthelper.cpp',
    'serializer.cpp',
    'serializerrunnable.cpp'
    ]
sources_qjson = map((lambda a : dir_qjson + a), sources_qjson)
objects_qjson = env.Object(sources_qjson)

### For qdjango
dir_qdjango = 'tools/qdjango/src/db/'
sources_qdjango = [
    'QDjango.cpp',
    'QDjangoModel.cpp',
    'QDjangoMetaModel.cpp',
    'QDjangoQuerySet.cpp',
    'QDjangoWhere.cpp'
    ]

sources_qdjango = map((lambda a : dir_qdjango + a), sources_qdjango)
sources_qdjango.append(env.Moc4(dir_qdjango + 'QDjango_p.h'))
sources_qdjango.append(env.Moc4(dir_qdjango + 'QDjangoQuerySet_p.h'))
sources_qdjango.append(env.Moc4(dir_qdjango + 'QDjangoWhere_p.h'))
objects_qdjango = env.Object(sources_qdjango)


### For testing
dir_test = 'tests/'
sources_test = [
    "common.cpp",
    "test_config.cpp",
    #"test_emanualcmd.cpp",
    "test_models.cpp",
    #"prepare_config.cpp",
    #"test_dialogs.cpp",
    #"test_libs.cpp"
    ]
sources_test = map((lambda a : dir_test + a), sources_test)
objects_test = env.Object(sources_test)


### header directories
env['CPPPATH'] = env['CPPPATH'] + [dir_qjson] + [dir_test] +\
    [dir_qdjango]

sources = sources_base + objects_qjson + objects_qdjango + objects_test
env.VariantDir('build', 'src', duplicate=0)
env.Program(target = 'build/Test', source = sources)

