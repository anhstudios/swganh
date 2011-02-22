#!/bin/bash
# Idea from http://stackoverflow.com/questions/2144406/git-shallow-submodules
git submodule init
for i in $(git submodule | sed -e 's/.* //'); do
    spath=$(git config -f .gitmodules --get submodule.$i.path)
    surl=$(git config -f .gitmodules --get submodule.$i.url)
    git clone --depth 1 $surl $spath
done
git submodule update