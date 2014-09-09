To Build
=======

clang connect.c -o connect -lcassandra
clang schema.c -o schema -lcassandra
clang prepare.c -o prepare -lcassandra
clang async.c -o async -lcassandra
