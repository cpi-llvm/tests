#! /usr/bin/env bash

export LD_LIBRARY_PATH="$PWD"

case $(uname) in
	FreeBSD) DL="" ;;
	Linux) DL="-ldl" ;;
esac

if [[ "$CC" = "" ]] ; then
    CC=$(which clang)
fi

if [[ "$TIMEOUT" = "" ]] ; then
    TIMEOUT=10s
fi

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:."

cmd() {
    echo "$@"
    "$@"
}

check() {
    rm -f bin stdout
    cmd "$CC" -O0 -g "$@" -o bin &&
    #timeout -s 9 -k 0 $TIMEOUT ./bin >stdout || { echo -e "  \e[31mFAIL\e[m"; return; }
    ./bin >stdout || { echo -e "  \e[31mFAIL\e[m"; return; }
    grep -q "Hello ./bin" stdout || { echo -e "  \e[31mFAIL\e[m"; return; }
    rm -f bin stdout
}

TIMEOUT=2s

# checks the availability of unsafe variables in the main thread
check basic.c
check -fPIC basic.c
check -fsanitize=safe-stack basic.c
check -fsanitize=safe-stack -fPIC basic.c
check -lpthread basic.c
check -lpthread -fPIC basic.c
check -lpthread -fsanitize=safe-stack basic.c
check -lpthread -fsanitize=safe-stack -fPIC basic.c

# checks the availability of unsafe variables in a thread
check -lpthread pthread.c
check -lpthread -fPIC pthread.c
check -lpthread -fsanitize=safe-stack pthread.c
check -lpthread -fsanitize=safe-stack -fPIC pthread.c

# checks the availability of unsafe variables in a thread,
# using user provided stack
check -lpthread pthread-user-stack.c
check -lpthread -fPIC pthread-user-stack.c
check -lpthread -fsanitize=safe-stack pthread-user-stack.c
check -lpthread -fsanitize=safe-stack -fPIC pthread-user-stack.c

# prepare dynamic libraries
cmd "$CC" -O0 -g -shared -fPIC libhello.c -o libhello.so
cmd "$CC" -O0 -g -shared -fPIC -fsanitize=safe-stack libhello.c -o libhello-safestack.so
cmd "$CC" -O0 -g -shared -fPIC libhello-pthread.c -o libhello-pthread.so -lpthread
cmd "$CC" -O0 -g -shared -fPIC -fsanitize=safe-stack libhello-pthread.c -o libhello-pthread-safestack.so -lpthread

# check linking to dynamic libraries
check link.c -L. -lhello
check link.c -L. -lhello-pthread
check link.c -L. -lhello-safestack
check link.c -L. -lhello-pthread-safestack
check -fPIC link.c -L. -lhello
check -fPIC link.c -L. -lhello-pthread
check -fPIC link.c -L. -lhello-safestack
check -fPIC link.c -L. -lhello-pthread-safestack
check -fsanitize=safe-stack link.c -L. -lhello
check -fsanitize=safe-stack link.c -L. -lhello-pthread
check -fsanitize=safe-stack link.c -L. -lhello-safestack
check -fsanitize=safe-stack link.c -L. -lhello-pthread-safestack
check -fsanitize=safe-stack -fPIC link.c -L. -lhello
check -fsanitize=safe-stack -fPIC link.c -L. -lhello-pthread
check -fsanitize=safe-stack -fPIC link.c -L. -lhello-safestack
check -fsanitize=safe-stack -fPIC link.c -L. -lhello-pthread-safestack

# check dlopen
check_dlopen() {
	dlopen=$@
	check $DL $dlopen -DSHARED_LIBRARY=\"./libhello.so\"
	check $DL $dlopen -DSHARED_LIBRARY=\"./libhello-pthread.so\"
	check $DL $dlopen -DSHARED_LIBRARY=\"./libhello-safestack.so\"
	check $DL $dlopen -DSHARED_LIBRARY=\"./libhello-pthread-safestack.so\"
	check $DL -fPIC $dlopen -DSHARED_LIBRARY=\"./libhello.so\"
	check $DL -fPIC $dlopen -DSHARED_LIBRARY=\"./libhello-pthread.so\"
	check $DL -fPIC $dlopen -DSHARED_LIBRARY=\"./libhello-safestack.so\"
	check $DL -fPIC $dlopen -DSHARED_LIBRARY=\"./libhello-pthread-safestack.so\"
	check $DL -fsanitize=safe-stack $dlopen -DSHARED_LIBRARY=\"./libhello.so\"
	check $DL -fsanitize=safe-stack $dlopen -DSHARED_LIBRARY=\"./libhello-pthread.so\"
	check $DL -fsanitize=safe-stack $dlopen -DSHARED_LIBRARY=\"./libhello-safestack.so\"
	check $DL -fsanitize=safe-stack $dlopen -DSHARED_LIBRARY=\"./libhello-pthread-safestack.so\"
	check $DL -fsanitize=safe-stack -fPIC $dlopen -DSHARED_LIBRARY=\"./libhello.so\"
	check $DL -fsanitize=safe-stack -fPIC $dlopen -DSHARED_LIBRARY=\"./libhello-pthread.so\"
	check $DL -fsanitize=safe-stack -fPIC $dlopen -DSHARED_LIBRARY=\"./libhello-safestack.so\"
	check $DL -fsanitize=safe-stack -fPIC $dlopen -DSHARED_LIBRARY=\"./libhello-pthread-safestack.so\"

	check $DL $dlopen -lpthread -DSHARED_LIBRARY=\"././libhello.so\"
	check $DL $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-pthread.so\"
	check $DL $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-safestack.so\"
	check $DL $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-pthread-safestack.so\"
	check $DL -fPIC $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello.so\"
	check $DL -fPIC $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-pthread.so\"
	check $DL -fPIC $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-safestack.so\"
	check $DL -fPIC $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-pthread-safestack.so\"
	check $DL -fsanitize=safe-stack $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello.so\"
	check $DL -fsanitize=safe-stack $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-pthread.so\"
	check $DL -fsanitize=safe-stack $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-safestack.so\"
	check $DL -fsanitize=safe-stack $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-pthread-safestack.so\"
	check $DL -fsanitize=safe-stack -fPIC $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello.so\"
	check $DL -fsanitize=safe-stack -fPIC $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-pthread.so\"
	check $DL -fsanitize=safe-stack -fPIC $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-safestack.so\"
	check $DL -fsanitize=safe-stack -fPIC $dlopen -lpthread -DSHARED_LIBRARY=\"./libhello-pthread-safestack.so\"
}

check_dlopen dlopen.c
check_dlopen dlopen-pthread-pre.c -lpthread
check_dlopen dlopen-pthread-post.c -lpthread

# check 10000 unsafe stack allocation for memory leak
TIMEOUT=20s
check -lpthread 1001-threads.c
check -lpthread -fPIC 1001-threads.c
check -lpthread -fsanitize=safe-stack 1001-threads.c
check -lpthread -fsanitize=safe-stack -fPIC 1001-threads.c

# cleanup
rm -f bin stdout *.so
