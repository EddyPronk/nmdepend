model=Experimental
echo step1
tclsh ../../Dart/Source/Client/DashboardManager.tcl DartConfiguration.tcl $model DashboardStart
echo step2
#tclsh ../../Dart/Source/Client/DashboardManager.tcl DartConfiguration.tcl $model Coverage
tclsh ../../Dart/Source/Client/DashboardManager.tcl DartConfiguration.tcl $model Start Test Coverage
echo step3
tclsh ../../Dart/Source/Client/DashboardManager.tcl DartConfiguration.tcl $model DashboardEnd
