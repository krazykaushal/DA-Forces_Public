#! /bin/bash

qID=$1
qPath="Admin/Questions/$qID"
uID=$2
uPath="Admin/users/$uID"
iPath="$qPath/in.txt"
oPath="$qPath/out.txt"
vPath="verdict.txt"



if [[ ! -e $qPath ]] || [[ ! -e $uPath ]] || [[ ! -e $iPath ]] || [[ ! -e $oPath ]]
then
    exit 2
fi

gcc $uPath/$qID/submission.c -o $uPath/$qID/submission.out
chmod 777 $uPath/$qID/submission.out
./$uPath/$qID/submission.out < $iPath > $vPath
rm $uPath/$qID/submission.out

if [ $? == 0 ]
then
    cmp --silent $oPath $vPath && echo 'All Correct' > $vPath || echo 'Wrong Answer' > $vPath
    exit 0
else
    exit 1
fi
