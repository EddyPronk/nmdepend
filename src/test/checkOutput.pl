#!perl -w

$bindir=shift;
$expected=shift;
$packageLevel=shift;

print "checkOutput script...\n";
print "expected result in : $expected\n";
print "running nmdepend on directory : $bindir\n";

unlink("object.dot");
unlink("package.dot");

$cmd = "../nmdepend -n$packageLevel $bindir";
print "cmd : $cmd\n";
system($cmd);
$rc = system("diff $expected/package.dot.expected package.dot");

print "result : $rc\n";
if($rc == 0)
{
  $rc = system("diff $expected/object.dot.expected object.dot");
  print "result : $rc\n";
}

print "returning : $rc\n";

$rc = 1 if($rc ne 0);

exit($rc);
