EnsureSConsVersion( 2, 3, 0 )

qpsnrVersionMajor = "0"
qpsnrVersionMinor = "2"
qpsnrVersionMicro = "1"

qpsnrVersion = [
    qpsnrVersionMajor,
    qpsnrVersionMinor,
    qpsnrVersionMicro ]

qpsnrVersionStr = ".".join( qpsnrVersion )

buildMode = ARGUMENTS.get( 'mode', 'release' )
if not ( buildMode in [ 'debug', 'release' ] ) :
    raise Exception( "Can't select build mode ['debug', 'release']" )

env = Environment()
env.Append(
    CPPPATH = [
        '/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7'
    ]
)

envPy = env.Clone()

conf = Configure(env)

if not conf.CheckLibWithHeader('avutil', 'libavutil/avutil.h', 'c'):
    print 'Did not find libavutil.a or avutil.lib, exiting!'
    Exit(1)

if not conf.CheckLibWithHeader('avcodec', 'libavcodec/avcodec.h', 'c'):
    print 'Did not find libavcodec.a or avcodec.lib, exiting!'
    Exit(1)

if not conf.CheckLibWithHeader('avformat', 'libavformat/avformat.h', 'c'):
    print 'Did not find libavformat.a or avformat.lib, exiting!'
    Exit(1)

if not conf.CheckLibWithHeader('swscale', 'libswscale/swscale.h', 'c'):
    print 'Did not find libswscale.a or swscale.lib, exiting!'
    Exit(1)

python_lib_name = 'python'
python_inc_dir = ''
if not conf.CheckLibWithHeader('python', 'Python.h', 'c'):
    if conf.CheckLibWithHeader('python2.7', 'python2.7/Python.h', 'c'):
        python_lib_name = 'python2.7'
        python_inc_dir = '/usr/include/python2.7'
    else:
        print 'Did not find libpython.a or python.lib, exiting!'
        Exit(1)

env = conf.Finish()

env.Append(
    CPPPATH = [
        '#src',
        python_inc_dir,
    ],
    CXXFLAGS = [
        '-Wall',
        '-fPIC',
        '-DQPSNR_VERSION_MAJOR=' + qpsnrVersionMajor,
        '-DQPSNR_VERSION_MINOR=' + qpsnrVersionMinor,
        '-DQPSNR_VERSION_MICRO=' + qpsnrVersionMicro,
    ],
    LIBPATH = [
        '#src',
        python_inc_dir,
    ],
    SHLIBVERSION = qpsnrVersionStr,
    )

# Python environment
envPy.Replace(
    SWIGCXXFILESUFFIX= '_wrapPython$CXXFILESUFFIX',
    SHLIBPREFIX= '_',
    SHLIBSUFFIX= '.so',
    CXXFLAGS = [
        '-Wall',
    ],
    SWIGFLAGS = [
        '-python',
        '-c++',
        '-fcompact',
    ],
    LINKFLAGS = [
    ],
    LIBPATH = [
        "#src",
        python_inc_dir,
    ],
)

envPy.Append(
    CPPPATH = [
        "#src",
        python_inc_dir,
    ],
    SWIGPATH = envPy['CPPPATH']
)


Export('env')
Export('envPy')
Export('python_lib_name')

VariantDir('build/' + buildMode + '/src', 'src', duplicate = 0)
VariantDir('build/' + buildMode + '/app', 'app', duplicate = 0)

SConscript('src/SConscript', variant_dir = 'build/' + buildMode + '/src')
SConscript('app/SConscript', variant_dir = 'build/' + buildMode + '/app')

