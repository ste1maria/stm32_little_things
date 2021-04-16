#!/bin/bash
 echo -e >../Inc/git_version.h   && \
 echo -e '/** @File version.h \nGenerated file please do not change \n run ../version.sh to generate /n**/'>>../Inc/git_version.h   && \
 echo -e -n '#ifndef GIT_HASH  \n#define GIT_HASH "' >>../Inc/git_version.h  && \
 echo -n  $(git rev-parse HEAD |cut -c1-7) >>../Inc/git_version.h   && \
 echo -e -n   '"\n#endif\n' >>../Inc/git_version.h
     
    

