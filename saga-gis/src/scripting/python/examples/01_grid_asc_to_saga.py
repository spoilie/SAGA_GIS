#! /usr/bin/env python

import saga_api, sys, os

##########################################
def grid_asc2sgrd(fASC):

### load all module libraries from a directory at once:
#    if os.name == 'nt':    # Windows
#        saga_api.SG_Get_Module_Library_Manager().Add_Directory(os.environ['SAGA_32' ], 0)
#    else:                  # Linux
#        saga_api.SG_Get_Module_Library_Manager().Add_Directory(os.environ['SAGA_MLB'], 0)
#    print '__________________'
#    print 'number of loaded libraries: ' + str(saga_api.SG_Get_Module_Library_Manager().Get_Count())
#    print saga_api.SG_Get_Module_Library_Manager().Get_Summary(saga_api.SG_SUMMARY_FMT_FLAT_NO_INTERACTIVE).c_str()
#    print '__________________'

### load just the needed module library:
    if os.name == 'nt':    # Windows
        saga_api.SG_Get_Module_Library_Manager().Add_Library(os.environ['SAGA_32' ] + '/modules/io_grid.dll')
    else:                  # Linux
        saga_api.SG_Get_Module_Library_Manager().Add_Library(os.environ['SAGA_MLB'] + '/libio_grid.so')

    sASC = saga_api.CSG_String(fASC)
    m    = saga_api.SG_Get_Module_Library_Manager().Get_Module(saga_api.CSG_String('io_grid'), 1)
    print m.Get_Description().c_str()

    p    = m.Get_Parameters()
    p(saga_api.CSG_String('FILE')).Set_Value(sASC)

    if m.Execute() == 0:
        print 'ERROR: executing module [' + m.Get_Name().c_str() + ']'
        return 0

    if p(saga_api.CSG_String('GRID')).asGrid().Save(sASC) == 0:
        print 'ERROR: saving grid [' + sASC + ']'
        return 0

    print 'success'
    return 1

##########################################
if __name__ == '__main__':
    print 'Python - Version ' + sys.version
    print saga_api.SAGA_API_Get_Version()
    print

    if len(sys.argv) != 2:
        print 'Usage: grid_asc_to_saga.py <in: ascii grid file>'
        print '... trying to run with test_data'
        fASC    = './test.asc'
    else:
        fASC    = sys.argv[1]
        if os.path.split(fASC)[0] == '':
            fASC    = './' + fASC
	
    grid_asc2sgrd(fASC)
