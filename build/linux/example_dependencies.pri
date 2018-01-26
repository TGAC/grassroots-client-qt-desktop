#
# Edit these values for your grassroots installation
#

#
# This value is where your grassroots installation is located.
# The Qt client library will be copied to the the clients drawer
# located there and the libraries in the lib drawer there will 
# be used by this client
DIR_GRASSROOTS_INSTALL_ROOT = /home/billy/Applications/grassroots/0/grassroots/lib

# This is the path to the Grassroots core development packages
# containing the reuired header files
DIR_GRASSROOTS_CORE = ../../../../core

# This is the path to the iRODS installation below which will
# be such files as var/lib/irods/irodsctl.
# Generally this will be at /
DIR_IRODS_HOME = /


DIR_IRODS_BOOST_LIBS = /opt/irods-externals/boost1.60.0-0/lib

DIR_GRASSROOTS_EXTRAS = $$DIR_GRASSROOTS_INSTALL_ROOT/extras

DRMAA_TYPE = htcondor
GRASSROOTS_DRMAA_LIB_NAME = grassroots_drmaa
