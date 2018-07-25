#!/bin/sh

echo -e "==========================================================="
echo -e "\033[1;36m开始卸载编译环境......\033[0m"
make distclean 1>/dev/null
\rm -f aclocal.m4 1>/dev/null
\rm -rf autom4te.cache 1>/dev/null
\rm -f conf.* 1>/dev/null
\rm -f config.* 1>/dev/null
\rm -f configure 1>/dev/null
\rm -f depcomp 1>/dev/null
\rm -f install-sh 1>/dev/null
\rm -f Makefile.in 1>/dev/null
\rm -f missing 1>/dev/null
unlink ltmain.sh 1>/dev/null
\rm -f *.in 1>/dev/null
sh clear.sh 1>/dev/null
echo -e "\033[1;36m编译环境卸载成功!\033[0m"