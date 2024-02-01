#!/usr/bin/env bash
set -e

PATH="/QOpenSys/pkgs/bin:$PATH"
DATE=$(date +"%Y-%m-%d_%H-%M-%S")
BRANCH="release-$DATE"
REMOTE="origin"
MAIN_BRANCH="master"

if [ ! -d node_modules ]; then
# install deps
npm install
fi

# checkout main branch and pull the latest changes
git checkout $MAIN_BRNACH
git pull $REMOTE $MAIN_BRANCH
# create release branch and run release
git checkout -b "$BRANCH" $REMOTE/$MAIN_BRANCH
npm run release
# push branch to upstream remote to open a PR
git push $REMOTE $BRANCH

