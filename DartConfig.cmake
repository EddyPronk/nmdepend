#
# Dart server to submit results (used by client)
#
SET (DROP_SITE "localhost")
SET (DROP_LOCATION "/incoming")
SET (DROP_SITE_USER "anonymous")
SET (DROP_SITE_PASSWORD "dart-tester@somewhere.com")
SET (TRIGGER_SITE "http://${DROP_SITE}/cgi-bin/dart-dt.pl")

#
# Dart server configuration 
#
#SET (CVS_WEB_URL "http://${DROP_SITE}/cgi-bin/cvsweb.cgi/dt/")
#SET (CVS_WEB_CVSROOT "dt")
SET (CVS_WEB_URL "http://${DROP_SITE}/")
SET (DOXYGEN_URL "http://${DROP_SITE}/" )
SET (GNATS_WEB_URL "http://${DROP_SITE}/")

#
# Copy over the testing logo
#
#CONFIGURE_FILE(${DART_TEST_SOURCE_DIR}/TestingLogo.gif ${DART_TEST_BINARY_DIR}/Testing/HTML/TestingResults/Icons/Logo.gif COPYONLY)
