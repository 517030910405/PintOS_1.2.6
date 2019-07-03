make clean
make
cd build/
#pintos -v -k -T 60 qemu  -- -q  run priority-donate-one < /dev/null #2> tests/threads/priority-donate-one.errors > tests/threads/priority-donate-one.output
#perl -I../.. ../../tests/threads/priority-donate-one.ck tests/threads/priority-donate-one tests/threads/priority-donate-one.result
#pintos -v -k -T 60 qemu  -- -q  run priority-donate-multiple2 < /dev/null #2> tests/threads/priority-donate-multiple2.errors > tests/threads/priority-donate-multiple2.output
#perl -I../.. ../../tests/threads/priority-donate-multiple2.ck tests/threads/priority-donate-multiple2 tests/threads/priority-donate-multiple2.result
#pintos -v -k -T 60 qemu  -- -q  run priority-donate-lower < /dev/null #2> tests/threads/priority-donate-lower.errors > tests/threads/priority-donate-lower.output
#perl -I../.. ../../tests/threads/priority-donate-lower.ck tests/threads/priority-donate-lower tests/threads/priority-donate-lower.result
#pintos -v -k -T 60 qemu  -- -q  run priority-condvar < /dev/null #2> tests/threads/priority-condvar.errors > tests/threads/priority-condvar.output
#perl -I../.. ../../tests/threads/priority-condvar.ck tests/threads/priority-condvar tests/threads/priority-condvar.result
#pintos -v -k -T 480 qemu  -- -q -mlfqs run mlfqs-load-1 < /dev/null  #2> tests/threads/mlfqs-load-1.errors > tests/threads/mlfqs-load-1.output
#perl -I../.. ../../tests/threads/mlfqs-load-1.ck tests/threads/mlfqs-load-1 tests/threads/mlfqs-load-1.result
#pintos -v -k -T 480 qemu  -- -q -mlfqs run mlfqs-load-60 < /dev/null 2> tests/threads/mlfqs-load-60.errors > tests/threads/mlfqs-load-60.output
pintos -v -k -T 480 qemu  -- -q -mlfqs run mlfqs-recent-1 < /dev/null 2> tests/threads/mlfqs-recent-1.errors > tests/threads/mlfqs-recent-1.output
perl -I../.. ../../tests/threads/mlfqs-recent-1.ck tests/threads/mlfqs-recent-1 tests/threads/mlfqs-recent-1.result
cd ..
