from distutils.core import setup, Extension, os

module1 = Extension(
	'_saga_api',

	sources = [
		'saga_api_wrap.cxx'
	],

	include_dirs = [
		os.environ['WXWIN'] + '/include',
		os.environ['WXWIN'] + '/include/msvc',
		'./'
	],

	library_dirs = [
		os.environ['WXWIN'] + '/lib/vc_lib',
		os.environ['SAGA'] + '/bin/saga_vc'
	],

	libraries = [
		'wxtiff',
		'wxjpeg',
		'wxpng',
		'wxzlib',
		'wxregex',
		'wxexpat',
		'wxmsw28_html',
		'wxmsw28_adv',
		'wxmsw28_core',
		'wxbase28_xml',
		'wxbase28',
		'winmm',
		'comctl32',
		'rpcrt4',
		'wsock32',
		'oleacc',
		'odbc32',
		'kernel32',
		'user32',
		'gdi32',
		'winspool',
		'comdlg32',
		'advapi32',
		'shell32',
		'ole32',
		'oleaut32',
		'uuid',
		'odbccp32',
		'saga_api'
	],

	extra_compile_args = [
		'-D__WXMSW__',
		'-D_FILE_OFFSET_BITS=64',
		'-D_LARGE_FILES',
		'-D_LARGEFILE_SOURCE=1',
		'-DNO_GCC_PRAGMA',
		'-D_TYPEDEF_BYTE',
		'-D_TYPEDEF_WORD',
		'-D_SAGA_API_EXPORTS',
		'-DDEBUG'
	]
)

setup(
	name 		= 'SAGA Python API',
	version 	= '0.1',
	description = '',
	ext_modules = [module1]
)