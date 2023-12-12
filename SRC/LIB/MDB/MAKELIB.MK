rm -f mdb.a
ar r mdb.a Btree.o LimitMan.o RateRule.o TransactOfAggregate.o
ranlib mdb.a 
