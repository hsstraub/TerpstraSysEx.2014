#!/bin/bash

USAGE="usage: package-linux-release [ version_id ] ?tidy"

LTE_BIN=`pwd`/../Builds/Linux/build/Lumatone\ Editor

if [[ ! $LTE_BIN ]]; then
    echo $USAGE
    echo No binary found.
    exit 1
fi

[[ $1 ]] && VERSION=$1
[[ ! $VERSION ]] && echo "No version specified" && exit 1
echo "preparing zip for version ${VERSION}"

[[ -d `pwd`/../Releases/ ]] || mkdir `pwd`/../Releases

PARENT_DIR=`pwd`/../Releases/Linux
TARGET=$PARENT_DIR/$VERSION

([[ -d $PARENT_DIR ]] && rm -r $TARGET/) || mkdir $PARENT_DIR
mkdir $TARGET

echo copying files...               && \
    cp "${LTE_BIN}" $TARGET/            && \
    cp ./install.sh $TARGET/    && \
    cp ../LICENSE $TARGET/              && \
    cp -r ../Presets $TARGET/           && \
    # todo install deps
    # todo readme
    COPIED=1

[[ ! $COPIED ]] && exit 1

cd ${TARGET}

ZIP_OUT="LumatoneEditor-${VERSION}-Linux.zip"

zip -r $ZIP_OUT ./* && ZIPPED=1
if [[ $ZIPPED ]]; then
    echo wrote $ZIP_OUT
else
    echo "Error zipping files for ${ZIP_OUT}"
    exit 1
fi

if [[ $2 == "tidy" ]]; then
    echo cleaning residuals...
    find ${TARGET}/* \( ! -name "*.zip" \) -delete
elif [[ $2 ]]; then
    echo unknown argument \"${2}\"
fi
