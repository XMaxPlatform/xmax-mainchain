#!/bin/sh

set -o xtrace
set -o verbose
set -o errexit

_SystemImageArch=$1
shift
_ToolchainArch=$1
shift

SDK_ROOT=$PWD/android_sdk
mkdir $SDK_ROOT

TOOLCHAIN=$PWD/android_toolchain
mkdir $TOOLCHAIN

API_VERSION=24

(
    cd $SDK_ROOT
    SDK_PACKAGE=sdk-tools-linux-3859397.zip
    curl -O https://dl.google.com/android/repository/$SDK_PACKAGE
    unzip $SDK_PACKAGE
    echo y | ./tools/bin/sdkmanager "platforms;android-24" "emulator" "ndk-bundle" "platform-tools" "build-tools;23.0.3" "system-images;android-24;google_apis;$_SystemImageArch"
)

python $SDK_ROOT/ndk-bundle/build/tools/make_standalone_toolchain.py --arch $_ToolchainArch --api $API_VERSION  --stl=libc++ --force  --install-dir $TOOLCHAIN

echo SDK_ROOT=${SDK_ROOT}
echo TOOLCHAIN=${TOOLCHAIN}
echo API_VERSION=${API_VERSION}
