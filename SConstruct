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
				'#src',
		],
		CXXFLAGS = [
				'-Wall',
				'-fPIC',
				'-DQPSNR_VERSION_MAJOR=' + qpsnrVersionMajor,
				'-DQPSNR_VERSION_MINOR=' + qpsnrVersionMinor,
				'-DQPSNR_VERSION_MICRO=' + qpsnrVersionMicro,
		],
		LIBPATH = [
				'#src'
		],
		SHLIBVERSION = qpsnrVersionStr,
		)


Export( 'env' )

VariantDir( 'build/' + buildMode + '/src', 'src', duplicate = 0 )
VariantDir( 'build/' + buildMode + '/app', 'app', duplicate = 0 )

SConscript( 'src/SConscript', variant_dir = 'build/' + buildMode + '/src' )
SConscript( 'app/SConscript', variant_dir = 'build/' + buildMode + '/app' )

