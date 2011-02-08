redo-ifchange $1.cc

g++-4.1 -DUT_DSO_TAGINFO='"3262197cbf1f401f26860881492985f9224095ae917cb3873cfb8d515b4fe5cc34907fc09f452c70432cece667c8271c5bf9ef01fed8eb5353042de966c081ac5bbe45a0ff9249d6fa3c8d080af35ab981"' -DVERSION=\"11.0.581\" -D_GNU_SOURCE -DLINUX -DAMD64 -m64 -fPIC -DSIZEOF_VOID_P=8 -DSESI_LITTLE_ENDIAN -DENABLE_THREADS -DUSE_PTHREADS -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DNEED_SPECIALIZATION_STORAGE -c  -DGCC4 -DGCC3 -Wno-deprecated -I/opt/hfs11.0.581-gcc4.1/toolkit/include -I/opt/hfs11.0.581-gcc4.1/toolkit/include/htools -Wall -W -Wno-parentheses -Wno-sign-compare -Wno-reorder -Wno-uninitialized -Wunused -Wno-unused-parameter -O2 -DMAKING_DSO -o $3 $1.cc

