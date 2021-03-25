rm ./*.o; rm ./searxcli;

GCCVARS="-c"
test "$1" = "DEBUG" && GCCVARS="-c -g"

gcc dirhelper.c $GCCVARS; gcc parameters.c $GCCVARS; gcc jsonhelper.c $GCCVARS; gcc curlhelper.c $GCCVARS; gcc searxcli.c $GCCVARS; gcc dirhelper.o parameters.o jsonhelper.o curlhelper.o searxcli.o -lconfig -largtable2 -lcurl -lssl -lcrypto -ljson-c -o searxcli
